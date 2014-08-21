/*
 * InformativePNAEstimator.cpp
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#include "InformativePNAEstimator.h"

InformativePNAEstimator::InformativePNAEstimator() :
        PNAEstimator(PNAEstimator::INFORMATIVE) {

}

InformativePNAEstimator::~InformativePNAEstimator() {

}

double InformativePNAEstimator::getNextSubCycleFactor(Cycle *cycle) {
    double factor;
    SubCycle *subCycle = cycle->getCurrentSubCycle();

    long estimatedIdlePNAs = fakeInstance->getCycles()->at(fakeInstance->getCurrentCycle()->getId() - 1)->getStartedSlots();

    factor = (subCycle->getRequestingDevices()) / (float) estimatedIdlePNAs;
    setEstimatedIdlePNAS(estimatedIdlePNAs);
    return factor;
}

Instance *InformativePNAEstimator::getFakeInstance() const {
    return fakeInstance;
}

void InformativePNAEstimator::setFakeInstance(Instance *fakeInstance) {
    this->fakeInstance = fakeInstance;
}


