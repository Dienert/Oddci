#include "PNASelector.h"

PNASelector::~PNASelector() {

}

PNASelector::PNASelector(Selection selection) {
    this->selection = selection;
}

PNASelector::Selection PNASelector::getSelection() {
    return this->selection;
}
