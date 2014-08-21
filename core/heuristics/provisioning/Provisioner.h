/*
 * Provisioner.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */


#ifndef PROVISIONER_H_
#define PROVISIONER_H_

#include <omnetpp.h>
#include "Cycle.h"
#include "Instance.h"
#include "PNAEstimator.h"
#include "PNASelector.h"
#include "WakeupMsg_m.h"
#include "SubCycle.h"
#include "Controller.h"

class Controller;
class Provisioner {
public:
    enum Heuristic {
        REACTIVE, PROACTIVE
    };

private:
    Heuristic heuristic;
    PNAEstimator *pnaEstimator;
    PNASelector *pnaSelector;
    SimTime cycleInterval;
    SimTime subCycleInterval;
    SimTime evaluationShift;

public:
    Provisioner(Heuristic heuristic, PNASelector* pnaSelector, PNAEstimator *pnaEstimator);
    virtual ~Provisioner();
//    virtual int getNextRequestingDevices(Cycle *cycle) = 0;
    virtual int getNextRequestingDevices(Cycle *cycle, double *volatilityFactor) = 0;
    virtual simtime_t getNextEvaluationTime(simtime_t wakeupArriveTime, Cycle *cycle) = 0;
    virtual bool isSendWakeup(Cycle *cycle) = 0;
    virtual void posEvaluation(WakeupMsg *message, SubCycle *subCycle, simtime_t endWakeupTransmission,simtime_t subCycleInterval, Controller *controller) = 0;
    virtual double getParallelism(Cycle *cycle) = 0;


    PNAEstimator *getPnaEstimator();
    PNASelector *getPnaSelector();

    void setPnaEstimator(PNAEstimator *pnaEstimator);
    void setPnaSelector(PNASelector *pnaSelector);

    virtual Heuristic getHeuristic();
    SimTime getCycleInterval() const;
    SimTime getSubCycleInterval() const;
    void setCycleInterval(SimTime cycleInterval);
    void setHeuristic(Heuristic heuristic);
    void setSubCycleInterval(SimTime subCycleInterval);
    SimTime getEvaluationShift() const;
    void setEvaluationShift(SimTime initialShiftVerify);
};


#endif /* PROVISIONER_H_ */
