#include "dates.h"
#include "textProcess.h"
// #include "src/quote.hpp"
using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>


#include <istream>

int main() {
    vector<string> etoro_compaNames;
    etoro_compaNames = {"hooa", "adeu", "aigua", "adeu", "aigua"};
    // yahoo finance only allows around 600 requests per 3 min.

   
        vector<string> part(etoro_compaNames.begin()+1, 
                        etoro_compaNames.begin() + 4);
        
       
    OutputVector(part);
    
}

