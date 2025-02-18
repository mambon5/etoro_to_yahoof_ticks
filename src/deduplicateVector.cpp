#include "dates.h"
#include "matrix_utils.h"
#include "textProcess.h"

// #include "src/quote.hpp"
using namespace std;
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>
#include <string.h>

#include <dirent.h>

#include <istream>

// compile by the command: 
// g++ time_utils.cpp curl_utils.cpp quote.cpp spot.cpp transform_etoro.cpp -o trans_etoro -lcurl

string file = "../output/yfin_etoroTicks.csv";
string outputFile = "../output/yfin_etoroTicks_clean.csv";
string outLogs = "../output/dedupOutLogs.txt";

int main() {
    
    vector<string> original = readCsv(file);
    vector<string> dedup = CsvFilterDuplicates(file);
    
    cout << "We found " +to_string(LenghtOfVectorStr(original) - LenghtOfVectorStr(dedup)) + 
    " repetitions. Final vector has size: " + to_string(LenghtOfVectorStr(dedup)) << endl;

    WriteToFile(dedup, outputFile);

    
}

