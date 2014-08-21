/*
 * InformativePNAEstimator.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#ifndef INFORMATIVEPNAESTIMATOR_H_
#define INFORMATIVEPNAESTIMATOR_H_

#include "PNAEstimator.h"
#include "Instance.h"

/**
 * Deve ser baseada em sondas que já foram recebidas
 * No primeiro momento, tenta aproveitar os retornos
 * de cada wakeup e vai refinando esse número
 */
class InformativePNAEstimator: public PNAEstimator {
public:
    InformativePNAEstimator();
    virtual ~InformativePNAEstimator();

    virtual double getNextSubCycleFactor(Cycle *cycle);
    Instance *getFakeInstance() const;
    void setFakeInstance(Instance *fakeInstance);

private:
    Instance *fakeInstance;
};

#endif /* INFORMATIVEPNAESTIMATOR_H_ */
