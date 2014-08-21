/*
 * RankingSelector.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */


#ifndef RANKINGSELECTOR_H_
#define RANKINGSELECTOR_H_

#include "PNASelector.h"

class RankingSelector : public PNASelector {
public:
    RankingSelector();
    virtual ~RankingSelector();

    virtual int getNextRankingTarget(Cycle *cycle);
};

#endif /* RANKINGSELECTOR_H_ */
