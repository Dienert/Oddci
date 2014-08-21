
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
#include "Controller.h"
#include <typeinfo>
#include <vector>
#include "Cycle.h"
#include "PNA.h"
#include <string>
#include <simtime.h>
#include <unitconversion.h>
#include "ReactiveProvisioner.h"
#include "ProactiveProvisioner.h"
#include "OddciException.h"

#define throw_line(arg) throw OddciException(arg, __FILE__, __LINE__);

using namespace std;

#define MIN_PACKET_SIZE	   1024

Define_Module(Controller);

Controller::Controller() {
}

Controller::~Controller() {
    cOwnedObject *Del=NULL;
    int OwnedSize = this->defaultListSize();
    for(int i=0;i<OwnedSize;i++){
            Del=this->defaultListGet(0);
            this->drop(Del);
            delete Del;
    }
}

void Controller::initialize() {

    time (&start);

    address = par("addr");
    transmitterAddr = par("transmitterAddr");

    datarate = par("broadcastBandwidth_");

    receivingMsgLog = par("receivingMsgLog");
    wakeupLog = par("wakeupLog");
    bool wholeLog = par("wholeLog");
    OddciUtils::setLogActive(wholeLog);

    experimentId = par("expId");

    char expId[10];

    sprintf(expId, "%03d", experimentId);

    experimentIdFormated = string(expId);

    finishCycleShift = par("finishCycleShift_");

    repetitions = par("expRepetitions");
    currentRepetition = par("currRepetition");

    initialAvailability = par("initialAvailability_").doubleValue();
    volatility = par("volatility_").doubleValue();
    const char* heuri = par("heuris");
    const char* met = par("estimationMethod");
    if (strcmp(met, "Informative") == 0) {
        pnaEstimator = new InformativePNAEstimator();
    } else {
        ExploratoryPNAEstimator *expEstimator = new ExploratoryPNAEstimator(par("probFactor").doubleValue());
        expEstimator->setFactorIncrement(par("factorIncrement_").doubleValue());
        pnaEstimator = expEstimator;
    }
    const char* selec = par("selec");
    if (strcmp(selec, "Tempestive") == 0) {
        pnaSelector = new TempestiveSelector();
    } else {
        pnaSelector = new RankingSelector();
    }
    if (strcmp(heuri, "Reactive") == 0) {
        provisioner = new ReactiveProvisioner(pnaSelector, pnaEstimator);
    } else {
        provisioner = new ProactiveProvisioner(pnaSelector, pnaEstimator);
    }
    experimentCount = -1;

    resultsDir = "./results/";
    workloadsDir = "./experiments/workloads/";
    resultsFileName = getName(provisioner, pnaEstimator, pnaSelector);
    string cyclesPath = resultsDir + "cycles-exp-" + experimentIdFormated +"-" + resultsFileName + ".txt";
    string subCyclesPath = resultsDir + "subcycles-exp-" + experimentIdFormated +"-"+ resultsFileName + ".txt";

    cycleResultsFile.close();
    cycleResultsFile.open(cyclesPath.c_str(), ios_base::app);

    subCycleResultsFile.close();
    subCycleResultsFile.open(subCyclesPath.c_str(), ios_base::app);

    cycleResultsFile << experimentId << endl;

    totalDevNum = par("devNum");
    cycleInterval = SimTime::parse(par("cycleInt"));
    informativeProbeInterval = SimTime::parse(par("probeIntervalInformative_"));
    subCycleInterval = SimTime::parse(par("subCycleInt"));
    evaluatableSubCycles = cycleInterval/subCycleInterval;

    provisioner->setCycleInterval(cycleInterval);
    provisioner->setSubCycleInterval(subCycleInterval);
    provisioner->setEvaluationShift(SimTime::parse(par("evaluationShift_")));

    finishSubCycleShift = SimTime::parse(par("finishSubCycleShift_"));

    lastWakeupTransmissionFinish = 0;

    imageSize = par("imageSize_").stdstringValue();

    volatilityFactor = 0;

    string logPath = experimentIdFormated+"-"+resultsFileName + ".out";
    char *logPathChar = new char[logPath.size() + 1];
    strcpy ( logPathChar, logPath.c_str() );
    OddciUtils::setLogFile(logPathChar);

    volatilityFactor = new double;
    *volatilityFactor = 0;
    scheduledWakeups = 0;

    initExperiment();

}

void Controller::initExperiment() throw () {


    //TODO ranqueamento inicial deve ser baseado na média que o controller recebe das sondas
    vector<int> rankings = OddciUtils::getIntsFromString(par("ranks"));

    experimentCount++;
    isFirstInstance = true;

    instances.clear();

    while(!instances.empty()) delete instances.back(), instances.pop_back();

    instances.clear();

    finishedInstances = 0;
    string workloadPath = workloadsDir;
    const char *tmp = par("workload_");
    workloadPath += tmp;
    ifstream workloadsFile(workloadPath.c_str());

    workloadsFile >> instancesNumber;

    Instance *fakeInstance = new Instance(0, evaluatableSubCycles);

    char changeCycleTimerName[20];
    sprintf(changeCycleTimerName, "%s %d", "timer changeCycle", 0);
    cMessage *timerChangeCycle = new cMessage(changeCycleTimerName);
    timerChangeCycle->setKind(CHANGE_CYCLE);
    fakeInstance->setTimerChangeCycle(timerChangeCycle);
    instances.push_back(fakeInstance);

    if (pnaEstimator->getMethod() == PNAEstimator::INFORMATIVE) {
        InformativePNAEstimator *informativeEstimator = dynamic_cast<InformativePNAEstimator *>(pnaEstimator);
        informativeEstimator->setFakeInstance(fakeInstance);
        scheduleAt(simTime() + SimTime::parse("5min"), timerChangeCycle); // conta os PNAS idle em 5 minutos
    }

    for (int i = 1; i <= instancesNumber; i++) {
        Instance *instance = new Instance(i, evaluatableSubCycles);
        char startTime[50];
        workloadsFile >> startTime;
        instance->setTimeToStart(SimTime::parse(startTime));
        int requestedDevices;
        workloadsFile >> requestedDevices;
        instance->setRequestedDevices(requestedDevices);
        int requestedSlots;
        workloadsFile >> requestedSlots;
        instance->setRequestedSlots(requestedSlots);
        instance->setImageSize((int)UnitConversion::parseQuantity(imageSize.c_str(), "B")); // todas estão com o mesmo tamanho
        instance->setMaxRanking(rankings.at(0)); // todas com o mesmo alvo
        instance->setCompletedSlots(0);
        instance->setCurrentCycle(0);

        instances.push_back(instance);

        char evaluationTimerName[20];
        sprintf(evaluationTimerName, "%s %d", "timer evaluation", i);

        cMessage *timerMessage = new cMessage(evaluationTimerName);
        timerMessage->setKind(EVALUATION);
        instance->setTimerEvaluation(timerMessage);

        // timer para mandar o primeiro wakeup
        scheduleAt(instance->getTimeToStart(), timerMessage);

        // timer para mudar o ciclo corrente para o próximo ciclo e adicionar um ciclo
        char changeCycleTimerName[20];
        sprintf(changeCycleTimerName, "%s %d", "timer changeCycle", instance->getId());
        cMessage *timerAddCycle = new cMessage(changeCycleTimerName);
        timerAddCycle->setKind(CHANGE_CYCLE);
        instance->setTimerChangeCycle(timerAddCycle);

        // este timer só é agendado quando o subciclo é iniciado (no término do envio do wakeup)
        char finishSubCycleTimerName[20];
        sprintf(finishSubCycleTimerName, "%s %d", "timer finishSubCycle", instance->getId());
        cMessage *timerFinishSubCycle = new cMessage(finishSubCycleTimerName);
        timerFinishSubCycle->setKind(CHANGE_SUBCYCLE);
        instance->setTimerChangeSubCycle(timerFinishSubCycle);

        char scheduleWakeupTimerName[20];
        sprintf(scheduleWakeupTimerName, "%s", "timer scheduleBroadcastMessage");
        // timer para enviar mensagens de broadcast
        cMessage *scheduleBrodcastMesssage = new cMessage(scheduleWakeupTimerName);
        scheduleBrodcastMesssage->setKind(SCHEDULE_WAKEUP);
        instance->setTimerBroadcastSchedule(scheduleBrodcastMesssage);

    }
}

string Controller::getName(Provisioner *provisioner, PNAEstimator *pnaEstimator, PNASelector *selector) {
    if (provisioner->getHeuristic() == Provisioner::PROACTIVE) {
        strcpy(heuristicName, string("Proactive").c_str());
    } else {
        strcpy(heuristicName, string("Reactive").c_str());
    }
    if (pnaEstimator->getMethod() == PNAEstimator::INFORMATIVE) {
        strcpy(estMethodName, string("Informative").c_str());
    } else {
        strcpy(estMethodName, string("Exploratory").c_str());
    }
    if (selector->getSelection() == PNASelector::TEMPESTIVE) {
        strcpy(selectionName, string("Tempestive").c_str());
    } else {
        strcpy(selectionName, string("Ranking").c_str());
    }
    string name = heuristicName;
    name += "-";
    name += estMethodName;
    name += "-";
    name += selectionName;
    return name;
}

void Controller::evaluateInstance() throw () {
    try {
        Instance *instance = instances.at(currentInstance);
        Cycle * currentCycle = instance->getCurrentCycle();
        long currentCycleId = currentCycle->getId();

        if (instance->getCompletedSlots() < instance->getRequestedSlots()) {

            SubCycle *subCycle = currentCycle->getCurrentSubCycle();
            SubCycle *prevSubCycle = currentCycle->getPrevSubCycle();

//            double failRate = 0;
//            subCycle->setRequestingDevices(provisioner->getNextRequestingDevices(currentCycle));
            subCycle->setRequestingDevices(provisioner->getNextRequestingDevices(currentCycle, volatilityFactor));
            subCycle->setFactor(pnaEstimator->getNextSubCycleFactor(currentCycle));
            subCycle->setRankingTarget(pnaSelector->getNextRankingTarget(currentCycle));

            bool sendWakeup = provisioner->isSendWakeup(currentCycle); // verifica se eh para mandar wakeup (depois da redundancia, fator e ranking)

            vector<string> vetor;
            vetor.push_back("### Avaliando subciclo ");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getCycle()->getId()));
            vetor.push_back(".");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getId()));
            vetor.push_back("para começar subciclo");
            vetor.push_back(OddciUtils::itoa(subCycle->getCycle()->getId()));
            vetor.push_back(".");
            vetor.push_back(OddciUtils::itoa(subCycle->getId()));
            vetor.push_back("da instancia");
            vetor.push_back(OddciUtils::itoa(instance->getId()));
            vetor.push_back("\n### Serão solicitados");
            vetor.push_back(OddciUtils::itoa(subCycle->getRequestingDevices()));
            vetor.push_back("| responderam");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getAllocatedPNAs()+prevSubCycle->getDiscardedPNAs()));
            vetor.push_back("| alocados");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getAllocatedPNAs()));
            vetor.push_back("| descartados");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getDiscardedPNAs()));
            vetor.push_back("| ativos");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getWorkingPNAs()));
            vetor.push_back("/");
            vetor.push_back(OddciUtils::itoa(instance->getRequestedDevices()));
            vetor.push_back("| falharam");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getFailedDevices()));
            vetor.push_back("| volatilidade");
            vetor.push_back(OddciUtils::dtoa(prevSubCycle->getObservedVolatility()));
            vetor.push_back("| startedSlots");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getStartedSlots()));
            vetor.push_back("| CompletedSlots");
            vetor.push_back(OddciUtils::itoa(prevSubCycle->getCompletedSlots()));
            vetor.push_back("| wakeup");
            string str = sendWakeup ? "sim" : "não";
            vetor.push_back(str);
            vetor.push_back("|");
            vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
            OddciUtils::print(vetor);

            WakeupMsg *message;
            message = new WakeupMsg("broadcast");
            message->setMessageKind(ODDCI_WAKEUP);
            message->setInstanceId(currentInstance);
            message->setCycleId(currentCycleId);
            message->setFactor(subCycle->getFactor());
            message->setRankingTarget(subCycle->getRankingTarget());
            message->setDestAddress(transmitterAddr);
            message->setSrcAddress(address);
            message->setByteLength(instance->getImageSize());



            if (sendWakeup) { // TODO: colocar em outra função
                Cycle *cycle = subCycle->getCycle();
                sendWakeUp(message);

                if (wakeupLog) {
                    vector<string> vetor;
                    vetor.push_back("### Instancia");
                    vetor.push_back(OddciUtils::itoa(instance->getId()));
                    vetor.push_back("| Ciclo");
                    vetor.push_back(OddciUtils::itoa(cycle->getId()));
                    vetor.push_back("| WM");
                    vetor.push_back(OddciUtils::itoa(cycle->getNumberOfWakeUps()));
                    vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                    OddciUtils::print(vetor);
                }
            }

            simtime_t now = simTime();
            simtime_t endWakeupTransmission = now < lastWakeupTransmissionFinish ? lastWakeupTransmissionFinish : now;

            provisioner->posEvaluation(message->dup(),subCycle, endWakeupTransmission, subCycleInterval, this);

        } else {
            finishInstance(instance);
        }
    } catch (out_of_range& oor) {
//        throw_line("## Id de instancia incorreto");
    }


}

void Controller::scheduleWakeUp(WakeupMsg *message, simtime_t startTime) {

    try {
        Instance *instance = instances.at(message->getInstanceId());
        cMessage * timerBroadcastSchedule = instance->getTimerBroadcastSchedule();
        timerBroadcastSchedule->removeObject(message);
        timerBroadcastSchedule->addObject(message);

        vector<string> vetor;
        vetor.push_back("### Agendando Wakeup da Instancia ");
        vetor.push_back(OddciUtils::itoa(message->getInstanceId()));
        vetor.push_back("| para ");
        vetor.push_back(OddciUtils::getDurationFromSeconds(startTime.dbl()));
        OddciUtils::print(vetor);

        cancelEvent(timerBroadcastSchedule);
        scheduleAt(startTime, timerBroadcastSchedule);
    } catch (out_of_range& oor) {
        delete message;
//        throw_line("### Id de instancia incorreto");
    }

}

void Controller::sendWakeUp(WakeupMsg *message) {

    try {

        if (simTime() > lastWakeupTransmissionFinish) {
            lastWakeupTransmissionFinish = simTime();
        }

        vector<string> vetor;
        vetor.push_back("### instancia");
        vetor.push_back(OddciUtils::itoa(message->getInstanceId()));
        vetor.push_back("iniciará envio de wakeup em");
        vetor.push_back(OddciUtils::getDurationFromSeconds(lastWakeupTransmissionFinish.dbl()));
        OddciUtils::print(vetor);

        cMessage * timerBroadcastSend = getNewSendBroadcastMessageTimer();
        timerBroadcastSend->removeObject(message);
        timerBroadcastSend->addObject(message);

        // transformando bytes em bits e dividindo por bits por segundo
        lastWakeupTransmissionFinish += message->getByteLength() * 8 / datarate;
    //    simtime_t subCycleStart = lastWakeupTransmissionFinish;
        cancelEvent(timerBroadcastSend);
        scheduleAt(lastWakeupTransmissionFinish, timerBroadcastSend);

        scheduledWakeups++;
    } catch (out_of_range& oor) {
        delete message;
//       throw_line("### Id de instancia incorreto");
    }

}

void Controller::scheduleNextEvaluation(simtime_t nextEvaluationTime, Instance *instance) {

    instance->setNextEvaluationTime(nextEvaluationTime);

    vector<string> vetor;
    vetor.push_back("### Próxima avaliação na hora");
    vetor.push_back(OddciUtils::getDurationFromSeconds(nextEvaluationTime.dbl()));
    OddciUtils::print(vetor);

    cancelEvent(instance->getTimerEvaluation());
    scheduleAt(nextEvaluationTime, instance->getTimerEvaluation()); // TODO considerar o tempo de transmissão retornado em cima da fila

}

void Controller::startInstance(Instance *instance) {

    // timer para gravar o ciclo anterior
    char recordCycleTimerName[20];
    sprintf(recordCycleTimerName, "%s %d", "timer recordCycle", instance->getId());
    cMessage *timerRecorder = new cMessage(recordCycleTimerName);
    timerRecorder->setKind(RECORD_CYCLE);
    instance->setTimerRecordCycle(timerRecorder);
    scheduleAt(simTime() + 2 * cycleInterval , timerRecorder);

    simtime_t changeSubCycle = simTime() + subCycleInterval - finishSubCycleShift;

    vector<string> vetor;
    vetor.push_back("### instancia");
    vetor.push_back(OddciUtils::itoa(instance->getId()));
    vetor.push_back("iniciada | O subciclo 0 . 0 vai terminar na hora");
    vetor.push_back(OddciUtils::getDurationFromSeconds(changeSubCycle.dbl()));
    OddciUtils::print(vetor);

    cancelEvent(instance->getTimerChangeSubCycle());
    scheduleAt(changeSubCycle, instance->getTimerChangeSubCycle());

}

void Controller::recordSample(SubCycle *subCycle, string sampleId, string value) throw() {
    Cycle *cycle = subCycle->getCycle();
    Instance *instance = cycle->getInstance();
    subCycleResultsFile
            << heuristicName[0] << ","
            << estMethodName[0] << ","
            << selectionName[0] << ","
            << experimentId << ","
            << currentRepetition << ","
            << instance->getId() << ","
            << cycle->getId() << ","
            << subCycle->getId() << ","
            << sampleId << ","
            << value << ","
            << OddciUtils::getDurationFromSecondsWithDots(simTime().dbl()) << "\n";

    subCycleResultsFile.close();
    string path = resultsDir + "subcycles-exp-" + experimentIdFormated +"-"+resultsFileName + ".txt";
    subCycleResultsFile.open(path.c_str(), ios_base::app);

}

void Controller::recordSubCycle(SubCycle *subCycle) throw() {
    Instance *instance = subCycle->getCycle()->getInstance();

    recordSample(subCycle, "QDRI", OddciUtils::itoa(instance->getRequestedDevices()));
    recordSample(subCycle, "TSRI", OddciUtils::itoa(instance->getRequestedSlots()));

    recordSample(subCycle, "TDER", OddciUtils::itoa(totalDevNum));

    recordSample(subCycle, "TDLR", OddciUtils::itoa(PNA::getOnPNAs()));
    recordSample(subCycle, "TDAR", OddciUtils::itoa(PNA::getBusyPNAs()));
    recordSample(subCycle, "TDIR", OddciUtils::itoa(PNA::getIdlePNAs()));
    recordSample(subCycle, "TDDR", OddciUtils::itoa(PNA::getOffPNAs()));

    recordSample(subCycle, "VORS", OddciUtils::dtoa(subCycle->getObservedVolatility())); //alterei aqui
    recordSample(subCycle, "TERS", OddciUtils::itoa(pnaEstimator->getEstimatedIdlePNAS()));

    recordSample(subCycle, "DAIS", OddciUtils::itoa(subCycle->getWorkingPNAs()));
    recordSample(subCycle, "DRIS", OddciUtils::itoa(subCycle->getRequestingDevices()));
    recordSample(subCycle, "DLIS", OddciUtils::itoa(subCycle->getAllocatedPNAs()));
    recordSample(subCycle, "DDIS", OddciUtils::itoa(subCycle->getDiscardedPNAs()));

    recordSample(subCycle, "SIIS", OddciUtils::itoa(subCycle->getStartedSlots())); //alterei aqui
    recordSample(subCycle, "SCIS", OddciUtils::itoa(subCycle->getCompletedSlots())); //alterei aqui

    recordSample(subCycle, "QWIS", OddciUtils::itoa(subCycle->getNumberOfWakeUps())); //alterei aqui
    recordSample(subCycle, "TMFS", OddciUtils::itoa(scheduledWakeups));
//    recordSample(subCycle, "QSIS", OddciUtils::itoa(instance->getRequestedDevices()));

}

void Controller::recordCycle(Cycle *cycle) throw () {
    Instance *instance = cycle->getInstance();
    instance->setCompletedSlots(instance->getCompletedSlots() + cycle->getCompletedSlots());
    if (cycle->getCompletedSlots() == 0) {
        instance->setCompletedWithZeroCount(instance->getCompletedWithZeroCount() + 1);
        if (instance->getCompletedWithZeroCount() == 5) {
            string warning;
            warning += getName(provisioner, pnaEstimator, pnaSelector);
            warning += " ";
            warning += "Dispositivos insuficientes";
//            OddciUtils::stop(warning);
            cancelEvent(instance->getTimerEvaluation());
            KillMsg *msg = new KillMsg("broadcast");
            msg->setMessageKind(ODDCI_KILL);
            msg->setDestAddress(transmitterAddr);
            msg->setSrcAddress(address);

            finishInstance(cycle->getInstance());
            send(msg, "g$o");
        }
    } else {
        instance->setCompletedWithZeroCount(0);
    }

    vector<SubCycle *>::iterator scy;

    int discardedPNAs = 0;
    int workingPNAs = 0;
    int heartBeats = 0;
    for (int i=0; i < cycle->getSubCyclesNumber();i++) {
        SubCycle * subCycle = cycle->getSubCycles()->at(i);
        heartBeats += subCycle->getWorkingPNAs();
        discardedPNAs += subCycle->getDiscardedPNAs();
        workingPNAs += subCycle->getWorkingPNAs();
    }

    double progress = 0;
    for (int i=0;i<=cycle->getId();i++) {
        Cycle *cycle = instance->getCycles()->at(i);
        progress += cycle->getCompletedSlots();
    }

    progress /= instance->getRequestedSlots();

    workingPNAs /= cycle->getSubCyclesNumber();

    cycle->setDiscardedPNAs(discardedPNAs);
    cycle->setWorkingPNAs(workingPNAs);
    cycle->setHeartBeats(heartBeats);
    cycle->setProgress(progress);
    time (&currentTime);
    double diff = difftime (currentTime,start);

    cycleResultsFile
            << experimentId << "\t"
            << heuristicName << "\t"
            << estMethodName << "\t"
            << selectionName << "\t"
            << totalDevNum << "\t"
            << imageSize << "\t"
            << initialAvailability*100 << "%" << "\t"
            << ((totalDevNum != 0) ? OddciUtils::convertLocalNumber(PNA::getOnPNAs()/(double)totalDevNum) : "0") << "\t"
            << volatility*100 << "%" << "\t"
            << currentRepetition << "\t"
            << instance->getId() << "\t"
            << OddciUtils::getDurationFromSeconds(simTime().dbl()) << "\t"
            << OddciUtils::getDurationFromSeconds(diff) << "\t"
            << cycle->getId() << "\t"
            << PNA::getOnPNAs() << "\t"
            << PNA::getBusyPNAs() << "\t"
            << PNA::getIdlePNAs() << "\t"
            << PNA::getOffPNAs() << "\t"
            << cycle->getAllocatedPNAs() << "\t"
            << cycle->getDiscardedPNAs() << "\t"
            << workingPNAs << "\t"
            << cycle->getStartedSlots() << "\t"
            << cycle->getCompletedSlots() << "\t"
            << instance->getRequestedDevices() << "\t"
            << instance->getCompletedSlots() << "\t"
            << instance->getRequestedSlots() << "\t"
            << cycle->getNumberOfWakeUps() << "\t"
            << scheduledWakeups << "\t"
            << cycle->getSubCyclesNumber() << "\t"
            << heartBeats << "\t"
            << OddciUtils::convertLocalNumber(provisioner->getParallelism(cycle)) << "\t"
            << OddciUtils::convertLocalNumber(cycle->getDiscard()) << "\t"
            << OddciUtils::convertLocalNumber(cycle->getObservedVolatility()) << "\t"
            << OddciUtils::convertLocalNumber(PNA::getBusyPNAs()/(double)PNA::getOnPNAs()) << "\t"
            << OddciUtils::convertLocalNumber(cycle->getProgress()) << "\t"
            << endl ;

    cycleResultsFile.close();
    string path = resultsDir + "cycles-exp-" + experimentIdFormated +"-"+ resultsFileName + ".txt";
    cycleResultsFile.open(path.c_str(), ios_base::app);

    cycle->setRecorded(true);

    instance->setCycleToRecord(instance->getCycleToRecord() + 1);
}

void Controller::finishInstance(Instance *instance) throw () {
    instance->setFinished(true);
    finishedInstances++;

    bool stopExperiments = false;
    if (finishedInstances == instancesNumber) {
        stopExperiments = true;
    }

    double averageWorkingPNAs = 0;
    double averageCompletedSlots = 0;
    double averageParallelism = 0;

    int cycleId = 0;
    Cycle *cycle;
    bool finished = true;
    try {
        do {
            cycle = instance->getCycles()->at(cycleId);
            if (cycle->isRecorded()) {
                averageWorkingPNAs += cycle->getWorkingPNAs();
                averageCompletedSlots += cycle->getCompletedSlots();
                averageParallelism += cycle->getParallelism();
                cycleId++;
            } else {
                finished = false;
                break;
            }
        } while (cycle->getProgress() < 1);
    } catch (...) {
        finished = false;
    }

    if (cycleId != 0) {
        averageWorkingPNAs /= (cycleId);
        averageCompletedSlots /= (cycleId);
        averageParallelism /= (cycleId);
    } else {

    }

    string result = "finished";
    if (!finished) {
        result = "aborted";
    }
        cycleResultsFile
                << experimentId << "\t"
                << result << "\t" //heuristicName
                << "\t" //estMethodName
                << "\t" //selectionName
                << "\t" //totalDevNum
                << "\t" //image
                << "\t" //initialAvailability
                << "\t" //availability
                << "\t" //volatility
                << "\t" //currentRepetition
                << instance->getId() << "\t" //instance->getId()
                << "\t" //simulation time
                << "\t" //real time
                << "\t" //cycle->getId()
                << "\t" //PNA::getOnPNAs()
                << "\t" //PNA::getBusyPNAs()
                << "\t" //PNA::getIdlePNAs()
                << "\t" //PNA::getOffPNAs()
                << "\t" //cycle->getAllocatedPNAs()
                << "mean" << "\t" //cycle->getDiscardedPNAs()
                << OddciUtils::convertLocalNumber(averageWorkingPNAs) << "\t" //workingPNAs
                << "mean" << "\t" //cycle->getStartedSlots()
                << OddciUtils::convertLocalNumber(averageCompletedSlots) << "\t" //cycle->getCompletedSlots()
                << "\t" //instance->getRequestedDevices()
                << "\t" //instance->getCompletedSlots()
                << "\t" //instance->getRequestedSlots()
                << "\t" //cycle->getNumberOfWakeUps()
                << "\t" //totalWakeups
                << "\t" //subCycles
                << "mean" << "\t" //heartBeats
                << OddciUtils::convertLocalNumber(averageParallelism) << "\t" //paralelism
                << "\t" //discard
                << "\t" //observedVolatility
                << "\t" //total busy
                << "\t" //progress
                << endl ;

    cycleResultsFile.close();
    string path = resultsDir + "cycles-exp-" + experimentIdFormated +"-"+ resultsFileName + ".txt";
    cycleResultsFile.open(path.c_str(), ios_base::app);

    if (!stopExperiments) {
        DieMsg *message = new DieMsg("broadcast");
        message->setMessageKind(ODDCI_DIE);
        message->setInstanceId(instance->getId());
        message->setDestAddress(transmitterAddr);
        message->setSrcAddress(address);
        cancelAndDelete(instance->getTimerEvaluation());
        instance->setTimerEvaluation(NULL);
        cancelAndDelete(instance->getTimerChangeCycle());
        instance->setTimerChangeCycle(NULL);
        cancelAndDelete(instance->getTimerChangeSubCycle());
        instance->setTimerChangeSubCycle(NULL);
        cancelAndDelete(instance->getTimerRecordCycle());
        instance->setTimerRecordCycle(NULL);
        cancelAndDelete(instance->getTimerBroadcastSchedule());
        instance->setTimerBroadcastSchedule(NULL);
        instance->getCycles()->clear();

        vector<string> vetor;
        vetor.push_back("### Enviando die via broadcast para instancia");
        vetor.push_back(OddciUtils::itoa(instance->getId()));
        vetor.push_back("|");
        vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
        OddciUtils::print(vetor);

        send(message, "g$o");
    } else {
        KillMsg *message = new KillMsg("broadcast");
        message->setMessageKind(ODDCI_KILL);
        message->setDestAddress(transmitterAddr);
        message->setSrcAddress(address);
        //OddciUtils::stop(fileName);

        send(message, "g$o");

        for (vector<Instance *>::iterator it = instances.begin(); it != instances.end(); it++) {
            Instance *instance = *it;
            if (instance->getId() == 0) {
                cancelAndDelete(instance->getTimerChangeCycle());
                instance->setTimerChangeCycle(NULL);
            } else {
                cancelAndDelete(instance->getTimerEvaluation());
                instance->setTimerEvaluation(NULL);
                cancelAndDelete(instance->getTimerChangeCycle());
                instance->setTimerChangeCycle(NULL);
                cancelAndDelete(instance->getTimerChangeSubCycle());
                instance->setTimerChangeSubCycle(NULL);
                cancelAndDelete(instance->getTimerRecordCycle());
                instance->setTimerRecordCycle(NULL);
                cancelAndDelete(instance->getTimerBroadcastSchedule());
                instance->setTimerBroadcastSchedule(NULL);
            }
        }

    }
}

cMessage * Controller::getNewSendBroadcastMessageTimer() {
    char sendWakeupTimerName[20];
    sprintf(sendWakeupTimerName, "%s", "timer sendBroadcastMessage");

    // timer para enviar mensagens de broadcast
    cMessage *sendBrodcastMesssage = new cMessage(sendWakeupTimerName);
    sendBrodcastMesssage->setKind(SEND_BROADCAST);

    return sendBrodcastMesssage;
}

void Controller::handleTimer(cMessage *msg) throw () {
	vector<string> timerName = OddciUtils::getStringsFromSplitedChars(msg->getName());
    switch (msg->getKind()) {
        case EVALUATION: {
            int instanceId = OddciUtils::atoi(timerName.at(2));
            currentInstance = instanceId;
            evaluateInstance();
            break;
        }
        case RECORD_CYCLE: {
            try {
                int instanceId = OddciUtils::atoi(timerName.at(2));
                Instance *instance = instances.at(instanceId);
                int cycleToRecord = instance->getCycleToRecord();
                if (cycleToRecord <= instance->getCurrentCycle()->getId() && simTime() >= instance->getTimeToStart()+2*cycleInterval) {
                    Cycle *cycle = instance->getCycles()->at(cycleToRecord);
                    recordCycle(cycle);
                }

                char recordCycleTimerName[20];
                sprintf(recordCycleTimerName, "%s %d", "timer recordCycle", instanceId);
                cMessage *timerRecorder = new cMessage(recordCycleTimerName);
                timerRecorder->setKind(RECORD_CYCLE);
                instance->setTimerRecordCycle(timerRecorder);

                if (!instance->isFinished()) {
                    cancelEvent(instance->getTimerRecordCycle());
                    scheduleAt(simTime() + cycleInterval, instance->getTimerRecordCycle());
                }
            } catch(out_of_range& oor) {
                delete msg;
//                throw_line("### Id de instancia incorreto");
            }

            break;
        }
        case CHANGE_CYCLE: {
            try {
                int instanceId = OddciUtils::atoi(timerName.at(2));
                Instance *instance = instances.at(instanceId);
                long cycleCount = instance->getCurrentCycle()->getId();
                cycleCount++;
                Cycle *nextNextCycle = new Cycle(cycleCount+1, evaluatableSubCycles, instance); // adiciona próximo do próximo ciclo
                vector<Cycle *> * cycles = instance->getCycles();
                cycles->push_back(nextNextCycle);
                instance->setCurrentCycle(cycleCount); // incrementa o ciclo atual para o próximo ciclo

                if (instance->getId() == 0) {
                    cancelEvent(instance->getTimerChangeCycle());
                    scheduleAt(simTime() + informativeProbeInterval, instance->getTimerChangeCycle());
                } else {
                    vector<string> vetor;
                    vetor.push_back("### Terminando ciclo");
                    vetor.push_back(OddciUtils::itoa(cycleCount-1));
                    vetor.push_back("| instancia");
                    vetor.push_back(OddciUtils::itoa(instance->getId()));
                    vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                    vetor.push_back("\n");
                    OddciUtils::print(vetor);

                    currentInstance = instanceId;
                    evaluateInstance();

                }
            } catch (out_of_range& oor) {
                delete msg;
//                throw_line("### Id de instancia incorreto");
            }
            break;
        }
        case CHANGE_SUBCYCLE: {
            try {
                int instanceId = OddciUtils::atoi(timerName.at(2));
                Instance *instance = instances.at(instanceId);
                Cycle *currentCycle = instance->getCurrentCycle();
                SubCycle *currentSubCycle = currentCycle->getCurrentSubCycle();
                int subCycleCount = currentSubCycle->getId();

                vector<string> vetor;
                vetor.push_back("### Terminando subciclo");
                vetor.push_back(OddciUtils::itoa(currentCycle->getId()));
                vetor.push_back(".");
                vetor.push_back(OddciUtils::itoa(currentSubCycle->getId()));
                vetor.push_back("| instancia");
                vetor.push_back(OddciUtils::itoa(instance->getId()));
                vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                vetor.push_back("\n");
                OddciUtils::print(vetor);

                recordSubCycle(currentSubCycle);

                if (subCycleCount == currentCycle->getSubCyclesNumber()-1) {
                    cancelEvent(instance->getTimerChangeCycle());
                    scheduleAt(simTime(), instance->getTimerChangeCycle());
                } else {
                    currentSubCycle->setFinished(true);
                    subCycleCount++;
                    currentCycle->setCurrentSubCycleId(subCycleCount); // incrementa o subciclo atual para o próximo subciclo
                    currentInstance = instanceId;
                    evaluateInstance();
                }

                if (instance->getTimerChangeSubCycle()) {
                    cancelEvent(instance->getTimerChangeSubCycle());
                    scheduleAt(simTime() + subCycleInterval, instance->getTimerChangeSubCycle());
                }
            } catch (out_of_range& oor) {
                delete msg;
//                throw_line("### Id de instancia incorreto");
            }
            break;
        }
        case SEND_BROADCAST: {
            OddciMessage *message = check_and_cast<OddciMessage *>(msg->getObject("broadcast"));
            msg->removeObject(message);
            try {
                Instance *instance = instances.at(message->getInstanceId());

                vector<string> vetor;
                vetor.push_back("### Mensagem de broadcast transmitida | instancia");
                vetor.push_back(OddciUtils::itoa(message->getInstanceId()));
                vetor.push_back("|");
                vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                OddciUtils::print(vetor);

                Cycle *cycle = instance->getCurrentCycle();
                SubCycle *subCycle = cycle->getCurrentSubCycle();

                cycle->setNumberOfWakeUps(cycle->getNumberOfWakeUps() + 1);
                subCycle->setNumberOfWakeUps(subCycle->getNumberOfWakeUps() + 1);

                if (cycle->getId() == 0 && subCycle->getId() == 0 && cycle->getNumberOfWakeUps() == 1) {
                    startInstance(instance);
                }
                send(message, "g$o");
                scheduledWakeups--;
            } catch (out_of_range& oor) {
                delete msg;
//                string errorMessage = "### Id de instancia incorreto ";
//                errorMessage += OddciUtils::itoa(message->getInstanceId());
//                errorMessage += " | ";
//                errorMessage += OddciUtils::itoa(instances.size());
//                errorMessage += " instancias";
//                throw_line(errorMessage);
            }
            break;
        }
        case SCHEDULE_WAKEUP: {
            try {
                WakeupMsg *message = check_and_cast<WakeupMsg *>(msg->getObject("broadcast"));
                msg->removeObject(message);

                Instance *instance = instances.at(message->getInstanceId());
                Cycle *cycle = instance->getCycles()->at(message->getCycleId());
                SubCycle *subCycle = cycle->getCurrentSubCycle();

                if (subCycle->getDeficitDevices() > 0) {

                    double factor = subCycle->getFailedDevices()/(float) pnaEstimator->getEstimatedIdlePNAS();
                    subCycle->setRequestingDevices(subCycle->getRequestingDevices()+subCycle->getFailedDevices());
                    message->setFactor(factor);

                    sendWakeUp(message);
                }
            } catch(out_of_range& oor) {
                delete msg;
//                throw_line("### Id de instancia incorreto");
            }
            break;
        }
    }
}

void Controller::handleMessage(cMessage *msg) {

    try {

        if (msg->isSelfMessage()) { // Self-Message from Controller Server - response to Provider

            vector<string> timerName = OddciUtils::getStringsFromSplitedChars(msg->getName());
            if (timerName.at(0).compare("timer") == 0) {
                handleTimer(msg);
            } else {
                send(msg, "g$o");
            }
        } else { // Mensagens do Provider e dos PNAs para o Controller Server

            OddciMessage *message = check_and_cast<OddciMessage *>(msg);
            switch (message->getMessageKind()) {
                case ODDCI_WAITING : {
                    WaitingMsg *message = check_and_cast<WaitingMsg *>(msg);

                    int respondingCycleId = message->getCycleId();

                    Instance *instance = instances.at(message->getInstanceId());

                    if (!instance->isFinished()) {

                        Cycle *respondingCycle = instance->getCycles()->at(respondingCycleId);
                        Cycle *currentCycle = instance->getCurrentCycle();
                        SubCycle *currentSubCycle = currentCycle->getCurrentSubCycle();

                        if (message->getSlotCompleted()) {

                            respondingCycle->setCompletedSlots(respondingCycle->getCompletedSlots() + 1);
                            currentSubCycle->setCompletedSlots(currentSubCycle->getCompletedSlots() + 1);

                            SubCycle *nextSubCycle = currentSubCycle->getNextSubCycle();
                            nextSubCycle->setStartedSlots(nextSubCycle->getStartedSlots()+1);

                            Cycle *nextCycle = instance->getCycles()->at(respondingCycleId+1);
                            nextCycle->setStartedSlots(nextCycle->getStartedSlots() + 1);

                            simtime_t subcycleChangeTime = instance->getTimerChangeSubCycle()->getArrivalTime();

                            simtime_t firstProbe;

                            simtime_t lastHeartBeat = message->getLastHeartBeatTime();


                            if (provisioner->getHeuristic() == Provisioner::PROACTIVE) {
                                if ((subcycleChangeTime - lastHeartBeat) < subCycleInterval) { // a última sonda foi neste subciclo
                                    firstProbe = subcycleChangeTime - simTime()+subCycleInterval*0.9; // sonda deve vir no final do próximo subciclo
                                } else {
                                    firstProbe = subcycleChangeTime - simTime()-subCycleInterval*0.1; // sonda deve vir no final deste subciclo
                                    if (firstProbe < 0) {
                                        firstProbe = subCycleInterval;
                                        currentSubCycle->setWorkingPNAs(currentSubCycle->getWorkingPNAs()+1);
                                    }
                                }
                            } else {
                                // a última sonda foi neste subciclo
                                if ((subcycleChangeTime - lastHeartBeat) < subCycleInterval) {
                                    // sonda deve vir antes do meio do próximo subciclo mas não conta como ativo, pois já foi contado
                                    firstProbe = subcycleChangeTime - simTime()+subCycleInterval/2*0.9;
                                } else {
                                    // sonda deve vir antes do meio do deste subciclo
                                    firstProbe = subcycleChangeTime - simTime()-subCycleInterval/2*1.1;
                                    if (firstProbe < 0) {
                                        // sonda deve vir antes do meio do próximo subciclo, mas conta como ativo
                                        firstProbe = subcycleChangeTime - simTime()+subCycleInterval/2*0.9;
                                        currentSubCycle->setWorkingPNAs(currentSubCycle->getWorkingPNAs()+1);
                                    }
                                }
                            }

                            if (firstProbe < 0) {
                               vector<string> vetor;
                               vetor.push_back("### intervalo para sonda negativa no Controller para PNA ");
                               vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                               vetor.push_back("| instancia ");
                               vetor.push_back(OddciUtils::itoa(instance->getId()));
                               vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                               OddciUtils::print(vetor);
                            }

                            WorkMsg * work = new WorkMsg("work");
                            work->setMessageKind(ODDCI_WORK);
                            work->setInstanceId(instance->getId());
                            work->setFirstHeartBeatInterval(firstProbe);
                            if (work->getFirstHeartBeatInterval() < 0) {
                               vector<string> vetor;
                               vetor.push_back("### intervalo para sonda negativa no Controller para PNA ");
                               vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                               vetor.push_back("| instancia ");
                               vetor.push_back(OddciUtils::itoa(instance->getId()));
                               vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                               OddciUtils::print(vetor);
                            }
                            work->setSlotInterval(cycleInterval);
                            work->setHeartBeatInterval(subCycleInterval);
                            work->setCycleId(respondingCycle->getId()+1); // inicia no subciclo 0 do próximo ciclo
                            work->setSrcAddress(address);
                            work->setDestAddress(message->getSrcAddress());
                            send(work, "g$o");

                            if (receivingMsgLog) {
                                vector<string> vetor;
                                vetor.push_back(OddciUtils::itoa(respondingCycle->getCompletedSlots()));
                                vetor.push_back("waiting aceito | outro slot | instancia");
                                vetor.push_back(OddciUtils::itoa(instance->getId()));
                                vetor.push_back("| subciclo corrente");
                                vetor.push_back(OddciUtils::itoa(instance->getCurrentCycle()->getId()));
                                vetor.push_back(".");
                                vetor.push_back(OddciUtils::itoa(instance->getCurrentCycle()->getCurrentSubCycle()->getId()));
                                vetor.push_back("| slot completo -> ciclo");
                                vetor.push_back(OddciUtils::itoa(respondingCycleId));
                                vetor.push_back("| iniciado p/subCiclo");
                                vetor.push_back(OddciUtils::itoa(respondingCycleId+1));
                                vetor.push_back(".");
                                vetor.push_back(OddciUtils::itoa(0));
                                vetor.push_back("| pna");
                                vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                                vetor.push_back("|");
                                vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                                OddciUtils::print(vetor);
                            }

                            if (instance->getCompletedSlots() >= instance->getRequestedSlots() && !instance->isFinished()) {
                                finishInstance(instance);
                            }

                        } else { // estão querendo entrar na instancia

                            if (respondingCycleId <= instance->getCurrentCycle()->getId() && (currentSubCycle->getAllocatedPNAs() < currentSubCycle->getRequestingDevices())) {

                                currentSubCycle->setAllocatedPNAs(currentSubCycle->getAllocatedPNAs() + 1);
                                currentSubCycle->setStartedSlots(currentSubCycle->getStartedSlots() + 1);
                                respondingCycle->setStartedSlots(respondingCycle->getStartedSlots() + 1);
                                respondingCycle->setAllocatedPNAs(respondingCycle->getAllocatedPNAs() + 1);

                                WorkMsg * work = new WorkMsg("work");
                                work->setMessageKind(ODDCI_WORK);
                                work->setInstanceId(instance->getId());
                                work->setSlotInterval(cycleInterval-finishCycleShift);

                                simtime_t subcycleChangeTime = instance->getTimerChangeSubCycle()->getArrivalTime();
                                simtime_t nextBroadcastTime = instance->getTimerBroadcastSchedule()->getArrivalTime();

                                simtime_t firstProbe;
                                bool alreadyCountedWorking = false;


                                if (provisioner->getHeuristic() == Provisioner::PROACTIVE) {
                                    // sonda deve vir no final deste subciclo
                                    firstProbe = subcycleChangeTime - simTime()-subCycleInterval*0.1;
                                } else { // heusística reativa
                                    // se está antes do wakeup agendado no meio do subciclo
                                    if (nextBroadcastTime > simTime()) {
                                        // sonda deve vir antes do próximo wakeup
                                        firstProbe = nextBroadcastTime - simTime()-subCycleInterval*0.1;
                                    } else { // se o próximo evento é o fim do subciclo - após o envio do wakeup do meio
                                        // sonda deve vir antes do meio do próximo subciclo
                                        firstProbe = subcycleChangeTime - simTime()+subCycleInterval/2*0.9;
                                        currentSubCycle->setWorkingPNAs(currentSubCycle->getWorkingPNAs()+1);
                                        alreadyCountedWorking = true;
                                    }
                                }

                                if (firstProbe < 0) {
                                    firstProbe = subCycleInterval;
                                    if (!alreadyCountedWorking) {
                                        currentSubCycle->setWorkingPNAs(currentSubCycle->getWorkingPNAs()+1);
                                    }
                                }
                                if (firstProbe < 0) {
                                   vector<string> vetor;
                                   vetor.push_back("### intervalo para sonda negativa no Controller antes de setar na mensagem para PNA ");
                                   vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                                   vetor.push_back("| instancia ");
                                   vetor.push_back(OddciUtils::itoa(instance->getId()));
                                   vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                                   OddciUtils::print(vetor);
                                }

                                work->setFirstHeartBeatInterval(firstProbe);
                                if (work->getFirstHeartBeatInterval() < 0) {
                                   vector<string> vetor;
                                   vetor.push_back("### intervalo para sonda negativa no Controller para PNA ");
                                   vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                                   vetor.push_back("| instancia ");
                                   vetor.push_back(OddciUtils::itoa(instance->getId()));
                                   vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                                   OddciUtils::print(vetor);
                                }
                                work->setHeartBeatInterval(subCycleInterval);
                                work->setCycleId(currentCycle->getId());
                                work->setSrcAddress(address);
                                work->setDestAddress(message->getSrcAddress());
                                send(work, "g$o");

                               if (receivingMsgLog) {
                                    vector<string> vetor;
                                    vetor.push_back(OddciUtils::itoa(currentSubCycle->getAllocatedPNAs()));
                                    vetor.push_back("waiting aceito | novo slot | instancia");
                                    vetor.push_back(OddciUtils::itoa(instance->getId()));
                                    vetor.push_back("| subciclo corrente");
                                    vetor.push_back(OddciUtils::itoa(instance->getCurrentCycle()->getId()));
                                    vetor.push_back(".");
                                    vetor.push_back(OddciUtils::itoa(instance->getCurrentCycle()->getCurrentSubCycle()->getId()));
                                    vetor.push_back("| iniciado p/subCiclo");
                                    vetor.push_back(OddciUtils::itoa(respondingCycleId));
                                    vetor.push_back(".");
                                    vetor.push_back(OddciUtils::itoa(0));
                                    vetor.push_back("| pna");
                                    vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                                    vetor.push_back("|");
                                    vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                                    OddciUtils::print(vetor);
                                }

                            } else {
                                currentSubCycle->setDiscardedPNAs(currentSubCycle->getDiscardedPNAs() + 1);
                                DieMsg * die = new DieMsg("die");
                                die->setMessageKind(ODDCI_DIE);
                                die->setInstanceId(message->getInstanceId());
                                die->setSrcAddress(address);
                                die->setDestAddress(message->getSrcAddress());
                                send(die, "g$o");

                                if (receivingMsgLog) {
                                    vector<string> vetor;
                                    vetor.push_back(OddciUtils::itoa(currentSubCycle->getDiscardedPNAs()));
                                    vetor.push_back("waiting descartado (número requisitado atendido) | instancia");
                                    vetor.push_back(OddciUtils::itoa(instance->getId()));
                                    vetor.push_back("| subciclo corrente");
                                    vetor.push_back(OddciUtils::itoa(instance->getCurrentCycle()->getId()));
                                    vetor.push_back(".");
                                    vetor.push_back(OddciUtils::itoa(instance->getCurrentCycle()->getCurrentSubCycle()->getId()));
                                    vetor.push_back("| pna");
                                    vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                                    vetor.push_back("|");
                                    vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                                    OddciUtils::print(vetor);
                                }
                            }
                        }
                    }
                    break;
                }
                case ODDCI_HEARTBEAT : {

                    HeartBeatMsg *message = check_and_cast<HeartBeatMsg *>(msg);

                    if (message) {

                        Instance *instance = instances.at(message->getInstanceId());

                        if (!instance->isFinished()) {

                            Cycle *currentCycle = instance->getCurrentCycle();
                            SubCycle *currentSubCycle = currentCycle->getCurrentSubCycle();
                            currentSubCycle->setWorkingPNAs(currentSubCycle->getWorkingPNAs()+1);

                            if (receivingMsgLog) {
                                vector<string> vetor;
                                vetor.push_back(OddciUtils::itoa(currentSubCycle->getWorkingPNAs()));
                                vetor.push_back("heartBeat para instancia");
                                vetor.push_back(OddciUtils::itoa(instance->getId()));
                                vetor.push_back("| subciclo corrente");
                                vetor.push_back(OddciUtils::itoa(instance->getCurrentCycle()->getId()));
                                vetor.push_back(".");
                                vetor.push_back(OddciUtils::itoa(instance->getCycles()->at(instance->getCurrentCycle()->getId())->getCurrentSubCycle()->getId()));
                                vetor.push_back("| pna");
                                vetor.push_back(OddciUtils::itoa(message->getSrcAddress()));
                                vetor.push_back("|");
                                vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                                OddciUtils::print(vetor);
                            }
                        }
                    }
                    break;
                }
                case ODDCI_IDLE_PROBE : {
                    Instance *fakeInstance = instances.at(0);
                    Cycle *cycle = fakeInstance->getCycles()->at(fakeInstance->getCurrentCycle()->getId());
                    cycle->setStartedSlots(cycle->getStartedSlots() + 1);
                    break;
                }
                case ODDCI_WAKEUP : {
                    break;
                }
                }
            delete msg;
        }

    } catch (out_of_range& oor) {
        delete msg;
//        throw_line("### Id de instancia incorreto");
    }
}

void Controller::setStatusString(const char *s) {
    if (ev.isGUI()) getDisplayString().setTagArg("t", 0, s);
}

void Controller::finish() {
    cycleResultsFile.close();
}

