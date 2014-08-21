/*
 * ExploratoryPNAEstimator.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#ifndef EXPLORATORYPNAESTIMATOR_H_
#define EXPLORATORYPNAESTIMATOR_H_

#include "PNAEstimator.h"
#include "Instance.h"

/**
 * Antes de cada wakeup real, manda um wakeup exploratório com
 * um fingerprint bem pequeno e de uma instância especial 0,
 * todos os pnas serão rejeitados
 * Após receber o retorno de todos, inferir o tamanho e mandar o wakeup real
 */
class ExploratoryPNAEstimator: public PNAEstimator {
private:
    double initialFactor;
    double factorIncrement;

public:
    ExploratoryPNAEstimator(double initialFactor) : PNAEstimator(PNAEstimator::EXPLORATORY) {
        this->initialFactor = initialFactor;
    }
    virtual ~ExploratoryPNAEstimator();

    virtual double getNextSubCycleFactor(Cycle *cycle);
    double getFactorIncrement() const;
    double getInitialFactor() const;
    void setFactorIncrement(double factorIncrement);
    void setInitialFactor(double initialFactor);
};

#endif /* EXPLORATORYPNAESTIMATOR_H_ */
