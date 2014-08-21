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

#ifndef SUBCYCLE_H_
#define SUBCYCLE_H_

#include "Cycle.h"

class Cycle;
class SubCycle {

private:
	int id;
	Cycle *cycle;
	int allocatedPNAs;
	int workingPNAs;
	int discardedPNAs;
	int startedSlots;
    int completedSlots;
    double factor;
    double requestingDevices;
    int rankingTarget;
    bool finished;
    int numberOfWakeUps;
public:
    SubCycle();
    SubCycle(int id, Cycle *cycle);
    virtual ~SubCycle();
    virtual double getFactor() const;
    int getId() const;
    Cycle *getCycle() const;
    int getAllocatedPNAs() const;
    virtual int getRankingTarget() const;
    SubCycle *getPrevSubCycle();
    SubCycle *getNextSubCycle();
    int getFailedDevices();
    int getDeficitDevices();
    double getObservedVolatility();
    void setId(int id);
    void setCycle(Cycle *cycle);
    void setAllocatedPNAs(int startedPNAs);
    void setDiscardedPNAs(int discardedPNAs);
    int getDiscardedPNAs() const;
    virtual void setFactor(const double factor);
    virtual double getRequestingDevices() const;
    virtual void setRequestingDevices(const double redundancy);
    virtual void setRankingTarget(const int currentRanking);
    int getWorkingPNAs() const;
    void setWorkingPNAs(int workingPnAs);
    bool isFinished() const;
    void setFinished(bool finished);
    int getCompletedSlots() const;
    void setCompletedSlots(int completedSlots);
    int getStartedSlots() const;
    void setStartedSlots(int startedSlots);
    int getNumberOfWakeUps() const;
    void setNumberOfWakeUps(int numberOfWakeUps);
};

#endif /* SUBCYCLE_H_ */
