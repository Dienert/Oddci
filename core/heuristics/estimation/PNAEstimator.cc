#include "PNAEstimator.h"

PNAEstimator::PNAEstimator(EstimationMethod method) {
    this->method = method;
}

PNAEstimator::~PNAEstimator() {

}

PNAEstimator::EstimationMethod PNAEstimator::getMethod() {
   return this->method;
}


long PNAEstimator::getEstimatedIdlePNAS() {
    return estimatedIdlePNAs;
}

void PNAEstimator::setEstimatedIdlePNAS(long estimatedPNAs) {
    this->estimatedIdlePNAs = estimatedPNAs;
}
