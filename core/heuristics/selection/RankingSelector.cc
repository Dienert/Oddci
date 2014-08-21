/*
 * RankingSelector.cpp
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#include "RankingSelector.h"

RankingSelector::RankingSelector() : PNASelector(PNASelector::TEMPESTIVE){

}

RankingSelector::~RankingSelector() {

}

int RankingSelector::getNextRankingTarget(Cycle *cycle) {
    int rankingTarget = 0;
    SubCycle *prevSubCycle= cycle->getPrevSubCycle();
    Instance   *instance = cycle->getInstance();

    double falha = (instance->getRequestedDevices() - prevSubCycle->getWorkingPNAs()) / (double) instance->getRequestedDevices();

    if (falha == 0) {
        if (prevSubCycle->getRankingTarget() < instance->getMaxRanking()) {
            rankingTarget = prevSubCycle->getRankingTarget() + 1;
        }
    } else if (falha > 0.1) {
        if (prevSubCycle->getRankingTarget() > 0) {
            rankingTarget = prevSubCycle->getRankingTarget() - 1;
        }
    }

    return rankingTarget;
}
