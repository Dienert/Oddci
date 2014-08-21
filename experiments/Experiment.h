/*
 * Experiment.h
 *
 *  Created on: 02/09/2012
 *      Author: Dienert
 */

#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_

#include "ExpInstance.h"
#include <vector>

using namespace std;

class Experiment {
public:
    Experiment();
    virtual ~Experiment();
    char *getAvailabilityCheckInterval() const;
    char *getCycleInterval() const;
    int getDevicesNumber() const;
    int getId() const;
    int getRepetitions() const;
    char *getSubcycleInterval() const;
    void setAvailabilityCheckInterval(char *availabilityCheckInterval);
    void setCycleInterval(char *cycleInterval);
    void setDevicesNumber(int devicesNumber);
    void setId(int id);
    void setRepetitions(int repetitions);
    void setSubcycleInterval(char *subcycleInterval);
    char *getBroadcastBandwidth() const;
    float getPnaAverageSessionTime() const;
    char *getPnaMaxDelay() const;
    char *getReturnChannelBandwidth() const;
    void setBroadcastBandwidth(char *broadcastBandwidth);
    void setPnaAverageSessionTime(float pnaAverageSessionTime);
    void setPnaMaxDelay(char * pnaMaxDelay);
    void setReturnChannelBandwidth(char *returnChannelBandwidth);
    vector<ExpInstance *> * getInstances() const;
    void setInstances(vector<ExpInstance *> * instances);
    char *getWorkload() const;
    void setWorkload(char *workload);
    char *getInitialAvailability() const;
    char *getVolatility() const;
    void setInitialAvailability(char *initialAvailability);
    void setVolatility(char *volatility);
    char *getImageSize() const;
    void setImageSize(char *imageSize);
private:
    int id;
    int devicesNumber;
    char *cycleInterval;
    char *subcycleInterval;
    int repetitions;
    char *availabilityCheckInterval;
    float pnaAverageSessionTime;
    char *broadcastBandwidth;
    char *returnChannelBandwidth;
    char * pnaMaxDelay;
    char *workload;
    char *initialAvailability;
    char *volatility;
    char *imageSize;
    vector<ExpInstance*> *instances;
};

#endif /* EXPERIMENT_H_ */
