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

#ifndef __ODDCI_CONTROLLER_H
#define __ODDCI_CONTROLLER_H

#include <omnetpp.h>

#include "HeartBeatMsg_m.h"
#include "WakeupMsg_m.h"
#include "DieMsg_m.h"
#include "WaitingMsg_m.h"
#include "WorkMsg_m.h"
#include "KillMsg_m.h"
#include "MessageType.h"

#include "Cycle.h"
#include "Instance.h"

#include "OddciUtils.h"

#include "Provisioner.h"
//#include "ReactiveProvisioner.h" // não incluir que dá pau
//#include "ProactiveProvisioner.h" // não incluir que dá pau

#include "PNAEstimator.h"
#include "ExploratoryPNAEstimator.h"
#include "InformativePNAEstimator.h"

#include "PNASelector.h"
#include "TempestiveSelector.h"
#include "RankingSelector.h"

//#include <ext/hash_map>
#include <iostream>
#include <fstream>

using namespace std;
using namespace __gnu_cxx;
using std::size_t;

//namespace __gnu_cxx {
//template<> struct hash<std::string> {
//        size_t operator()(const std::string& x) const {
//            return hash<const char*>()(x.c_str());
//        }
//};
//}
class Provisioner;
class Controller : public cSimpleModule {
private:
    enum Timer {
        EVALUATION = 0,
        RECORD_CYCLE = 1,
        CHANGE_CYCLE = 2,
        CHANGE_SUBCYCLE = 3,
        SEND_BROADCAST = 4,
        SCHEDULE_WAKEUP = 5
    };

public:
    Provisioner *provisioner;
    PNAEstimator *pnaEstimator;
    PNASelector *pnaSelector;

    int totalDevNum;
    int evaluatableSubCycles;
    simtime_t finishSubCycleShift;
    simtime_t finishCycleShift;
    simtime_t cycleInterval;
    simtime_t subCycleInterval;
    simtime_t informativeProbeInterval;
    simtime_t lastWakeupTransmissionFinish;
    time_t start,currentTime;

    cMessage *sendBroadcastTimer;

    int currentInstance;
    vector<Instance *> instances;
    int instancesNumber;
    int finishedInstances;
    int experimentCount;
    int scheduledWakeups;

    int address;
    int datarate;
    int transmitterAddr;
    bool receivingMsgLog;
    bool wakeupLog;

    double *volatilityFactor;

    string resultsDir;
    string workloadsDir;
    string resultsFileName;
    string imageSize;

    bool isFirstInstance;

    char estMethodName[20];
    char heuristicName[20];
    char selectionName[20];

    int portSend;

    bool earlySend; // if true, don't wait with sendRequest() until established()
    int numRequestsToSend; // requests to send in this session

    int repetitions;
    int experimentId;
    string experimentIdFormated;
    int currentRepetition;

    double initialAvailability;
    double volatility;

    ofstream cycleResultsFile;
    ofstream subCycleResultsFile;

public:
    Controller();
    virtual ~Controller();

protected:
    /**
     * Initialization. Should be redefined to perform or schedule a connect().
     */
    virtual void initialize();

    /**
     * Initialization. Should be redefined to perform or schedule a connect().
     */
    virtual void initExperiment() throw ();

    /**
     * For self-messages it invokes handleTimer(); messages arriving from TCP
     * will get dispatched to the socketXXX() functions.
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * Records basic statistics: numSessions, packetsSent, packetsRcvd,
     * bytesSent, bytesRcvd. Redefine to record different or more statistics
     * at the end of the simulation.
     */
    virtual void finish();

    /** When running under GUI, it displays the given string next to the icon */
    virtual void setStatusString(const char *s);
    //@}

    /** Invoked from handleMessage(). Should be redefined to handle self-messages. */
    virtual void handleTimer(cMessage *msg) throw ();

    virtual string getName(Provisioner *provisioner, PNAEstimator *pnaEstimator, PNASelector *selector);

    virtual void finishInstance(Instance *instance) throw ();

    virtual void evaluateInstance() throw ();

    void startInstance(Instance *instance);

    void scheduleNextEvaluation(simtime_t nextEvaluationTime, Instance *instance);

    virtual void recordCycle(Cycle *cycle) throw ();
    virtual void recordSubCycle(SubCycle *subCycle) throw ();
    void recordSample(SubCycle *subCycle, string sampleId, string value) throw();


    cMessage * getNewSendBroadcastMessageTimer();
    double generateRNG();

public:
    void sendWakeUp(WakeupMsg *msg);
    void scheduleWakeUp(WakeupMsg *msg, simtime_t startTime);

};

#endif
