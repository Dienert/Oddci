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

#include "SubCycle.h"

SubCycle::SubCycle(int id, Cycle *cycle) {
    this->id = id;
    this->cycle = cycle;
    workingPNAs = 0;
    discardedPNAs = 0;
    allocatedPNAs = 0;
    rankingTarget = 0;
    startedSlots = 0;
    completedSlots = 0;
    factor = 0;
    requestingDevices = 1;
    numberOfWakeUps = 0;
    finished = false;
}

SubCycle::~SubCycle() {
}

double SubCycle::getFactor() const {
    return factor;
}

int SubCycle::getId() const {
    return id;
}

Cycle * SubCycle::getCycle() const {
    return cycle;
}

int SubCycle::getAllocatedPNAs() const {
    return allocatedPNAs;
}

void SubCycle::setId(int id) {
    this->id = id;
}

void SubCycle::setCycle(Cycle *cycle) {
    this->cycle = cycle;
}

void SubCycle::setAllocatedPNAs(int startedPNAs) {
    this->allocatedPNAs = startedPNAs;
}

void SubCycle::setDiscardedPNAs(int discardedPNAs) {
    this->discardedPNAs = discardedPNAs;
}

int SubCycle::getDiscardedPNAs() const {
    return discardedPNAs;
}

void SubCycle::setFactor(const double factor) {
    this->factor = factor;
}

double SubCycle::getRequestingDevices() const {
    return requestingDevices;
}

void SubCycle::setRequestingDevices(const double requestedDevices) {
    this->requestingDevices = requestedDevices;
}

int SubCycle::getRankingTarget() const {
    return rankingTarget;
}

int SubCycle::getWorkingPNAs() const
{
    return workingPNAs;
}

bool SubCycle::isFinished() const
{
    return finished;
}

int SubCycle::getCompletedSlots() const
{
    return completedSlots;
}

int SubCycle::getStartedSlots() const
{
    return startedSlots;
}

int SubCycle::getNumberOfWakeUps() const
{
    return numberOfWakeUps;
}

void SubCycle::setNumberOfWakeUps(int numberOfWakeUps)
{
    this->numberOfWakeUps = numberOfWakeUps;
}

void SubCycle::setStartedSlots(int startedSlots)
{
    this->startedSlots = startedSlots;
}

void SubCycle::setCompletedSlots(int completedSlots)
{
    this->completedSlots = completedSlots;
}

void SubCycle::setFinished(bool finished)
{
    this->finished = finished;
}

void SubCycle::setWorkingPNAs(int workingPnAs)
{
    workingPNAs = workingPnAs;
}

void SubCycle::setRankingTarget(const int currentRanking) {
    this->rankingTarget = currentRanking;
}

SubCycle * SubCycle::getPrevSubCycle() {
    SubCycle * prevSubCycle = NULL;
    if (id > 0) {
        prevSubCycle = cycle->getSubCycles()->at(id-1);        // retorna o subciclo anterior
    } else if (id == 0 && cycle->getId() != 0) {
        Cycle *prevCycle = cycle->getInstance()->getCycles()->at(cycle->getId()-1);
        prevSubCycle = prevCycle->getSubCycles()->at(cycle->getSubCyclesNumber()-1);   // retorna o último subciclo do ciclo anterior
    } else if (id == 0 && cycle->getId() == 0) {
        prevSubCycle = cycle->getSubCycles()->at(id);          // retorna o mesmo subciclo
    }
    return prevSubCycle;
}

SubCycle * SubCycle::getNextSubCycle() {
    SubCycle * nextSubCycle = NULL;
    if (id == cycle->getSubCyclesNumber()-1) {
        Cycle *nextCycle = cycle->getInstance()->getCycles()->at(cycle->getId()+1);
        nextSubCycle = nextCycle->getSubCycles()->at(0);        // retorna o primeiro subciclo do próximo ciclo
    } else {
        nextSubCycle = cycle->getSubCycles()->at(id+1);          // retorna o próximo subciclo dentro do ciclo
    }
    return nextSubCycle;
}

int SubCycle::getFailedDevices() {
    int prevWorkingPNAs = 0;

    if (cycle->getId() != 0 || id != 0) {
        prevWorkingPNAs = getPrevSubCycle()->getWorkingPNAs();
    }

    return  prevWorkingPNAs + allocatedPNAs - workingPNAs;
}

double SubCycle::getObservedVolatility() {
    int prevWorkingPNAs = 0;

    if (cycle->getId() != 0 || id != 0) {
        prevWorkingPNAs = getPrevSubCycle()->getWorkingPNAs();
    }
    if (finished) {
        return  1 - workingPNAs/(double)(prevWorkingPNAs + allocatedPNAs);
    } else {
        return 0;
    }
}

int SubCycle::getDeficitDevices() {
    return cycle->getInstance()->getRequestedDevices()-workingPNAs;
}

