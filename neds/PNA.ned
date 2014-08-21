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

//
// Consumes received messages and collects statistics
//
simple PNA
{
    parameters:
        @display("i=device/device;is=n");
        int addr;
        int controllerAddr;
        double volatility_ = default(0.5);
	 	string estimationMethod = default("informative");       
	 	string cycleInterval_ = default("15min");       
        int checkInterval = default(1800); //intervalo de verificação de disponibilidade       
        int numberSeed = default(0);
        double initialAvailability_ = default(1);
//        double pnaAverageSessionTime_ = default(5.5);
        int numDevices = default(20);
        string informativeProbeInterval_ = default("15min");
        int pnaMaxDelay_ @unit(s) = default(5s);
        int waitingTimeOut_ @unit(s) = default(5min);
		int expId = default(1);
        int currRepetition = default(1);
        

    gates:
        input inAir;
        inout g;

}
