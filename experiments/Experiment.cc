/*
 * Experiment.cpp
 *
 *  Created on: 02/09/2012
 *      Author: Dienert
 */

#include "Experiment.h"

Experiment::Experiment() {
    instances = new vector<ExpInstance *>();
}

Experiment::~Experiment() {
}

char *Experiment::getAvailabilityCheckInterval() const {
    return availabilityCheckInterval;
}

char *Experiment::getCycleInterval() const {
    return cycleInterval;
}

int Experiment::getDevicesNumber() const {
    return devicesNumber;
}

int Experiment::getId() const {
    return id;
}

int Experiment::getRepetitions() const {
    return repetitions;
}

char *Experiment::getSubcycleInterval() const {
    return subcycleInterval;
}

void Experiment::setAvailabilityCheckInterval(char *availabilityCheckInterval) {
    this->availabilityCheckInterval = availabilityCheckInterval;
}

void Experiment::setCycleInterval(char *cycleInterval) {
    this->cycleInterval = cycleInterval;
}

void Experiment::setDevicesNumber(int devicesNumber) {
    this->devicesNumber = devicesNumber;
}

void Experiment::setId(int id) {
    this->id = id;
}

void Experiment::setRepetitions(int repetitions) {
    this->repetitions = repetitions;
}

void Experiment::setSubcycleInterval(char *subcycleInterval) {
    this->subcycleInterval = subcycleInterval;
}

char *Experiment::getBroadcastBandwidth() const {
    return broadcastBandwidth;
}

float Experiment::getPnaAverageSessionTime() const {
    return pnaAverageSessionTime;
}

char * Experiment::getPnaMaxDelay() const {
    return pnaMaxDelay;
}

char *Experiment::getReturnChannelBandwidth() const {
    return returnChannelBandwidth;
}

void Experiment::setBroadcastBandwidth(char *broadcastBandwidth) {
    this->broadcastBandwidth = broadcastBandwidth;
}

void Experiment::setPnaAverageSessionTime(float pnaAverageSessionTime) {
    this->pnaAverageSessionTime = pnaAverageSessionTime;
}

void Experiment::setPnaMaxDelay(char * pnaMaxDelay) {
    this->pnaMaxDelay = pnaMaxDelay;
}

void Experiment::setReturnChannelBandwidth(char *returnChannelBandwidth) {
    this->returnChannelBandwidth = returnChannelBandwidth;
}

vector<ExpInstance *> * Experiment::getInstances() const
{
    return instances;
}

void Experiment::setInstances(vector<ExpInstance *> *instances)
{
    this->instances = instances;
}

char *Experiment::getWorkload() const
{
    return workload;
}

void Experiment::setWorkload(char *workload)
{
    this->workload = workload;
}

char *Experiment::getInitialAvailability() const
{
    return initialAvailability;
}

char *Experiment::getVolatility() const
{
    return volatility;
}

void Experiment::setInitialAvailability(char *initialAvailability)
{
    this->initialAvailability = initialAvailability;
}

void Experiment::setVolatility(char *volatility)
{
    this->volatility = volatility;
}

char *Experiment::getImageSize() const
{
    return imageSize;
}

void Experiment::setImageSize(char *imageSize)
{
    this->imageSize = imageSize;
}









