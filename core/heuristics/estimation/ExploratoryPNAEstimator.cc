/*
 * ExploratoryPNAEstimator.cpp
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#include "ExploratoryPNAEstimator.h"

ExploratoryPNAEstimator::~ExploratoryPNAEstimator() {

}

double ExploratoryPNAEstimator::getFactorIncrement() const
{
    return factorIncrement;
}

double ExploratoryPNAEstimator::getInitialFactor() const
{
    return initialFactor;
}

void ExploratoryPNAEstimator::setFactorIncrement(double factorIncrement)
{
    this->factorIncrement = factorIncrement;
}

void ExploratoryPNAEstimator::setInitialFactor(double initialFactor)
{
    this->initialFactor = initialFactor;
}

double ExploratoryPNAEstimator::getNextSubCycleFactor(Cycle *cycle) {

    double factor = initialFactor;
    Instance *instance = cycle->getInstance();
    SubCycle *subCycle= cycle->getCurrentSubCycle();
    SubCycle *prevSubCycle= cycle->getPrevSubCycle();

    int respondingPNAs = prevSubCycle->getAllocatedPNAs() + prevSubCycle->getDiscardedPNAs();

    if (respondingPNAs == 0 && prevSubCycle->getFactor() < 1) {
        factor += prevSubCycle->getFactor() + factorIncrement;
    } else {
        int estimatedActivePNAs = 0;
        if (prevSubCycle->getFactor() == 0) {
             estimatedActivePNAs = respondingPNAs;
         } else {
             estimatedActivePNAs = respondingPNAs / prevSubCycle->getFactor();
         }

        //usando a redundancia que acabou se ser configurada
        factor = (instance->getRequestedDevices() * subCycle->getRequestingDevices() - prevSubCycle->getWorkingPNAs())
                       / (float) estimatedActivePNAs;

        if (factor > 1) {
           factor = 1;
        } else if (factor < 0) {
           factor = 0;
        }

    }
    return factor;
}
