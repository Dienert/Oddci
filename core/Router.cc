//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


#include <omnetpp.h>
#include "NetPkt_m.h"
#include "OddciUtils.h"
#include <vector>
#include <string>

/**
 * Represents the network "Router" between clients and the server;
 * see NED file for more info.
 */
class Router : public cSimpleModule
{
  private:
    simtime_t propDelay;
    int id;
    bool logActived;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Router);

void Router::initialize()
{
    propDelay = (double)par("propDelay");
    id = par("id");
    logActived = par("logActived");
}

void Router::handleMessage(cMessage *msg)
{
    try {
		// determine destination address
		NetPkt *pkt = check_and_cast<NetPkt *>(msg);
		int dest = pkt->getDestAddress();
		int src = pkt->getSrcAddress();
		EV << "Relaying packet to addr=" << dest << "\n";

		if (id == 1) { // Router 1
		    int router = dest/500000;
		    if (router == 0) { // Pacote para o controller ou o transmitter
		        if (logActived) {
                    vector<string> vetor;
                    vetor.push_back(OddciUtils::itoa(src));
                    vetor.push_back("> router 1 >");
                    vetor.push_back(OddciUtils::itoa(dest));
                    OddciUtils::print(vetor);
		        }
		        sendDelayed(pkt, propDelay, "g$o", dest);
		    } else { // Pacote para um receptor - enviar para um roteador antes
		        router++; // por causa do transmitter incrementa 1
		        if (logActived) {
                    vector<string> vetor;
                    vetor.push_back(OddciUtils::itoa(src));
                    vetor.push_back("> router 1 > router");
                    vetor.push_back(OddciUtils::itoa(router));
                    vetor.push_back(">");
                    vetor.push_back(OddciUtils::itoa(dest));
                    OddciUtils::print(vetor);
		        }
                sendDelayed(pkt, propDelay, "g$o", router);
		    }
		} else {
		    int index = dest % 500000;
		    if (logActived) {
                vector<string> vetor;
                vetor.push_back(OddciUtils::itoa(src));
                vetor.push_back("> router");
                vetor.push_back(OddciUtils::itoa(id));
                if (index == 0) {
                    vetor.push_back("> router 1 >");
                } else {
                    vetor.push_back(">");
                }
                vetor.push_back(OddciUtils::itoa(dest));
                OddciUtils::print(vetor);
		    }
            sendDelayed(pkt, propDelay, "g$o", index);
		}

    } catch (exception& e) {
    }
}


