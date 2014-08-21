/*
 * ReactiveProvisioner.cc
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#include "ReactiveProvisioner.h"

ReactiveProvisioner::ReactiveProvisioner(PNASelector* pnaSelector, PNAEstimator *pnaEstimator)
        : Provisioner(Provisioner::REACTIVE, pnaSelector, pnaEstimator) {

}

ReactiveProvisioner::~ReactiveProvisioner() {
}

//int ReactiveProvisioner::getNextRequestingDevices(Cycle *cycle, double volatilityFactor) {
//    SubCycle *prevSubCycle = cycle->getPrevSubCycle();
//
//    if (cycle->getId() == 0 && cycle->getCurrentSubCycle()->getId() == 0 && cycle->getNumberOfWakeUps() == 0) {
//        return cycle->getInstance()->getRequestedDevices();
//    } else {
//        return prevSubCycle->getFailedDevices();
//    }
//
//}

int ReactiveProvisioner::getNextRequestingDevices(Cycle *cycle, double *volatilityFactor) {

    SubCycle *prevSubCycle = cycle->getPrevSubCycle();
    return prevSubCycle->getDeficitDevices();

}

//int ReactiveProvisioner::getNextRequestingDevices(Cycle *cycle, double volatilityFactor) {
//
//    SubCycle *prevSubCycle = cycle->getPrevSubCycle();
//
//    double subCycleVolatilityFactor = cycle->getVolatilityFactor();
//
//    if (cycle->getId() == 0 && cycle->getCurrentSubCycle()->getId() == 0) {
//        // se for o subciclo 0.0 não tem volatilidade, mas dá pra pegar de outras instancias
//        subCycleVolatilityFactor = volatilityFactor;
//        if (subCycleVolatilityFactor > 1) {
//            return subCycleVolatilityFactor * cycle->getInstance()->getRequestedDevices();
//        } else {
//            return cycle->getInstance()->getRequestedDevices();
//        }
//    }
//
//    volatilityFactor = subCycleVolatilityFactor;
//
//    if (prevSubCycle->getFailedDevices() > 0) {
//        if (volatilityFactor > 1) {
//            return volatilityFactor * prevSubCycle->getFailedDevices();
//        } else {
//            return prevSubCycle->getFailedDevices();
//        }
//    } else {
//        return 0;
//    }
//
//}

simtime_t ReactiveProvisioner::getNextEvaluationTime(simtime_t wakeupArriveTime, Cycle *cycle) {
    if (cycle->getId() == 0 && cycle->getCurrentSubCycle()->getId() == 0) {
        if (getPnaEstimator()->getMethod() == PNAEstimator::EXPLORATORY) { // se for exploratorio, manda um wakeup rápido
            return wakeupArriveTime + 60;
        }
        else {
            // deslocamento de 5 min para a avaliação da instância
            // nos próximos ciclos e subciclos não há deslocamento
            return wakeupArriveTime + getSubCycleInterval() + getEvaluationShift();
        }
    }
    else {
        return wakeupArriveTime + getSubCycleInterval();
    }
}

bool ReactiveProvisioner::isSendWakeup(Cycle *cycle) {
    bool sendWakeup = false;
    SubCycle *prevSubCycle = cycle->getPrevSubCycle();

    if (prevSubCycle->getWorkingPNAs() < cycle->getInstance()->getRequestedDevices()) {
       sendWakeup = true;
    }
    return sendWakeup;
}

void ReactiveProvisioner::posEvaluation(WakeupMsg *message, SubCycle *subCycle, simtime_t endWakeupTransmission, simtime_t subCycleInterval, Controller *controller) {

    controller->scheduleWakeUp(message, endWakeupTransmission+(subCycleInterval/2));

}

double ReactiveProvisioner::getParallelism(Cycle *cycle) {
    double parallelism = cycle->getWorkingPNAs()/(double)cycle->getInstance()->getRequestedDevices();
    cycle->setParallelism(parallelism);
    return parallelism;
}
