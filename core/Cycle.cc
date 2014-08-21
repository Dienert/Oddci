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

#include "Cycle.h"

Cycle::Cycle(int id, int evaluatableSubCycles, Instance *instance) {
    this->id = id;
    this->instance = instance;
    this->subCyclesNumber = evaluatableSubCycles;

    subCycles = new vector<SubCycle *>();

    int maxSubCycles = 2 * evaluatableSubCycles - 1;
    for (int i = 0; i < maxSubCycles; i++) {
        SubCycle *subCycle = new SubCycle(i, this);
        subCycles->push_back(subCycle);

    }
    recorded = false;
    currentSubCycleId = 0;
    completedSlots = 0;
    discardedPNAs = 0;
    allocatedPNAs = 0;
    startedSlots = 0;
    numberOfWakeUps = 0;
    currentRanking = 0;
    redundancy = 1;
    factor = 0.1;
}

Cycle::~Cycle() {
    subCycles->clear();
//    subCycles.clear();
}

double Cycle::getFactor() const {
    return factor;
}

int Cycle::getNumberOfWakeUps() const {
    return numberOfWakeUps;
}

void Cycle::setNumberOfWakeUps(int numberOfWakeUps) {
    this->numberOfWakeUps = numberOfWakeUps;
}

int Cycle::getCompletedSlots() const {
    return completedSlots;
}

int Cycle::getId() const {
    return id;
}

Instance * Cycle::getInstance() const {
    return instance;
}

int Cycle::getStartedSlots() const {
    return startedSlots;
}

void Cycle::setCompletedSlots(int completedSlots) {
    this->completedSlots = completedSlots;
}

void Cycle::setId(int id) {
    this->id = id;
}

void Cycle::setInstance(Instance *instance) {
    this->instance = instance;
}

void Cycle::setStartedSlots(int startedSlots) {
    this->startedSlots = startedSlots;
}

void Cycle::setDiscardedPNAs(int discardedPNAs) {
    this->discardedPNAs = discardedPNAs;
}

int Cycle::getDiscardedPNAs() const {
    return discardedPNAs;
}

void Cycle::setFactor(const double factor) {
    this->factor = factor;
}

double Cycle::getRedundancy() const {
    return redundancy;
}

void Cycle::setRedundancy(const double redundancy) {
    this->redundancy = redundancy;
}

int Cycle::getCurrentRanking() const {
    return currentRanking;
}

void Cycle::setCurrentRanking(const int currentRanking) {
    this->currentRanking = currentRanking;
}

int Cycle::getAllocatedPNAs() const {
    return allocatedPNAs;
}

void Cycle::setAllocatedPNAs(int allocatedPnAs) {
    allocatedPNAs = allocatedPnAs;
}

int Cycle::getSubCyclesNumber() const {
    return subCyclesNumber;
}

double Cycle::getDiscard() const {
    double discard = 0;
    if ((discardedPNAs + allocatedPNAs) != 0) {
        discard = discardedPNAs / (double) (discardedPNAs + allocatedPNAs);
    }
    return discard;
}

double Cycle::getParallelism() const {
    return parallelism;
}

void Cycle::setParallelism(double parallelism) {
    this->parallelism = parallelism;
}

double Cycle::getObservedVolatility() const {
    double observedVolatility = 0;
    if (startedSlots != 0) {
        observedVolatility = 1 - completedSlots / (double) startedSlots;
    }
    return observedVolatility;
}

int Cycle::getHeartBeats() const {
    return heartBeats;
}

int Cycle::getWorkingPNAs() const {
    return workingPNAs;
}

void Cycle::setHeartBeats(int heartBeats) {
    this->heartBeats = heartBeats;
}

void Cycle::setWorkingPNAs(int workingPnAs) {
    this->workingPNAs = workingPnAs;
}

double Cycle::getProgress() const {
    return progress;
}

void Cycle::setProgress(double progress) {
    this->progress = progress;
}

void Cycle::setSubCyclesNumber(int subCyclesNumber) {
    this->subCyclesNumber = subCyclesNumber;
}

void Cycle::setSubCycles(vector<SubCycle*> *subCycles) {
    this->subCycles = subCycles;
}

void Cycle::setCurrentSubCycleId(const int currentSubCycle) {
    this->currentSubCycleId = currentSubCycle;
}

vector<SubCycle *> * Cycle::getSubCycles() const {
    return this->subCycles;
}

SubCycle * Cycle::getCurrentSubCycle() {
    return subCycles->at(currentSubCycleId);
}

SubCycle * Cycle::getPrevSubCycle() {
    SubCycle * prevSubCycle = NULL;
    if (currentSubCycleId > 0) {
        prevSubCycle = subCycles->at(currentSubCycleId - 1); // retorna o subciclo anterior
    } else if (currentSubCycleId == 0 && id != 0) {
        Cycle *prevCycle = this->instance->getCycles()->at(id - 1);
        prevSubCycle = prevCycle->getSubCycles()->at(subCyclesNumber - 1); // retorna o último subciclo do ciclo anterior
    } else if (currentSubCycleId == 0 && id == 0) {
        prevSubCycle = subCycles->at(currentSubCycleId); // retorna o mesmo subciclo
    }
    return prevSubCycle;
}

//SubCycle * Cycle::getPrevSubCycle() {
//    SubCycle * prevSubCycle = NULL;
//    if (currentSubCycleId > 1) {
//        prevSubCycle = subCycles->at(currentSubCycleId-2);        // retorna o antepenúltimo subciclo
//    } else if (currentSubCycleId == 1 && id != 0) {
//        Cycle *prevCycle = this->instance->getCycles()->at(id-1);
//        prevSubCycle = prevCycle->getSubCycles()->at(evaluatableSubCycles-1);   // retorna o último subciclo do ciclo anterior
//    } else if (currentSubCycleId == 0 && id != 0) {
//        Cycle *prevCycle = this->instance->getCycles()->at(id-1);
//        prevSubCycle = prevCycle->getSubCycles()->at(evaluatableSubCycles-2);   // retorna o penúltimo subciclo do ciclo anterior
//    } else if (currentSubCycleId == 1 && id == 0) {
//        prevSubCycle = subCycles->at(0);        // retorna o subciclo 0
//    } else if (currentSubCycleId == 0 && id == 0) {
//        prevSubCycle = subCycles->at(currentSubCycleId);          // retorna o mesmo subciclo
//    }
//    return prevSubCycle;
//}

double Cycle::getVolatilityFactor() {
    double volatilityFactor = 0;
    // Começando o subciclo 0.0 o fator de volatilidade é 0
    if (currentSubCycleId == 0 && id == 0) {
        volatilityFactor = 0;
    // Começando o subciclo 0.1 e calculando a volatilidade do subciclo 0.0
    } else if (currentSubCycleId == 1 && id == 0) {
        SubCycle *prevSubCycle = getPrevSubCycle();
        volatilityFactor = prevSubCycle->getAllocatedPNAs() / (double) (prevSubCycle->getAllocatedPNAs()-prevSubCycle->getWorkingPNAs());
    } else if ((currentSubCycleId > 1 && id == 0) || id != 0) {
        SubCycle *prevSubCycle = getPrevSubCycle();
        SubCycle *prevPrevSubCycle = prevSubCycle->getPrevSubCycle();
        volatilityFactor = (prevSubCycle->getAllocatedPNAs() + prevPrevSubCycle->getWorkingPNAs()) / (double) prevSubCycle->getFailedDevices();

    }
    return volatilityFactor;
}

double Cycle::getVolatilityRate() {
    double volatilityRate = 0;
    // Começando o subciclo 0.0 ou 0.1 a taxa de volatilidade é 0
    if ((currentSubCycleId == 0 || currentSubCycleId == 1) && id == 0) {
        volatilityRate = 0;
    } else if ((currentSubCycleId > 1 && id == 0) || id != 0) {
        SubCycle *prevSubCycle = getPrevSubCycle();
        SubCycle *prevPrevSubCycle = prevSubCycle->getPrevSubCycle();
        volatilityRate = prevPrevSubCycle->getFailedDevices() / (double) prevSubCycle->getFailedDevices();

    }
    return volatilityRate;
}

bool Cycle::isRecorded() const
{
    return recorded;
}

void Cycle::setRecorded(bool recorded)
{
    this->recorded = recorded;
}

Cycle * Cycle::getPrevCycle() {
    Cycle * prevCycle = NULL;
    if (id > 0) {
        prevCycle = instance->getCycles()->at(id-1);        // retorna o ciclo anterior
    } else if (id == 0) {
        prevCycle = instance->getCycles()->at(id);          // retorna o mesmo subciclo
    }
    return prevCycle;
}
