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

#ifndef __PNA_H
#define __PNA_H

#include <omnetpp.h>
#include "HeartBeatMsg_m.h"
#include "WakeupMsg_m.h"
#include "DieMsg_m.h"
#include "WaitingMsg_m.h"
#include "WorkMsg_m.h"
#include "IdleProbeMsg_m.h"
#include <set>
//#include <ext/hash_map>
#include "Controller.h"
#include "MessageType.h"
#include "PNAEstimator.h"
#include <pthread.h>

//using namespace __gnu_cxx;
//using std::size_t;
using namespace std;

class PNA : public cSimpleModule
{
  private:
	enum State {
		IDLE,
		BUSY,
		OFF,
		WAITING,
		FINISHED
	};

	enum Timer {
	    INITIAL_WAITING,
	    SLOT_WAITING,
	    HEARTBEAT,
        CHECK_AVAILABILITY,
	    LIVEORDIE,
	    CANCEL_WAITING,
	    IDLE_PROBE
	};

  private:
	simtime_t lastArrival;
    int addr;
	int controllerAddr;
	State state;
	int activeIntervals;
	int checkInterval;
	simtime_t heartBeatInterval;
	simtime_t slotInterval;
	simtime_t informativeProbeInterval;
	double cycleInterval;
	bool slotCompleted;
	int waitingTimeOut;
	double volatility;
	int instanceId;
//	hash_map<int, int> cycles;
//	hash_map<int, int> subCycles;
	int cycle;
//	int subCycle;
	bool firstLiveOrDie;
	double pnaMaxDelay;

	PNAEstimator::EstimationMethod method;

    // statistics
    cDoubleHistogram iaTimeHistogram;
    cOutVector arrivalsVector;

    static int count;
	static int onPNAs;
	static int busyPNAs;
	static int waitingPNAs;
	static int idlePNAs;
	static int offPNAs;
	static int numDevices;

  protected:
    cMessage *slotTimer;
    cMessage *heartBeatTimer;
    cMessage *checkAvailabilityTimer;
    cMessage *liveOrDieTimer;
    double initialAvailability;
//    double pnaAverageSessionTime;




    virtual void initialize();

    virtual void handleMessage(cMessage *msg);

    virtual void sendMessage();

    virtual void handleTimer(cMessage *msg);

    virtual const char * stateToStr();
    virtual void checkAvailability();
    virtual void cancelWaiting();
    virtual void liveOrDie();

  public:
    static int getOnPNAs() {return onPNAs;}
    static int getOffPNAs() {return numDevices-onPNAs;}
    static int getBusyPNAs() {return busyPNAs;}
    static int getWaitingPNAs() {return waitingPNAs;}
    static int getIdlePNAs() {return idlePNAs;}

    int getAddr() const {
        return addr;
    }

    void setAddr(int addr) {
        this->addr = addr;
    }

    const cMessage* getHeartBeatTimer() const {
        return this->heartBeatTimer;
    }

    void setHeartBeatTimer(cMessage* heartBeatTimer) {
        this->heartBeatTimer = heartBeatTimer;
    }

    int getInstanceId() const {
        return instanceId;
    }

    void setInstanceId(int instanceId) {
        this->instanceId = instanceId;
    }

    const cMessage* getSlotTimer() const {
        return this->slotTimer;
    }

    void setSlotTimer(cMessage* slotTimer) {
        this->slotTimer = slotTimer;
    }

    State getState() const {
        return state;
    }

    void setState(State state) {
        this->state = state;
    }
};

void * executeImage(void *ptr);

#endif
