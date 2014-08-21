#include "Provisioner.h"

Provisioner::Provisioner(Heuristic heuristic, PNASelector* pnaSelector, PNAEstimator *pnaEstimator) {
    this->heuristic = heuristic;
    this->pnaSelector = pnaSelector;
    this->pnaEstimator = pnaEstimator;
}

Provisioner::~Provisioner() {

}

SimTime Provisioner::getCycleInterval() const {
    return cycleInterval;
}

SimTime Provisioner::getSubCycleInterval() const {
    return subCycleInterval;
}

void Provisioner::setCycleInterval(SimTime cycleInterval) {
    this->cycleInterval = cycleInterval;
}

void Provisioner::setHeuristic(Heuristic heuristic) {
    this->heuristic = heuristic;
}

SimTime Provisioner::getEvaluationShift() const
{
    return evaluationShift;
}

void Provisioner::setEvaluationShift(SimTime initialShiftVerify)
{
    this->evaluationShift = initialShiftVerify;
}

void Provisioner::setSubCycleInterval(SimTime subCycleInterval) {
    this->subCycleInterval = subCycleInterval;
}

Provisioner::Heuristic Provisioner::getHeuristic() {
    return this->heuristic;
}

PNAEstimator *Provisioner::getPnaEstimator() {
    return pnaEstimator;
}

PNASelector *Provisioner::getPnaSelector() {
    return pnaSelector;
}

void Provisioner::setPnaEstimator(PNAEstimator *pnaEstimator) {
    this->pnaEstimator = pnaEstimator;
}

void Provisioner::setPnaSelector(PNASelector *pnaSelector) {
    this->pnaSelector = pnaSelector;
}

