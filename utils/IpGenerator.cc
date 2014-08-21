#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>

using namespace std;

int ma(int argc, char **argv) {
	ofstream out;
	out.open("ips.txt");
	int number = strtol(argv[1], &argv[1], 10);
	int count = 0;
	for (int i = 0; i < 255;i++) {
		for (int j = 0; j < 255;j++) {
			for (int k = 1; k < 255;k++) {
				if (count < number) {
					out << 10 << "." << i << "." << j << "." << k << " ";
					count++;
				} else {
					out.close();
					return 0;
				}
			}
		}
	}
	out.close();
	return 0;
}

