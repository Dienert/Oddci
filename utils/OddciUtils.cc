
#include "OddciUtils.h"

using namespace std;

ofstream OddciUtils::out;
bool OddciUtils::init = true;
bool OddciUtils::logActivated = false;
char *OddciUtils::logFile;

string OddciUtils::itoa(int number) {
    stringstream ss; //create a stringstream
    ss << number; //add number to the stream
    return ss.str(); //return a string with the contents of the stream
}

string OddciUtils::dtoa(double number) {
    stringstream ss; //create a stringstream
    ss << number; //add number to the stream
    return ss.str(); //return a string with the contents of the stream
}

int OddciUtils::atoi(string str) {
    int numb;
    istringstream(str) >> numb;
    return numb;
}

int OddciUtils::atoi(char *array) {
    string str(array);
    int numb;
    istringstream(str) >> numb;
    return numb;
}

vector<int> OddciUtils::getIntsFromString(const char* grouped) {
    string str(grouped);
    string buf; // Have a buffer string
    stringstream ss(str); // Insert the string into a stream

    vector<int> tokens; // Create vector to hold our words

    while (ss >> buf) {
        int i = OddciUtils::atoi(buf);
        tokens.push_back(i);
    }
    return tokens;
}

vector<char *> OddciUtils::getCharsFromSplitedChars(const char* chars) {
    string str(chars);
    string buf; // Have a buffer string
    stringstream ss(str); // Insert the string into a stream

    vector<char *> tokens; // Create vector to hold our words

    while (ss >> buf) {
        char * time = new char[buf.size() + 1];
        strcpy(time, buf.c_str());
        tokens.push_back(time);
    }
    return tokens;
}

vector<string> OddciUtils::getStringsFromSplitedChars(const char* chars) {
    string buf; // Have a buffer string
    stringstream ss(chars); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf) {
        tokens.push_back(buf);
    }
    return tokens;
}

void OddciUtils::print(vector<string> vetor) {
    if (logActivated) {
        if (init) {
            out.open(logFile);
            init = false;
        } else {
            out.open(logFile, ios_base::app);
        }
        for (vector<string>::iterator i = vetor.begin(); i != vetor.end();
                i++) {
            out << (*i).c_str() << " ";
        }
        out << "\n";
        out.close();
    }
}

string OddciUtils::getDurationFromSecondsWithDots(double seconds) {

    string durationView;
    string secs = "0";
    string min = "0";
    string hours = "0";

    // NumberFormat format = new DecimalFormat("00");

    int minInt = (int) seconds / 60;
    int secsInt = (int) seconds % 60;
    secs = itoa(secsInt);
    int hoursInt = minInt / 60;
    minInt = minInt % 60;
    min = itoa(minInt);
    hours = itoa(hoursInt);
    durationView = hours + ":" + min + ":" + secs;
    return durationView;
}

string OddciUtils::getDurationFromSeconds(double seconds) {

    string durationView;
    string secs = "0s";
    string min = "0min";
    string hours = "0h";
    string days = "0d";

    // NumberFormat format = new DecimalFormat("00");

    int minInt = (int) seconds / 60;
    int secsInt = (int) seconds % 60;
    secs = itoa(secsInt) + "s";

    if (minInt >= 60) {
        int hoursInt = minInt / 60;
        minInt = minInt % 60;
        min = itoa(minInt) + "min";
        if (hoursInt >= 24) {
            int daysInt = hoursInt / 24;
            days = itoa(daysInt) + "d";
            hoursInt = hoursInt % 24;
            hours = itoa(hoursInt) + "h";
            durationView = days + hours + min + secs;
        } else {
            hours = itoa(hoursInt) + "h";
            durationView = hours + min + secs;
        }

    } else {
        if (minInt > 0) {
            min = itoa(minInt) + "min";
            durationView = min + secs;
        } else {
            durationView = secs;
        }
    }
    return durationView;
}

void OddciUtils::stop(string message) {
    char msg[500];
    sprintf(msg, "%s '%s' &", "sudo gedit", message.c_str());
    system(msg);
}

void OddciUtils::setLogActive(bool logActivated) {
    OddciUtils::logActivated = logActivated;
}

bool OddciUtils::isLogActive() {
    return OddciUtils::logActivated;
}

char * OddciUtils::getLogFile() {
    return logFile;
}

void OddciUtils::setLogFile(char *logFileArg) {
    logFile = logFileArg;
}

string OddciUtils::convertLocalNumber(double number) {
    // Conversion code:
    ostringstream Convert;
    locale MyLocale(locale(), new WithComma); // Crate customized locale
    Convert.imbue(MyLocale); // Imbue the custom locale to the stringstream
    Convert << fixed << setprecision(5) << number; // Use some manipulators
    string result = Convert.str(); // Give the result to the string
    return result;
    // result is now equal to "0,120"
}

