

#include "Provider.h"

using namespace std;

#define MSGKIND_SEND     	  1
#define MSGKIND_BROADCAST     2

Define_Module(Provider);

void Provider::initialize()
{
//    const char *address = par("localAddress");
//    int portSend = par("localPort");
//
//    socket.bind(*address ? IPvXAddress(address) : IPvXAddress(), portSend);

	setStatusString("waiting");

    timerMessage = new cMessage("timer");
    timerMessage->setKind(MSGKIND_SEND);
//    scheduleAt(simTime(), timerMessage);

}

void Provider::sendMessage()
{
    WakeupMsg *msg = new WakeupMsg("wakeup");
    msg->setSrcAddress(2);
    msg->setDestAddress(0);

    send(msg, "g");

}

void Provider::handleTimer(cMessage *msg)
{
    switch (msg->getKind())
    {
        case MSGKIND_SEND:
           sendMessage();
           break;
    }
}

void Provider::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage()) {
        handleTimer(msg);
	} else {
		sendMessage();
	}

}

void Provider::setStatusString(const char *s) {
    if (ev.isGUI()) getDisplayString().setTagArg("t", 0, s);
}

