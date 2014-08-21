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

#ifndef CYCLE_H_
#define CYCLE_H_

#include <omnetpp.h>
#include "SubCycle.h"
#include <vector>
#include "Instance.h"

using namespace std;

class Instance;
class SubCycle;
class Cycle {

private:
	int id;
	Instance *instance;
	int startedSlots;
	double parallelism;
	int completedSlots;
	double progress;
	int discardedPNAs;
	int allocatedPNAs;
	int numberOfWakeUps;
	int heartBeats;
    int workingPNAs;
    double factor;
    double redundancy;
    int currentRanking;
    int currentSubCycleId;
    int subCyclesNumber;
    vector<SubCycle*> *subCycles;
    bool recorded;
public:
    Cycle();
    Cycle(int id, int evaluatableSubCycles, Instance *instance);
    virtual ~Cycle();
    virtual double getFactor() const;
    int getNumberOfWakeUps() const;
    void setNumberOfWakeUps(int numberOfWakeUps);
    int getCompletedSlots() const;
    int getId() const;
    Instance *getInstance() const;
    int getStartedSlots() const;
    int getDiscardedPNAs() const;
    virtual int getCurrentRanking() const;
    virtual vector<SubCycle*> *getSubCycles() const;
    SubCycle *getPrevSubCycle();
    SubCycle *getCurrentSubCycle();
    double getVolatilityFactor();
    double getVolatilityRate();
    void setCompletedSlots(int completedSlots);
    void setId(int id);
    void setInstance(Instance *instance);
    void setStartedSlots(int startedSlots);
    void setDiscardedPNAs(int discardedPNAs);
    virtual void setFactor(const double factor);
    virtual double getRedundancy() const;
    virtual void setRedundancy(const double redundancy);
    virtual void setCurrentRanking(const int currentRanking);
    virtual void setCurrentSubCycleId(const int currentSubCycle);
    int getAllocatedPNAs() const;
    void setAllocatedPNAs(int allocatedPnAs);
    void setSubCycles(vector<SubCycle*> *subCycles);
    int getSubCyclesNumber() const;
    void setSubCyclesNumber(int evaluatableSubCycles);
    double getDiscard() const;
    double getParallelism() const;
    void setParallelism(double);
    double getObservedVolatility() const;
    double getProgress() const;
    void setProgress(double progress);
    int getHeartBeats() const;
    int getWorkingPNAs() const;
    void setHeartBeats(int heartBeats);
    void setWorkingPNAs(int workingPnAs);
    Cycle *getPrevCycle();
    bool isRecorded() const;
    void setRecorded(bool recorded);
};

#endif /* CYCLE_H_ */
