
#include <stdlib.h>
#include <vector>
#include "ExpInstance.h"
#include "OddciUtils.h"
#include <math.h>
#include <simtime.h>

using namespace std;

int main3(int argc, char **argv) {
    ifstream workloadIn(argv[1]);
    char workloadOutName[1000];
    sprintf(workloadOutName, "%s-%s", argv[1], "adapted.txt");
    ofstream workloadOut(workloadOutName);
//    vector<ExpInstance *> instances;
    long soma = 0;
    while (!workloadIn.eof()) {
        int id;
        workloadIn >> id;
        double interval;
        workloadIn >> interval;
        int slotDuration;
        workloadIn >> slotDuration;
        int requestedDevices;
        workloadIn >> requestedDevices;

        soma += round(interval*3600);

        workloadOut << OddciUtils::getDurationFromSeconds(soma) << "\t";
        workloadOut << requestedDevices << "\t";
        workloadOut << slotDuration*requestedDevices << endl;
    }
    return 0;
}

