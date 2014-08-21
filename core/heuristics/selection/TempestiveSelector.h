/*
 * TempestiveSelector.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */


#ifndef TEMPESTIVESELECTOR_H_
#define TEMPESTIVESELECTOR_H_

#include "PNASelector.h"

class TempestiveSelector : public PNASelector {
public:
    TempestiveSelector();
    virtual ~TempestiveSelector();

    virtual int getNextRankingTarget(Cycle *cycle);
};

#endif /* TEMPESTIVESELECTOR_H_ */
