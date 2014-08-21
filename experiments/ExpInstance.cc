/*
 * ExpInstance.cc
 *
 *  Created on: Sep 7, 2012
 *      Author: root
 */

#include "ExpInstance.h"

ExpInstance::ExpInstance() {

}

ExpInstance::~ExpInstance() {
}

char *ExpInstance::getImageSize() const
{
    return imageSize;
}

char * ExpInstance::getRequestedDevices() const
{
    return requestedDevices;
}

char * ExpInstance::getRequestedSlots() const
{
    return requestedSlots;
}

char *ExpInstance::getStartTime() const
{
    return startTime;
}

void ExpInstance::setImageSize(char *imageSize)
{
    this->imageSize = imageSize;
}

void ExpInstance::setRequestedDevices(char * requestedDevices)
{
    this->requestedDevices = requestedDevices;
}

void ExpInstance::setRequestedSlots(char * requestedSlots)
{
    this->requestedSlots = requestedSlots;
}

void ExpInstance::setStartTime(char *startTime)
{
    this->startTime = startTime;
}

char * ExpInstance::getId() const
{
    return id;
}

void ExpInstance::setId(char * id)
{
    this->id = id;
}








