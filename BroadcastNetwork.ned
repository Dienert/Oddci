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

package oddci;

import oddci.neds.Transmitter;
import oddci.neds.PNA;
import oddci.neds.Controller;
import oddci.neds.Provider;
import oddci.neds.Router;
import ned.DatarateChannel;

channel fiberLine extends DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 512Mbps;
}

channel directChannel extends DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = default(1Mbps);
}

network BroadcastNetwork
{
    parameters:
        int devicesNumber = default(1000);

        int experimentId = default(1);

        string cycleInterval = default("1h"); // Ciclos de 1 hora

        string subCycleInterval = default("10min"); // SubCiclos de 10 min 
        // é apenas o inicial, deve ser dinamico e aferido dinamicamente de forma geral - 
        // para cada novo subciclo de uma instância deve ser informado na resposta da sonda 
        // e deve impactar no proximo momento de avaliaçao (um shift por subciclo)

        string activeHeuristic = default("reactive");
        string activePNAsEstimationMethod = default("informative"); // exploratory or informative
        string selection = default("tempestive");

        double initialAvailability = default(1);

//        double pnaAverageSessionTime = default(5.5);

        int repetitions = default(3);
        int currentRepetition = default(1);

        bool logActivated = default(true);
        bool logControllerReceivingMsg = default(false);
        bool logControllerWakeup = default(false);
        bool logRouterActivated = default(false);

        string probeIntervalInformative = default("15min");

        int broadcastBandwidth @unit(bps) = default(1Mbps);

        string workload = default("small_1_instance.txt");

        string imageSize = default("5MiB"); // tamanho default da aplicação 

        int pnaMaxDelay @unit(s) = default(5s);

        string rankings = default("4"); //TODO: vai sair para dentro da heuristica

        string evaluationShift = default("30s");
        string finishSubCycleShift = default("0.5s"); //
        int finishCycleShift @unit(s) = default(1min);


        double factor = default(0.1);
        double factorIncrement = default(0.2);

        double volatility = default(0.5);

        int waitingTimeOut @unit(s) = default(5min);

        int availabilityCheckInterval = default(1800); // checagem do tempo de disponibilidade - deve ser associado com slot size e mover pra a configuracao do PNA
        @display("bgb=824,454");
    submodules:
        //        provider: Provider {
        //            @display("p=105,190");
        //            providerAddress = "10.0.0.1";
        //            controller1Address = "10.0.0.2";
        //        }
        controller: Controller {
            @display("p=90,127");
            addr = 0;
            transmitterAddr = 1; // transmitter
            expId = experimentId;
            expRepetitions = repetitions; //numero de repetições
            devNum = devicesNumber;
            cycleInt = cycleInterval;
            subCycleInt = subCycleInterval;

            heuris = activeHeuristic;
            estimationMethod = activePNAsEstimationMethod;
            selec = selection;

            workload_ = workload;
            volatility_ = volatility;
            initialAvailability_ = initialAvailability;

            broadcastBandwidth_ = broadcastBandwidth;

            probeIntervalInformative_ = probeIntervalInformative;

            probFactor = factor;
            currRepetition = currentRepetition;

            evaluationShift_ = evaluationShift;
            finishSubCycleShift_ = finishSubCycleShift;
            finishCycleShift_ = finishCycleShift;

            wholeLog = logActivated;
            receivingMsgLog = logControllerReceivingMsg;
            wakeupLog = logControllerWakeup;
            factorIncrement_ = factorIncrement;
            imageSize_ = imageSize;

        }
        transmitter: Transmitter {
            parameters:
                @display("p=581,211;r=200");
                nummeroDevices = devicesNumber;
                addr = 1;
                broadcastBandwidth_ = broadcastBandwidth;
                pnaMaxDelay_ = pnaMaxDelay;
                //carouselInterv = carouselInterval;
        }
        pnaGroup1[devicesNumber/5]: PNA {
            addr = index+500001;
            controllerAddr = 0;
            volatility_ = volatility;
            estimationMethod = activePNAsEstimationMethod;
            cycleInterval_ = cycleInterval;
            checkInterval = availabilityCheckInterval;
            initialAvailability_ = initialAvailability;
            //            pnaAverageSessionTime_ = pnaAverageSessionTime;
            informativeProbeInterval_ = probeIntervalInformative;
            numDevices = devicesNumber;
            pnaMaxDelay_ = pnaMaxDelay;
            waitingTimeOut_ = waitingTimeOut;
            @display("p=506,43,ri,30,30");
        }

        pnaGroup2[devicesNumber/5]: PNA {
            addr = index+1000001;
            volatility_ = volatility;
            estimationMethod = activePNAsEstimationMethod;
            controllerAddr = 0;
            cycleInterval_ = cycleInterval;
            checkInterval = availabilityCheckInterval;
            initialAvailability_ = initialAvailability;
            //            pnaAverageSessionTime_ = pnaAverageSessionTime;
            informativeProbeInterval_ = probeIntervalInformative;
            numDevices = devicesNumber;
            pnaMaxDelay_ = pnaMaxDelay;
            waitingTimeOut_ = waitingTimeOut;
            @display("p=662,101,ri,30,30");
        }

        pnaGroup3[devicesNumber/5]: PNA {
            addr = index+1500001;
            volatility_ = volatility;
            estimationMethod = activePNAsEstimationMethod;
            cycleInterval_ = cycleInterval;
            controllerAddr = 0;
            checkInterval = availabilityCheckInterval;
            initialAvailability_ = initialAvailability;
            //            pnaAverageSessionTime_ = pnaAverageSessionTime;
            informativeProbeInterval_ = probeIntervalInformative;
            numDevices = devicesNumber;
            pnaMaxDelay_ = pnaMaxDelay;
            waitingTimeOut_ = waitingTimeOut;
            @display("p=681,239,ri,30,30");
        }

        pnaGroup4[devicesNumber/5]: PNA {
            addr = index+2000001;
            volatility_ = volatility;
            estimationMethod = activePNAsEstimationMethod;
            controllerAddr = 0;
            cycleInterval_ = cycleInterval;
            checkInterval = availabilityCheckInterval;
            initialAvailability_ = initialAvailability;
            //            pnaAverageSessionTime_ = pnaAverageSessionTime;
            informativeProbeInterval_ = probeIntervalInformative;
            numDevices = devicesNumber;
            pnaMaxDelay_ = pnaMaxDelay;
            waitingTimeOut_ = waitingTimeOut;
            @display("p=551,307,ri,30,30");
        }

        pnaGroup5[devicesNumber/5]: PNA {
            addr = index+2500001;
            volatility_ = volatility;
            estimationMethod = activePNAsEstimationMethod;
            controllerAddr = 0;
            cycleInterval_ = cycleInterval;
            checkInterval = availabilityCheckInterval;
            initialAvailability_ = initialAvailability;
            //            pnaAverageSessionTime_ = pnaAverageSessionTime;
            informativeProbeInterval_ = probeIntervalInformative;
            numDevices = devicesNumber;
            pnaMaxDelay_ = pnaMaxDelay;
            waitingTimeOut_ = waitingTimeOut;
            @display("p=551,370,ri,30,30");
        }

        router1: Router {
            @display("p=191,211");
            id = 1;
            logActived = logRouterActivated;
        }

        router2: Router {
            @display("p=335,101");
            id = 2;
            logActived = logRouterActivated;
        }

        router3: Router {
            @display("p=335,180");
            id = 3;
            logActived = logRouterActivated;
        }

        router4: Router {
            @display("p=336,256");
            id = 4;
            logActived = logRouterActivated;
        }

        router5: Router {
            @display("p=335,322");
            id = 5;
            logActived = logRouterActivated;
        }

        router6: Router {
            @display("p=335,398");
            id = 6;
            logActived = logRouterActivated;
        }
        
    connections:
        controller.g <--> router1.g++;
        transmitter.g <--> router1.g++;
        router1.g++ <--> fiberLine <--> router2.g++;
        router1.g++ <--> fiberLine <--> router3.g++;
        router1.g++ <--> fiberLine <--> router4.g++;
        router1.g++ <--> fiberLine <--> router5.g++;
        router1.g++ <--> fiberLine <--> router6.g++;

        for i=0..sizeof(pnaGroup1)-1 {
            transmitter.outAir1++ --> { @display("ls=,0"); } --> pnaGroup1[i].inAir;
            pnaGroup1[i].g <--> directChannel <--> router2.g++;
        }

        for i=0..sizeof(pnaGroup2)-1 {
            transmitter.outAir2++ --> { @display("ls=,0"); } --> pnaGroup2[i].inAir;
            pnaGroup2[i].g <--> directChannel <--> router3.g++;
        }

        for i=0..sizeof(pnaGroup3)-1 {
            transmitter.outAir3++ --> { @display("ls=,0"); } --> pnaGroup3[i].inAir;
            pnaGroup3[i].g <--> directChannel <--> router4.g++;
        }

        for i=0..sizeof(pnaGroup4)-1 {
            transmitter.outAir4++ --> { @display("ls=,0"); } --> pnaGroup4[i].inAir;
            pnaGroup4[i].g <--> directChannel <--> router5.g++;
        }
        
        for i=0..sizeof(pnaGroup5)-1 {
            transmitter.outAir5++ --> { @display("ls=,0"); } --> pnaGroup5[i].inAir;
            pnaGroup5[i].g <--> directChannel <--> router6.g++;
        }

}
