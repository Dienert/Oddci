/*
 * PNASelector.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */


#ifndef PNASELECTOR_H_
#define PNASELECTOR_H_

#include "Cycle.h"

class PNASelector {
public:
    enum Selection {
        TEMPESTIVE, RANKING
    };

private:

    Selection selection;

public:
    PNASelector(Selection selection);
    virtual ~PNASelector();

    virtual int getNextRankingTarget(Cycle *cycle) = 0;

    virtual Selection getSelection();
};

#endif /* PNASELECTOR_H_ */
