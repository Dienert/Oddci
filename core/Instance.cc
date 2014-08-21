#include "Instance.h"

using namespace std;

Instance::Instance(int id, int maxSubCycles, const char *name) :
        cNamedObject(name) {
	this->id = id;
	currentCycle = 0;
    cycleToRecord = 0;
    completedWithZeroCount = 0;
    Cycle *firstCycle = new Cycle(0, maxSubCycles, this);
    Cycle *secondCycle = new Cycle(1, maxSubCycles, this);
    cycles = new vector<Cycle *>();
    cycles->push_back(firstCycle);
    cycles->push_back(secondCycle);
    finished = false;
}

Instance::~Instance() {
    cycles->clear();
//    cycles.clear();
}

int Instance::getId() const {
    return id;
}

const SimTime Instance::getTimeToStart() const {
    return timeToStart;
}

int Instance::getRequestedSlots() const {
    return requestedSlots;
}

int Instance::getRequestedDevices() const {
    return requestedDevices;
}

int Instance::getMaxRanking() const {
    return maxRanking;
}

bool Instance::isFinished() const {
    return finished;
}

Cycle * Instance::getCurrentCycle() const {
    return cycles->at(currentCycle);
}

int Instance::getCompletedSlots() const {
    return completedSlots;
}

int Instance::getCompletedWithZeroCount() const {
    return completedWithZeroCount;
}

int Instance::getCycleToRecord() const {
    return cycleToRecord;
}

vector <Cycle *> * Instance::getCycles() {
    return cycles;
}

void Instance::setId(const int id) {
    this->id = id;
}

void Instance::setTimeToStart(const SimTime timeToStart) {
    this->timeToStart = timeToStart;
}

void Instance::setRequestedSlots(const int reqSlots) {
    this->requestedSlots = reqSlots;
}

void Instance::setRequestedDevices(const int requestedDevices) {
    this->requestedDevices = requestedDevices;
}

void Instance::setMaxRanking(const int maxRanking) {
    this->maxRanking = maxRanking;
}

void Instance::setCurrentCycle(const long currentCycle) {
    this->currentCycle = currentCycle;
}

void Instance::setCompletedSlots(const int completedSlots) {
    this->completedSlots = completedSlots;
}

void Instance::setCompletedWithZeroCount(const int completedWithZeroCount) {
    this->completedWithZeroCount = completedWithZeroCount;
}

void Instance::setCycleToRecord(const int cycleToRecord) {
    this->cycleToRecord = cycleToRecord;
}

cMessage * Instance::getTimerRecordCycle() const {
    return timerRecordCycle;
}

cMessage * Instance::getTimerChangeCycle() const {
    return timerChangeCycle;
}

cMessage * Instance::getTimerChangeSubCycle() const {
    return timerChangeSubCycle;
}

void Instance::setFinished(bool finished) {
    this->finished = finished;
}

void Instance::setTimerRecordCycle(cMessage *timerRecordCycle) {
    this->timerRecordCycle = timerRecordCycle;
}

void Instance::setTimerChangeCycle(cMessage *timerAddCycle) {
    this->timerChangeCycle = timerAddCycle;
}

void Instance::setTimerChangeSubCycle(cMessage *timerAddSubCycle) {
    this->timerChangeSubCycle = timerAddSubCycle;
}

void Instance::setTimerEvaluation(cMessage *timerEvaluation) {
    this->timerEvaluation = timerEvaluation;
}

cMessage * Instance::getTimerEvaluation() const {
    return timerEvaluation;
}

int Instance::getImageSize() const
{
    return imageSize;
}

bool Instance::isFakeCyle() const
{
    return fakeCyle;
}

void Instance::setFakeCyle(bool fakeCyle)
{
    this->fakeCyle = fakeCyle;
}

simtime_t Instance::getNextEvaluationTime() const
{
    return nextEvaluationTime;
}

cMessage *Instance::getTimerBroadcastSchedule() const
{
    return timerBroadcastSchedule;
}

void Instance::setTimerBroadcastSchedule(cMessage *timerBroadcastSchedule)
{
    this->timerBroadcastSchedule = timerBroadcastSchedule;
}

void Instance::setNextEvaluationTime(simtime_t nextEvaluationTime)
{
    this->nextEvaluationTime = nextEvaluationTime;
}

void Instance::setImageSize(int imageSize)
{
    this->imageSize = imageSize;
}

void Instance::setCycles(vector <Cycle *> *) {
    this->cycles = cycles;
}
