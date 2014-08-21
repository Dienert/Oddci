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

#ifndef INSTANCE_H
#define INSTANCE_H

#include <omnetpp.h>
#include "cnamedobject.h"
#include "cobject.h"
#include <string.h>
#include <simtime.h>
#include <vector>
#include "Cycle.h"

using namespace std;

class Cycle;
class Instance : public cNamedObject {

  private:
	int id;
	SimTime timeToStart;
	simtime_t nextEvaluationTime;
	int requestedSlots;
	int completedSlots;
	int requestedDevices;
	int maxRanking;
	int imageSize;
	cMessage *timerEvaluation;
	cMessage *timerRecordCycle;
	cMessage *timerChangeCycle;
	cMessage *timerChangeSubCycle;
	cMessage *timerBroadcastSchedule;
	long currentCycle;
	bool fakeCyle;
	vector <Cycle *>* cycles;
//	map <int, Cycle *>* cycles;
	int completedWithZeroCount;
	int cycleToRecord;
	bool finished;

  public:
	Instance(int id, int maxSubCycles, const char *name=NULL);
	virtual ~Instance();

	virtual int getId() const;
    const virtual SimTime getTimeToStart() const;
    virtual int getRequestedSlots() const;
    virtual int getRequestedDevices() const;
    virtual int getMaxRanking() const;
    virtual Cycle * getCurrentCycle() const;
    virtual int getCompletedSlots() const;
    virtual int getCompletedWithZeroCount() const;
    virtual int getCycleToRecord() const;
    virtual bool isFinished() const;
    virtual cMessage *getTimerEvaluation() const;
    virtual cMessage *getTimerRecordCycle() const;
    virtual cMessage *getTimerChangeCycle() const;
    virtual cMessage *getTimerChangeSubCycle() const;
    virtual vector<Cycle*> *getCycles();
    virtual void setId(const int id);
    virtual void setTimeToStart(const SimTime timeToStart);
    virtual void setRequestedSlots(const int duration);
    virtual void setRequestedDevices(const int requestedDevices);
    virtual void setMaxRanking(const int maxRanking);
    virtual void setCurrentCycle(const long  cycle);
    virtual void setCompletedSlots(const int completedSlots);
    virtual void setCompletedWithZeroCount(const int completedWithZeroCount);
    virtual void setCycleToRecord(const int cycleToRecord);
    virtual void setFinished(bool finished);
    virtual void setTimerRecordCycle(cMessage *timerRecordCycle);
    virtual void setTimerChangeCycle(cMessage *timerAddCycle);
    virtual void setTimerChangeSubCycle(cMessage *timerAddCycle);
    virtual void setTimerEvaluation(cMessage *timerMessage);
    virtual void setCycles(vector<Cycle*> *cycles);
    int getImageSize() const;
    bool isFakeCyle() const;
    void setFakeCyle(bool fakeCyle);
    void setImageSize(int imageSize);
    void setReqSlots(int reqSlots);
    simtime_t getNextEvaluationTime() const;
    void setNextEvaluationTime(simtime_t nextEvaluationTime);
    cMessage *getTimerBroadcastSchedule() const;
    void setTimerBroadcastSchedule(cMessage *timerBroadcastSchedule);

};

#endif
