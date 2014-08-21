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

#ifndef __ODDCI_TRANSMITTER_H
#define __ODDCI_TRANSMITTER_H

#include <omnetpp.h>
#include <vector>

using namespace std;

class Transmitter : public cSimpleModule
{
  public:
     Transmitter();
     virtual ~Transmitter();

  protected:
     simtime_t maxMsgDelay;

     simtime_t carouselInterval;

     int datarate;
     int pnaMaxDelay;
     int address;
     vector<double> delays[5];
     bool firstWakeup;

     vector<int> indexes1;
     vector<int> indexes2;
     vector<int> indexes3;
     vector<int> indexes4;
     vector<int> indexes5;

     double RNG;
     double generateRNG(int experimentId, int currentRepetition);

   protected:
     virtual void broadCast(cMessage *msg);

   protected:
     virtual void initialize();
     virtual void handleMessage(cMessage *msg);
};

#endif
