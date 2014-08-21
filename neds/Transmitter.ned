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

package oddci.neds;

// Generates messages with a configurable interarrival time.
//
simple Transmitter
{
    parameters:
        volatile double sendInterval @unit(s) = default(exponential(1s));
        int nummeroDevices;
        int addr = default(1);
        int broadcastBandwidth_ @unit(bps) = default(1Mbps);
        string carouselInterv = default("42s"); // 42 segundos
        int pnaMaxDelay_ @unit(s) = default(5s);
		int expId = default(1);
        int currRepetition = default(1);
        
        @display("bgl=2;i=device/antennatower;is=l");
        
    gates:
        output outAir1[];
        output outAir2[];
        output outAir3[];
        output outAir4[];
        output outAir5[];
        inout g;
}
