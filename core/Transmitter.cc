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

#include "Transmitter.h"
#include "Controller.h"
#include "OddciUtils.h"
#include "algorithm"

#define MSGKIND_CONNECT  	  0
#define MSGKIND_SEND     	  1
#define MSGKIND_BROADCAST     2

Define_Module(Transmitter);

Transmitter::Transmitter() {
}

double Transmitter::generateRNG(int experimentId, int currentRepetition) {
    return 100+experimentId*(currentRepetition+1);
}

void Transmitter::initialize() {
    address = par("addr");
    carouselInterval = SimTime::parse(par("carouselInterv"));
    datarate = par("broadcastBandwidth_");
    pnaMaxDelay = par("pnaMaxDelay_");

    for (int i = 0; i < gateSize("outAir1"); i++) {
        indexes1.push_back(i);
    }
    for (int i = 0; i < gateSize("outAir2"); i++) {
        indexes2.push_back(i);
    }
    for (int i = 0; i < gateSize("outAir3"); i++) {
        indexes3.push_back(i);
    }
    for (int i = 0; i < gateSize("outAir4"); i++) {
        indexes4.push_back(i);
    }
    for (int i = 0; i < gateSize("outAir5"); i++) {
        indexes5.push_back(i);
    }

    int expId = par("expId");
    int repetition = par("currRepetition");
    RNG = generateRNG(expId, repetition);

//    vector<string> vetor;
//    vetor.push_back("### RNG ### Transmitter =");
//    vetor.push_back(OddciUtils::dtoa(RNG));
//    OddciUtils::print(vetor);

    firstWakeup = true;
}

void Transmitter::broadCast(cMessage *msg) {

    cMessage *broadCastMsg = msg->dup();

    random_shuffle(indexes1.begin(), indexes1.end());
    random_shuffle(indexes2.begin(), indexes2.end());
    random_shuffle(indexes3.begin(), indexes3.end());
    random_shuffle(indexes4.begin(), indexes4.end());
    random_shuffle(indexes5.begin(), indexes5.end());

//    cPacket * pkt = check_and_cast<cPacket *>(msg);
//    int size = pkt->getByteLength();
//    double trasmissionDelay = size * 8 / datarate;

    double trasmissionDelay = 0; // transformando bytes em bits e dividindo por bits por segundo

    for (int i = 0; i < gateSize("outAir1"); i++) {
        cMessage *copy = broadCastMsg->dup();
        if (firstWakeup) {
            delays[0].push_back(uniform(0, pnaMaxDelay, RNG));
        }
        double totalDelay = trasmissionDelay + delays[0].at(i);
        sendDelayed(copy, totalDelay, "outAir1", indexes1.at(i));
    }
    for (int i = 0; i < gateSize("outAir2"); i++) {
        cMessage *copy = broadCastMsg->dup();
        if (firstWakeup) {
            delays[1].push_back(uniform(0, pnaMaxDelay, RNG));
        }
        double totalDelay = trasmissionDelay + delays[1].at(i);
        sendDelayed(copy, totalDelay, "outAir2", indexes2.at(i));
    }
    for (int i = 0; i < gateSize("outAir3"); i++) {
        cMessage *copy = broadCastMsg->dup();
        if (firstWakeup) {
            delays[2].push_back(uniform(0, pnaMaxDelay, RNG));
        }
        double totalDelay = trasmissionDelay + delays[2].at(i);
        sendDelayed(copy, totalDelay, "outAir3", indexes3.at(i));
    }
    for (int i = 0; i < gateSize("outAir4"); i++) {
        cMessage *copy = broadCastMsg->dup();
        if (firstWakeup) {
            delays[3].push_back(uniform(0, pnaMaxDelay, RNG));
        }
        double totalDelay = trasmissionDelay + delays[3].at(i);
        sendDelayed(copy, totalDelay, "outAir4", indexes4.at(i));
    }
    for (int i = 0; i < gateSize("outAir5"); i++) {
        cMessage *copy = broadCastMsg->dup();
        if (firstWakeup) {
            delays[4].push_back(uniform(0, pnaMaxDelay, RNG));
        }
        double totalDelay = trasmissionDelay + delays[4].at(i);
        sendDelayed(copy, totalDelay, "outAir5", indexes5.at(i));
    }

    firstWakeup = false;

    delete msg;
}

void Transmitter::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        broadCast(msg);
    } else {
        broadCast(msg);
    }
}

Transmitter::~Transmitter() {
    cOwnedObject *Del = NULL;
    int OwnedSize = this->defaultListSize();
    for (int i = 0; i < OwnedSize; i++) {
        Del = this->defaultListGet(0);
        this->drop(Del);
        delete Del;
    }
}

