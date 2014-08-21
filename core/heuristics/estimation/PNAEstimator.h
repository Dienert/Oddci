//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


#ifndef PNAESTIMATOR_H_
#define PNAESTIMATOR_H_

#include "Cycle.h"

class PNAEstimator {
public:
    enum EstimationMethod {
        INFORMATIVE, EXPLORATORY
    };
private:
    EstimationMethod method;
    int estimatedIdlePNAs;
public:
    PNAEstimator(EstimationMethod method);
    virtual ~PNAEstimator();

    virtual double getNextSubCycleFactor(Cycle *cycle) = 0;

    virtual EstimationMethod getMethod();

    virtual long getEstimatedIdlePNAS();
    virtual void setEstimatedIdlePNAS(long estimatedPNAs);


};

#endif /* PNAESTIMATOR_H_ */
