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

#ifndef __ODDCI_PROVIDER_H
#define __ODDCI_PROVIDER_H

#include <omnetpp.h>
#include "WakeupMsg_m.h"

/**
 * Message sink; see NED file for more info.
 */
class Provider : public cSimpleModule
{
  private:
    // state
    simtime_t lastArrival;
    int id;
	static int count;

  protected:
    cMessage *timerMessage;

    virtual void initialize();

    virtual void handleMessage(cMessage *msg);

    virtual void sendMessage();

    virtual void handleTimer(cMessage *msg);

    /** When running under GUI, it displays the given string next to the icon */
	virtual void setStatusString(const char *s);
	//@}

};


#endif
