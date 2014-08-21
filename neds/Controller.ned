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

simple Controller
{
    parameters:
        @display("i=device/server2_l");
        
 		int addr = default(0); 
        int transmitterAddr = default(1);


        int expRepetitions = default(3);
		int expId = default(1);
        int currRepetition = default(1);
        int instancesNum = default(3);
		double initialAvailability_ = default(0.2);
		double volatility_ = default(0.05);
        
        string ranks = default("4 4 4");
        string imageSize_ = default("5Mib");
        
        int devNum = default(40);
        
        string probeIntervalInformative_ = default("15min");
        
        string cycleInt = default("1h");
        string subCycleInt = default("10min");
        double probFactor = default(0.001);
        double factorIncrement_ = default(0.2);
        
        string heuris = default("reativa"); 
        string estimationMethod = default("informativa"); 
        string selec = default("tempestiva"); 
        
        int broadcastBandwidth_ @unit(bps) = default(1Mbps);
        
        string workload_ = default("small_1_instance.txt");
        
        bool wholeLog = default(false);
        bool receivingMsgLog = default(false);
        bool wakeupLog = default(false);
        
        string evaluationShift_ = default("30s");
        string finishSubCycleShift_ = default("10s");
        int finishCycleShift_ @unit(s) = default(1min);
        
    gates:
        inout g;
}
