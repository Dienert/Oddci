/*
 * ProactiveProvisioner.cc
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#include "ProactiveProvisioner.h"

ProactiveProvisioner::ProactiveProvisioner(PNASelector* pnaSelector, PNAEstimator *pnaEstimator)
            : Provisioner(Provisioner::PROACTIVE, pnaSelector, pnaEstimator) {

}

ProactiveProvisioner::~ProactiveProvisioner() {
}

//int ProactiveProvisioner::getNextRequestingDevices(Cycle *cycle, double volatilityFactor) {
//    SubCycle *prevSubCycle = cycle->getPrevSubCycle();
//
//    if (cycle->getId() == 0 && cycle->getCurrentSubCycle()->getId() == 0) {
//        return cycle->getInstance()->getRequestedDevices();
//    } else {
//        return prevSubCycle->getFailedDevices()*2;
//    }
//}

int ProactiveProvisioner::getNextRequestingDevices(Cycle *cycle, double *volatilityFactor) {

    SubCycle *prevSubCycle = cycle->getPrevSubCycle();
    Instance *instance = cycle->getInstance();

    if ((cycle->getId() == 0 || cycle->getId() == 1) || cycle->getCurrentSubCycle()->getId() != 0) {
            if (prevSubCycle->getDeficitDevices() < 0) { // caso os ativos estejam acima dos solicitados
                return prevSubCycle->getFailedDevices();
            } else {
                if (instance->getId() != 0 && (cycle->getId() == 0 || cycle->getId() == 1) && cycle->getCurrentSubCycle()->getId() == 0) {
                    return prevSubCycle->getDeficitDevices()/(1-*volatilityFactor);
                } else {
                    return prevSubCycle->getDeficitDevices()+(prevSubCycle->getFailedDevices()*3);
                }
            }
    } else {
        Cycle *prevPrevCycle = cycle->getPrevCycle()->getPrevCycle();
        *volatilityFactor = 1-prevPrevCycle->getCompletedSlots()/(double)prevPrevCycle->getStartedSlots();
        if (prevPrevCycle->getCompletedSlots() != 0) {
            return (prevPrevCycle->getStartedSlots()*instance->getRequestedDevices()/prevPrevCycle->getCompletedSlots()) - prevSubCycle->getWorkingPNAs();
        } else {
            return instance->getRequestedDevices();
        }
    }

}

//int ProactiveProvisioner::getNextRequestingDevices(Cycle *cycle, double volatilityFactor) {
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
//    if (volatilityFactor > 1) {
//        return volatilityFactor * prevSubCycle->getFailedDevices();
//    } else {
//        return prevSubCycle->getFailedDevices();
//    }
//}

//int ProactiveProvisioner::getNextRequestingDevices(Cycle *cycle, double volatilityRate) {
//    SubCycle *prevSubCycle = cycle->getPrevSubCycle();
//
//    double subCycleVolatilityRate = cycle->getVolatilityRate();
//
//    if (cycle->getId() == 0 && cycle->getCurrentSubCycle()->getId() == 0) {
//        // se for o subciclo 0.0 não tem volatilidade, mas dá pra pegar de outras instancias
//        subCycleVolatilityRate = volatilityRate;
//        if (subCycleVolatilityRate > 1) {
//            return subCycleVolatilityRate * cycle->getInstance()->getRequestedDevices();
//        } else {
//            return cycle->getInstance()->getRequestedDevices();
//        }
//    }
//
//    volatilityRate = subCycleVolatilityRate;
//
//    if (volatilityRate > 1) {
//        return volatilityRate * prevSubCycle->getFailedDevices();
//    } else {
//        return prevSubCycle->getFailedDevices();
//    }
//}

simtime_t ProactiveProvisioner::getNextEvaluationTime(simtime_t wakeupArriveTime, Cycle *cycle) {
    if (cycle->getId() == 0 && cycle->getCurrentSubCycle()->getId() == 0) {
        if (getPnaEstimator()->getMethod() == PNAEstimator::EXPLORATORY) { // se for exploratorio, manda um wakeup rápido
            return wakeupArriveTime + 60;
        }
        else {
            // deslocamento de 5 min para a avaliação da instância
            // nos próximos ciclos e subciclos não há deslocamento
            return wakeupArriveTime + getSubCycleInterval() + getEvaluationShift();
        }
    } else {
        return wakeupArriveTime + getSubCycleInterval();
    }
}

bool ProactiveProvisioner::isSendWakeup(Cycle *cycle) {
    bool sendWakeup = false;
    if (cycle->getCurrentSubCycle()->getRequestingDevices() > 0) {
        sendWakeup = true;
    }
    return sendWakeup;
}

void ProactiveProvisioner::posEvaluation(WakeupMsg *message, SubCycle *subCycle, simtime_t endWakeupTransmission, simtime_t subCycleInterval, Controller *controller) {

}

double ProactiveProvisioner::getParallelism(Cycle *cycle) {
    double parallelism = cycle->getCompletedSlots()/(double)cycle->getInstance()->getRequestedDevices();
    cycle->setParallelism(parallelism);
    return parallelism;
}

