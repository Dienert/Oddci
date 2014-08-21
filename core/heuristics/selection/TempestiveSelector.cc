/*
 * TempestiveSelector.cpp
 *
 *  Created on: Jun 16, 2012
 *      Author: Diénert Vieira
 */

#include "TempestiveSelector.h"

TempestiveSelector::TempestiveSelector() : PNASelector(PNASelector::TEMPESTIVE) {

}

TempestiveSelector::~TempestiveSelector() {

}

int TempestiveSelector::getNextRankingTarget(Cycle *cycle) {
    return 0;
}
