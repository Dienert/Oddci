
#include "PNA.h"
#include <stdlib.h>
#include <algorithm>
#include "OddciUtils.h"
#include <unitconversion.h>

using namespace std;

Define_Module(PNA);

int PNA::onPNAs = 0;
int PNA::idlePNAs = 0;
int PNA::busyPNAs = 0;
int PNA::waitingPNAs = 0;
int PNA::numDevices = 0;

void PNA::initialize() {

    addr = par("addr");
    controllerAddr = par("controllerAddr");

    numDevices = par("numDevices");

    pnaMaxDelay = par("pnaMaxDelay_").doubleValue();

    slotTimer = new cMessage("slotTimer");
    heartBeatTimer = new cMessage("heartbeatTimer");
    slotTimer->setKind(WAITING);
    heartBeatTimer->setKind(HEARTBEAT);

    checkAvailabilityTimer = new cMessage("timerCheckAvailability");
    checkAvailabilityTimer->setKind(CHECK_AVAILABILITY);

    liveOrDieTimer = new cMessage("timerLiveOrDie");
    liveOrDieTimer->setKind(LIVEORDIE);

    checkInterval = par("checkInterval");

    initialAvailability = par("initialAvailability_").doubleValue();
//    pnaAverageSessionTime = par("pnaAverageSessionTime_").doubleValue();

    const char* met = par("estimationMethod");
    if (strcmp(met, "informative") == 0) {
        method = PNAEstimator::INFORMATIVE;
    } else {
        method = PNAEstimator::EXPLORATORY;
    }
    state = OFF;

    firstLiveOrDie = true;

    activeIntervals = 0;

    slotCompleted = false;

    waitingTimeOut = par("waitingTimeOut_");

    volatility = par("volatility_");

    cycleInterval = SimTime::parse(par("cycleInterval_")).dbl();
    informativeProbeInterval = SimTime::parse(par("informativeProbeInterval_"));

    scheduleAt(simTime(), liveOrDieTimer);

}

void PNA::sendMessage() {

    switch (state) {
		case BUSY: {
            HeartBeatMsg *message = new HeartBeatMsg("heartBeat");
            message->setMessageKind(ODDCI_HEARTBEAT);
//            subCycle++;
//            message->setCycle(cycle+1);
//            message->setSubCycle(subCycle);
            message->setInstanceId(instanceId);
            message->setSrcAddress(addr);
            message->setDestAddress(controllerAddr);

            cancelEvent(heartBeatTimer);

            if ((slotTimer->getArrivalTime() - simTime() > heartBeatInterval)) { // se ainda há tempo para uma nova sonda, então será agendada
                scheduleAt(simTime() + heartBeatInterval, heartBeatTimer);
            }
            send(message, "g$o");
			break;
		}
		case WAITING : {
			WaitingMsg *message = new WaitingMsg("waiting");
            message->setMessageKind(ODDCI_WAITING);
			message->setCycleId(cycle+1);
//			message->setSubCycleId(subCycle+1);
			message->setSlotCompleted(slotCompleted);
			message->setInstanceId(instanceId);
			message->setSrcAddress(addr);
			message->setDestAddress(controllerAddr);

            if (slotCompleted) {
                message->setLastHeartBeatTime(heartBeatTimer->getArrivalTime());
                slotCompleted = false;
                cycle++;
            }

			slotTimer->setKind(CANCEL_WAITING);
			cancelEvent(slotTimer);
			scheduleAt(simTime() + waitingTimeOut, slotTimer);

			send(message, "g$o");
			break;
		}
		case IDLE :	{
		    IdleProbeMsg *message = new IdleProbeMsg("idleProbe");
            message->setMessageKind(ODDCI_IDLE_PROBE);
            message->setSrcAddress(addr);
            message->setDestAddress(controllerAddr);
			send(message, "g$o");

			slotTimer->setKind(IDLE_PROBE);
			cancelEvent(slotTimer);
			scheduleAt(simTime() + informativeProbeInterval, slotTimer);

		    break;
		}
		case OFF : break;
    }
}

void PNA::handleTimer(cMessage *msg) {
    switch (msg->getKind()) {
        case INITIAL_WAITING:
        case HEARTBEAT:
            slotCompleted = false;
            sendMessage();
            break;
        case SLOT_WAITING:
            if (state == BUSY) { // só manda heartBeat se estiver ativo
                slotCompleted = true;
                state = WAITING;
                waitingPNAs++;
                busyPNAs--;
                sendMessage();
            }
//            else if (state == FINISHED) {
//                slotCompleted = true;
//                state = IDLE;
//                idlePNAs++;
//                busyPNAs--;
//                sendMessage();
//            }

            break;
        case IDLE_PROBE:
            sendMessage(); // active OPEN
            break;
        case LIVEORDIE:
            liveOrDie();
            break;
        case CHECK_AVAILABILITY:
            checkAvailability();
            break;
        case CANCEL_WAITING:
        	cancelWaiting();
        	break;
    }
}

void PNA::checkAvailability() {
    if (state == BUSY || state == IDLE) {
        activeIntervals++;
        checkAvailabilityTimer->setKind(CHECK_AVAILABILITY);
        scheduleAt(simTime() + checkInterval, checkAvailabilityTimer);
    } else {
        activeIntervals = 0;
    }
}

void PNA::cancelWaiting() {
    if (state == WAITING) {
        waitingPNAs--;
        idlePNAs++;
        state = IDLE;
    }
}

void PNA::liveOrDie() {

    int nextState;

    double nextLiveOrDie = cycleInterval;

    if (firstLiveOrDie) {
        nextLiveOrDie = uniform(0, cycleInterval);
        nextState = bernoulli(initialAvailability);
        firstLiveOrDie = false;
    } else {
        if (state == OFF ) {
            nextState = bernoulli( (numDevices * initialAvailability * volatility) / (numDevices * (1 - initialAvailability) ) );
        } else {
            nextState = bernoulli(1-volatility);
        }
    }
    if (!(bool)nextState) { // desligando
        if (state != OFF) {
            onPNAs--;
            if (state == IDLE) { // estava IDLE
                idlePNAs--;
            } else if (state == BUSY) {
                busyPNAs--;
            } else if (state == WAITING) {
                waitingPNAs--;
            }
            activeIntervals = 0;
            cancelEvent(slotTimer); // Ficou OFF então não envia sonda idle
            cancelEvent(heartBeatTimer); // Ficou OFF então não envia heartbeat
            cancelEvent(checkAvailabilityTimer); // Ficou OFF então não checa a disponibilidade
        }
        state = OFF;
        instanceId = 0;

    } else { // Ligando
        if (state == OFF) { // se estava OFF
            onPNAs++;
            idlePNAs++;
            state = IDLE;
            checkAvailabilityTimer->setKind(CHECK_AVAILABILITY); // se está idle dispara o verificador de disponibilidade
            scheduleAt(simTime() + checkInterval, checkAvailabilityTimer);

            slotTimer->setKind(IDLE_PROBE);
            scheduleAt(simTime(), slotTimer); // envia sonda idle
        }
    }
    scheduleAt(simTime() + nextLiveOrDie, liveOrDieTimer);

}


//void PNA::liveOrDie2() {
//    int live;
//
//    if (firstLiveOrDie) {
//        live = bernoulli(initialAvailability);
//        firstLiveOrDie = false;
//    } else {
//        if (state == OFF) {
//            live = 1; //vai ser ligado
//        } else {
//            live = 0; //vai ser desligado
//        }
//    }
//    if (!(bool)live) { // desligando
//        if (state != OFF) {
//            onPNAs--;
//            if (state == IDLE) { // estava IDLE
//                idlePNAs--;
//            } else if (state == BUSY) {
//                busyPNAs--;
//            } else if (state == WAITING) {
//                waitingPNAs--;
//            }
//            activeIntervals = 0;
//        }
//        state = OFF;
//
//        double alpha = (24-pnaAverageSessionTime)/(24-pnaAverageSessionTime-1);
//
//        long nextLiveOrDie = pareto_shifted(alpha, 1, 0) * 60 * 60;
//        scheduleAt(simTime() + nextLiveOrDie, liveOrDieTimer);
//
//        cancelEvent(checkAvailabilityTimer); // Ficou OFF então não checa a disponibilidade
//    } else { // Ligando
//        if (state == OFF) { // se estava OFF
//            onPNAs++;
//            idlePNAs++;
//            state = IDLE;
//            checkAvailabilityTimer->setKind(CHECK_AVAILABILITY); // se está idle dispara o verificador de disponibilidade
//            scheduleAt(simTime() + checkInterval, checkAvailabilityTimer);
//        }
//
//        double alpha = pnaAverageSessionTime/(pnaAverageSessionTime-1);
//
//        long nextLiveOrDie = pareto_shifted(alpha, 1, 0) * 60 * 60; // TODO: fazer um varredura de 24 horas // VOLATILIDADE
//        scheduleAt(simTime() + nextLiveOrDie, liveOrDieTimer);
//        // se estava WORKING não faz nada
//    }
//
//}

void PNA::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        handleTimer(msg);
    } else {
        OddciMessage * message = check_and_cast<OddciMessage *>(msg);
        switch (message->getMessageKind()) {
            case ODDCI_WAKEUP : {
                WakeupMsg * message = check_and_cast<WakeupMsg *>(msg);
                if (message) {
                    switch (state) {
                        case IDLE: {
                            bool result = bernoulli(message->getFactor());
                            if (result && activeIntervals >= message->getRankingTarget()) {
                                state = WAITING;
                                waitingPNAs++;
                                idlePNAs--;
                                cycle = message->getCycleId() - 1; // vai ser alterado para o correto na mensagem de work
//                                subCycle = 0; // vai ser alterado para o correto na mensagem de work
                                instanceId = message->getInstanceId();
                                slotTimer->setKind(INITIAL_WAITING);
                                cancelEvent(slotTimer);
                                scheduleAt(simTime()+uniform(0,pnaMaxDelay), slotTimer);
                            } else {
    //                            EV << "Wake up received, but the factor didn't pass";
                            }
                            break;
                        }
                        case BUSY:
    //                        EV << "Wake up ignored " << addr << " " << stateToStr() << "\n";
                            break;
                        case WAITING:
    //                        EV << "Wake up ignored " << addr << " " << stateToStr() << "\n";
                            break;
                        case OFF:
    //                        EV << "Wake up discarded " << addr << " " << stateToStr() << "\n";
                            break;
                    }
                }
                break;
            }
            case ODDCI_DIE : {
                if (state == BUSY) {
                    DieMsg * message = check_and_cast<DieMsg *>(msg);
                    if (message && (message->getInstanceId() == instanceId)) {

                        vector<string> vetor;
                        vetor.push_back("### Recebendo die para a instancia");
                        vetor.push_back(OddciUtils::itoa(message->getInstanceId()));
                        vetor.push_back("| pna ");
                        vetor.push_back(OddciUtils::itoa(addr));
                        vetor.push_back("|");
                        vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                        OddciUtils::print(vetor);

                        state = IDLE;
                        busyPNAs--;
                        idlePNAs++;
                        cancelEvent(slotTimer);
                        if (method == PNAEstimator::INFORMATIVE) {
                            slotTimer->setKind(IDLE_PROBE);
                            scheduleAt(simTime(), slotTimer); // envia sonda idle
                        }
                    }
                } else if (state == WAITING) {
                    DieMsg * message = check_and_cast<DieMsg *>(msg);
                    if (message && (message->getInstanceId() == instanceId)) {
                        state = IDLE;
                        waitingPNAs--;
                        idlePNAs++;
                        cancelEvent(slotTimer);
                        if (method == PNAEstimator::INFORMATIVE) {
                            slotTimer->setKind(IDLE_PROBE);
                            scheduleAt(simTime(), slotTimer); // envia sonda idle
                        }
                    }
                }
                break;
            }
            case ODDCI_WORK : {
                if (state == WAITING) {
                    WorkMsg * message = check_and_cast<WorkMsg *>(msg);
        //            EV << "Wake up received " << addr << "\n";
                    state = BUSY;
                    busyPNAs++;
                    waitingPNAs--;
                    slotInterval = message->getSlotInterval();
                    instanceId = message->getInstanceId();
                    cycle = message->getCycleId() - 1; // é incrementado na hora de mandar a sonda
                    heartBeatInterval = message->getHeartBeatInterval();
                    heartBeatTimer->setKind(HEARTBEAT);
                    cancelEvent(heartBeatTimer);
                    if (message->getFirstHeartBeatInterval() < 0) {
                        vector<string> vetor;
                        vetor.push_back("### intervalo para sonda negativa PNA ");
                        vetor.push_back(OddciUtils::itoa(addr));
                        vetor.push_back("| instancia ");
                        vetor.push_back(OddciUtils::itoa(instanceId));
                        vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
                        OddciUtils::print(vetor);
                    }
                    scheduleAt(simTime() + message->getFirstHeartBeatInterval(), heartBeatTimer);
                    slotTimer->setKind(SLOT_WAITING);
                    cancelEvent(slotTimer);
                    scheduleAt(simTime() + slotInterval, slotTimer);

                    // Executar código
                    //pthread_t thread;

                    //pthread_create(&thread, NULL, executeImage, (void *) this);

//                    thread t(PNA::executeImage, 0);

                }
                break;
            }
            case ODDCI_KILL : {

//                vector<string> vetor;
//                vetor.push_back("### Killing PNA ####");
//                vetor.push_back(OddciUtils::itoa(addr));
//                vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
//                OddciUtils::print(vetor);

                // Para tudo
                cancelAndDelete(checkAvailabilityTimer);
                checkAvailabilityTimer = NULL;
                cancelAndDelete(slotTimer);
                slotTimer = NULL;
                cancelAndDelete(heartBeatTimer);
                heartBeatTimer = NULL;
                cancelAndDelete(liveOrDieTimer);
                state = OFF;
                liveOrDieTimer = NULL;
                onPNAs = 0;
                waitingPNAs = 0;
                busyPNAs = 0;
                idlePNAs = 0;
                finish();
                break;
            }
        }
        delete msg;
    }
}

void * executeImage(void *ptr) {
    FILE *fp;
    char result[1035];

    PNA *pna = (PNA *) ptr;

    /* Open the command for reading. */
    fp = popen("java -jar /home/dienert/omnetpp-4.4.1/samples/Oddci/app.jar", "r");
    if (fp == NULL) {
        EV << "Failed to run command\n";
        strcpy(result, "0");
    }

    /* Read the output a line at a time - output it. */
    while (fgets(result, sizeof(result)-1, fp) != NULL);

    string resultado = "Falha";

    if (strcmp(result, "1\n") == 0) {
        resultado = "Sucesso";
    }

    EV << "PNA address: " << pna->getAddr() << " - " << resultado;


   vector<string> vetor;
   vetor.push_back("### resultado da execução da PNA ");
   vetor.push_back(OddciUtils::itoa(pna->getAddr()));
   vetor.push_back("| instancia ");
   vetor.push_back(OddciUtils::itoa(pna->getInstanceId()));
   vetor.push_back(OddciUtils::getDurationFromSeconds(simTime().dbl()));
   vetor.push_back("| resultado:");
   vetor.push_back(resultado);
   OddciUtils::print(vetor);

}

const char * PNA::stateToStr() {
    switch (state) {
        case OFF:
            return string("OFF").c_str();
            break;
        case BUSY:
            return string("WORKING").c_str();
            break;
        case IDLE:
            return string("IDLE").c_str();
            break;
        case WAITING:
            return string("WAITING").c_str();
            break;
    }
    return string("ERRO").c_str();
}

