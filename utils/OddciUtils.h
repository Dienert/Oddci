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

#ifndef __ODDCI_ODDCIUTILS_H
#define __ODDCI_ODDCIUTILS_H

#include <omnetpp.h>
#include "cnamedobject.h"
#include "cobject.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <locale>
#include <sstream>

using namespace std;

// Defining own numeric facet:

class WithComma: public numpunct<char> // class for decimal numbers using comma instead of point
{
    protected:
        char do_decimal_point() const { return ','; } // change the decimal separator
};

class OddciUtils {

private:
	static ofstream out;
	static bool init;
	static bool logActivated;
	static char *logFile;

  public:

	static string itoa(int number);
	static string dtoa(double number);
	static int atoi(string str);
	static int atoi(char *array);
	static vector<int> getIntsFromString(const char* grouped);
	static vector<char *> getCharsFromSplitedChars(const char* times);
	static vector<string> getStringsFromSplitedChars(const char* str);
	static void print(vector<string> vetor);
	static void stop(string message);
	static string getDurationFromSecondsWithDots(double seconds);
	static string getDurationFromSeconds(double seconds);
	static void setLogActive(bool logActivated);
	static bool isLogActive();
	static string convertLocalNumber(double number);
    static char *getLogFile();
    static void setLogFile(char *logFile);
};

#endif
