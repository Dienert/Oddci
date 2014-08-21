/*
 * ReactiveProvisioner.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */


#ifndef REACTIVEPROVISIONER_H_
#define REACTIVEPROVISIONER_H_

#include "Provisioner.h"
#include "Cycle.h"
#include "OddciUtils.h"
#include <vector>

class Controller;
class ReactiveProvisioner : public Provisioner {
public:
    ReactiveProvisioner(PNASelector* pnaSelector, PNAEstimator *pnaEstimator);
    virtual ~ReactiveProvisioner();

//    virtual int getNextRequestingDevices(Cycle *cycle);
    virtual int getNextRequestingDevices(Cycle *cycle, double *volatilityFactor);

    virtual simtime_t getNextEvaluationTime(simtime_t wakeupArriveTime, Cycle *cycle);

    virtual bool isSendWakeup(Cycle *cycle);

    void posEvaluation(WakeupMsg *message, SubCycle *subCycle, simtime_t endWakeupTransmission, simtime_t subCycleInterval, Controller *controller);

    double getParallelism(Cycle *cycle);

};

#endif /* REACTIVEPROVISIONER_H_ */
