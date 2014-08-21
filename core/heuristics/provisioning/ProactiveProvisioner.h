/*
 * ProactiveProvisioner.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#ifndef PROACTIVEPROVISIONER_H_
#define PROACTIVEPROVISIONER_H_

#include "Provisioner.h"
#include "Cycle.h"
class Controller;

class ProactiveProvisioner: public Provisioner {
public:
    ProactiveProvisioner(PNASelector* pnaSelector, PNAEstimator *pnaEstimator);
    virtual ~ProactiveProvisioner();

//    virtual int getNextRequestingDevices(Cycle *cycle);
    virtual int getNextRequestingDevices(Cycle *cycle, double *volatilityFactor);
    virtual simtime_t getNextEvaluationTime(simtime_t wakeupArriveTime, Cycle *cycle);
    virtual bool isSendWakeup(Cycle *cycle);
    void posEvaluation(WakeupMsg *message, SubCycle *subCycle, simtime_t endWakeupTransmission, simtime_t subCycleInterval, Controller *controller);
    double getParallelism(Cycle *cycle);

};

#endif /* PROACTIVEPROVISIONER_H_ */
