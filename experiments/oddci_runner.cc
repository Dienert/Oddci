//
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

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include "rapidxml.hpp"
#include "Experiment.h"
#include "ExpInstance.h"
#include <pthread.h>
#include "OddciUtils.h"
#include <getopt.h>
#include <omp.h>

#define PROGRAM_NAME "oddci_runner"

#define AUTHORS "Diénert Vieira"

using namespace rapidxml;

enum Heuristic {
    REACTIVE, PROACTIVE
};

enum Method {
    INFORMATIVE, EXPLORATORY
};

enum Selection {
    TEMPESTIVE, RANKING
};

static struct option const long_options[] = {
        { "combinations", required_argument, 0, 'c' },
        { "experiments", required_argument, 0, 'e' },
        { "generate", no_argument, 0, 'g' },
        { "repetition", required_argument, 0, 'r' },
        { "threads", required_argument, 0, 't' },
//        { "author", no_argument, 0, AUTHOR_OPTION },
        { "help", no_argument, 0, 'h'},
        { "version", no_argument, 0, 'v' },
        { NULL, 0, NULL, 0 }
};

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv;
int startedThreads = 0;
int activeThreads = 0;
int coresNumber;
int totalThreads = 0;
int experimentsNumber = 0;
int completedThreads = 0;

double availability = 0.2;
Heuristic activeHeuristic;
Method activePNAsEstimationMethod;
Selection selection;
string dir = "results/";
string fileName;
ofstream cyclesfile;

int numArg;
vector<int> combinationsVector;
string experiments;
int repetition;
bool generateIniFilesOnly;
char estMethodName[13];
char heuristicName[13];
char selectionName[13];

/* The name the program was run with. */
char *program_name;

string getName(Heuristic heuristic, Method estMethod, Selection selection) {
    if (heuristic == PROACTIVE) {
        strcpy(heuristicName, string("Proactive").c_str());
    } else {
        strcpy(heuristicName, string("Reactive").c_str());
    }
    if (estMethod == INFORMATIVE) {
        strcpy(estMethodName, string("Informative").c_str());
    } else {
        strcpy(estMethodName, string("Exploratory").c_str());
    }
    if (selection == TEMPESTIVE) {
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

char * createInitializationFile(Experiment *experiment) {

//    cout << "vou iniciar a copia do arquivo geral de inicializacao" << endl;
    ifstream generalInit("general.ini", std::ios::binary);
    char *specificIni = new char[200];
    sprintf(specificIni, "%03d-%s-%s-%s-%0.2f-%0.2f%s", experiment->getId(),
            heuristicName, estMethodName, selectionName,
            atof(experiment->getInitialAvailability()),
            atof(experiment->getVolatility()), ".ini");
    ofstream specificInitBinary(specificIni, std::ios::binary);
    specificInitBinary << generalInit.rdbuf();
    specificInitBinary.close();

    ofstream specificInit(specificIni, std::ios::app);

    specificInit << endl;
    specificInit << "repeat = " << experiment->getRepetitions() << endl;
    specificInit << endl;
    specificInit << "BroadcastNetwork.experimentId = " << experiment->getId()
            << endl;
    specificInit << "BroadcastNetwork.devicesNumber = "
            << experiment->getDevicesNumber() << endl;
    specificInit << "BroadcastNetwork.cycleInterval = \""
            << experiment->getCycleInterval() << "\"" << endl;
    specificInit << "BroadcastNetwork.subCycleInterval = \""
            << experiment->getSubcycleInterval() << "\"" << endl;
    specificInit << "BroadcastNetwork.initialAvailability = "
            << experiment->getInitialAvailability() << endl;
    specificInit << "BroadcastNetwork.volatility = "
            << experiment->getVolatility() << endl;
    specificInit << "BroadcastNetwork.activeHeuristic = \"" << heuristicName
            << "\"" << endl;
    specificInit << "BroadcastNetwork.activePNAsEstimationMethod = \""
            << estMethodName << "\"" << endl;
    specificInit << "BroadcastNetwork.selection = \"" << selectionName << "\""
            << endl;
    specificInit << "BroadcastNetwork.imageSize = \""
            << experiment->getImageSize() << "\"" << endl;
//    specificInit << "BroadcastNetwork.pnaAverageSessionTime = " << experiment->getPnaAverageSessionTime() << endl;
    specificInit << "BroadcastNetwork.pnaMaxDelay = "
            << experiment->getPnaMaxDelay() << endl;
    specificInit << "BroadcastNetwork.workload = \""
            << experiment->getWorkload() << "\"" << endl;
    specificInit << "BroadcastNetwork.broadcastBandwidth = "
            << experiment->getBroadcastBandwidth() << endl;
    specificInit << "#BroadcastNetwork.logControllerReceivingMsg = true"
            << endl;
    specificInit << "**.pnaGroup*.g.channel.datarate = "
            << experiment->getReturnChannelBandwidth() << " #direct channel"
            << endl;

    specificInit.close();

    return specificIni;

}

void * execExperiment(void *ptr) {
    char msg[500] = "Oddci -u Cmdenv -f ";
    char *initializationFile = (char *) ptr;
    cout << "Initialization cyclesfile: " << initializationFile << endl;
    char outPutFile[200] = " > results/out/";
    strcat(outPutFile, initializationFile);
    strcat(outPutFile, ".out");
    cout << "Outputfile " << outPutFile << endl;
    strcat(msg, initializationFile);
    strcat(msg, " -r ");
    strcat(msg, OddciUtils::itoa(repetition).c_str());

    strcat(msg, outPutFile);
    cout << msg << endl;
    time_t start, end;

    bool failed = false;
    time(&start);
//    sleep(5);
    if (!generateIniFilesOnly) {
        try {
            system(msg);
        } catch (...) {
            failed = true;
        }
    }
    time(&end);

    if (!failed) {
        cout << "\nFinishing thread " << initializationFile << " ";
    } else {
        cout << "\nFailed thread " << initializationFile << " ";
    }
    double diff = difftime(end, start);
    cout << " in " << OddciUtils::getDurationFromSeconds(diff) << endl;

    pthread_mutex_lock(&mutex);
    activeThreads--;
    completedThreads++;
    pthread_cond_signal(&cv); //notifying the main thread
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void createResultsFiles(int experimentId) {
    cout << "Criando arquivo de registros de ciclos" << endl;
    fileName = getName(activeHeuristic, activePNAsEstimationMethod, selection);
    string cyclesPath = dir + "cycles-" + fileName + ".txt";
    printf("%s\n", cyclesPath.c_str());
    cyclesfile.open(cyclesPath.c_str());
    cyclesfile.close();
}

//void handleAvailability(Experiment *experiment) {
//	do {
//		execExperiment(experiment);
//		availability += 0.2;
//	} while(availability <= 0.8);
//	availability = 0.2;
//}

ExpInstance * handleInstance(xml_node<> *instanceNode) {
    ExpInstance *instance = new ExpInstance();
    instance->setId(instanceNode->first_attribute("id")->value());
    instance->setImageSize(instanceNode->first_attribute("imageSize")->value());
    instance->setStartTime(instanceNode->first_attribute("startTime")->value());
    instance->setRequestedSlots(
            instanceNode->first_attribute("requestedSlots")->value());
    instance->setRequestedDevices(
            instanceNode->first_attribute("requestedDevices")->value());
    return instance;
}

void iterateInstance(Experiment *experiment, xml_node<> *instanceNode) {
    ExpInstance *instance = handleInstance(instanceNode);
    experiment->getInstances()->push_back(instance);
    cout << "terminei a instancia " << instance->getId() << endl;
    xml_node<> *nextInstance = instanceNode->next_sibling();
    if (nextInstance) {
        iterateInstance(experiment, nextInstance);
    }
}

void handleExperiment(xml_node<> *experimentNode) {

    Experiment *experiment = new Experiment();
    experiment->setId(atoi(experimentNode->first_attribute("id")->value()));
    experiment->setDevicesNumber(
            atoi(experimentNode->first_attribute("devicesNumber")->value()));
    experiment->setCycleInterval(
            experimentNode->first_attribute("cycleInterval")->value());
    experiment->setSubcycleInterval(
            experimentNode->first_attribute("subcycleInterval")->value());
    experiment->setRepetitions(
            atoi(experimentNode->first_attribute("repetitions")->value()));
    experiment->setAvailabilityCheckInterval(
            experimentNode->first_attribute("availabilityCheckInterval")->value());
    experiment->setImageSize(
            experimentNode->first_attribute("imageSize")->value());
//    experiment->setPnaAverageSessionTime(atof(experimentNode->first_attribute("pnaAverageSessionTime")->value()));
    experiment->setBroadcastBandwidth(
            experimentNode->first_attribute("broadcastBandwidth")->value());
    experiment->setReturnChannelBandwidth(
            experimentNode->first_attribute("returnChannelBandwidth")->value());
    experiment->setPnaMaxDelay(
            experimentNode->first_attribute("pnaMaxDelay")->value());
    experiment->setWorkload(
            experimentNode->first_attribute("workload")->value());
    experiment->setInitialAvailability(
            experimentNode->first_attribute("initialAvailability")->value());
    experiment->setVolatility(
            experimentNode->first_attribute("volatility")->value());

//    xml_node<> *instance = experimentNode->first_node();
//    iterateInstance(experiment, instance);

    printf("\n####%s %03d####\n", "Experiment:", experiment->getId());

    activeHeuristic = REACTIVE;
    activePNAsEstimationMethod = INFORMATIVE;
    selection = TEMPESTIVE;

    for (vector<int>::iterator it = combinationsVector.begin(); it != combinationsVector.end(); it++) {
        switch (*it) {
        case 1:
            activeHeuristic = REACTIVE;
            activePNAsEstimationMethod = INFORMATIVE;
            selection = TEMPESTIVE;
            break;
        case 2:
            activeHeuristic = REACTIVE;
            activePNAsEstimationMethod = INFORMATIVE;
            selection = RANKING;
            break;
        case 3:
            activeHeuristic = REACTIVE;
            activePNAsEstimationMethod = EXPLORATORY;
            selection = TEMPESTIVE;
            break;
        case 4:
            activeHeuristic = REACTIVE;
            activePNAsEstimationMethod = EXPLORATORY;
            selection = RANKING;
            break;
        case 5:
            activeHeuristic = PROACTIVE;
            activePNAsEstimationMethod = INFORMATIVE;
            selection = TEMPESTIVE;
            break;
        case 6:
            activeHeuristic = PROACTIVE;
            activePNAsEstimationMethod = INFORMATIVE;
            selection = RANKING;
            break;
        case 7:
            activeHeuristic = PROACTIVE;
            activePNAsEstimationMethod = EXPLORATORY;
            selection = TEMPESTIVE;
            break;
        case 8:
            activeHeuristic = PROACTIVE;
            activePNAsEstimationMethod = EXPLORATORY;
            selection = RANKING;
            break;
        }
        const char *combinationName = getName(activeHeuristic,
                activePNAsEstimationMethod, selection).c_str();
        printf("\n%s %s\n", "Combination:", combinationName);
//        createResultsFiles(experiment->getId());
        char *initializationFile = createInitializationFile(experiment);

        pthread_mutex_lock(&mutex);
        if (activeThreads < coresNumber) {
            pthread_t thread;
            startedThreads++;
            cout << "\nStarting thread " << startedThreads << "/"
                    << totalThreads << endl;
//            int iret = pthread_create(&thread, NULL, execExperiment, (void *) initializationFile);
            pthread_create(&thread, NULL, execExperiment,
                    (void *) initializationFile);
            activeThreads++;
        }
        while (activeThreads == coresNumber && startedThreads < totalThreads) { //numero de threads ativas chegou ao máximo
            cout << "\nWaiting one thread finalize to initialize another"
                    << endl;
            pthread_cond_wait(&cv, &mutex);
            cout << "New slot free, ative threads: " << activeThreads << endl;
        }
        pthread_mutex_unlock(&mutex);

    }
}

void iterateExperiment(xml_node<> *experimentNode) {
    handleExperiment(experimentNode);
    xml_node<> *nextExperiment = experimentNode->next_sibling();
    if (nextExperiment) {
        iterateExperiment(nextExperiment);
    }
}

void usage(int status) {

    printf(
            ("\
Usage: %s [options]\n\
  -c, --combinations=COMBINATIONS   ids of heuristics combination separeted by comma\n\
                                    default is all the existent in the following order\n\
                                       IDS:\n\
                                       1: reactive informative tempestive\n\
                                       2: reactive informative ranking\n\
                                       3: reactive exploratory tempestive\n\
                                       4: reactive exploratory ranking\n\
                                       5: proactive informative tempestive\n\
                                       6: proactive informative ranking\n\
                                       7: proactive exploratory tempestive\n\
                                       8: proactive exploratory ranking\n\
  -e, --experiments=XML             path to the xml file describing the experiments\n\
                                      default is the relative  path \"experiments/validation-experiments.xml\"\n\
  -g, --generate                    only generate the .ini file without executing the experiments\n\
  -r, --repetition=REPETITION       number of the repetition to run\n\
                                      default is 0\n\
  -t, --threads=THREADS             number of threads used to process the exsperiments\n\
                                      default is the number of existent core processors\n\
Report problems for: Diénert Vieira <dienertalencar@gmail.com>\n\
"),
            program_name);
    exit(status);
}

int main2(int argc, char **argv) {

    numArg = argc;

    program_name = argv[0];
    //setting the default arguments
    string combinations = "1,2,3,4,5,6,7,8";
    experiments = "experiments/validation-experiments.xml";
    repetition = 0;
    generateIniFilesOnly = false;
    coresNumber = sysconf( _SC_NPROCESSORS_ONLN );

    int optc;


    while ((optc = getopt_long(argc, argv, "c:e:gr:t:", long_options, NULL)) != -1) {
        switch (optc) {
        case 0:
            break;
        case 'c':
            combinations = optarg;
            break;
        case 'e':
            experiments = optarg;
            break;
        case 'g':
            generateIniFilesOnly = true;
            break;
        case 'r':
            repetition = atoi(optarg);
            break;
        case 't':
            coresNumber = atoi(optarg);
            break;
        case 'h':
        case 'v':
        default:
            usage(EXIT_FAILURE);
        }
    }

    xml_document<> oddci;
    ifstream t(experiments.c_str()); // arquivo xml contendo a descrição dos experimentos

    stringstream ss(combinations);

    int i;

    while (ss >> i) {
        combinationsVector.push_back(i);

        if (ss.peek() == ',') {
            ss.ignore();
        }
    }

    printf ("\n\
*********************************************\n\
Starting execution with parameters:\n\
    combinations: %s\n\
    experiments: %s\n\
    %s\n\
    repetition: %d\n\
    number of cores to use: %d\n\
********************************************\n"
        , combinations.c_str()
        , experiments.c_str()
        , (generateIniFilesOnly ? "generating .ini files only, no execution of the experiments" : "generating .ini files and executing experiments")
        , repetition
        , coresNumber
        );

    // transformando o xml em string
    string str;
    t.seekg(0, std::ios::end);
    str.reserve((unsigned long) t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    char *xml = const_cast<char*>(str.c_str());

    //fazendo o parsing da string
    oddci.parse<0>(xml);

    xml_node<> *experiment = oddci.first_node("oddci")->first_node("experiment");
    experimentsNumber++;
    xml_node<> *brother = experiment->next_sibling();
    if (brother) {
        while (brother->next_sibling()) {
            brother = brother->next_sibling();
            experimentsNumber++;
        }
        experimentsNumber++;
    }

    totalThreads = experimentsNumber * combinationsVector.size();
    time_t start, end;
    time(&start);
    iterateExperiment(experiment);

    pthread_mutex_lock(&mutex);
    while (completedThreads < totalThreads) {
        pthread_cond_wait(&cv, &mutex);
    }
    time(&end);

    cout << "\nAll threads finished in ";
    double diff = difftime(end, start);
    cout << OddciUtils::getDurationFromSeconds(diff) << endl;
    pthread_mutex_unlock(&mutex);

    return 0;
}
