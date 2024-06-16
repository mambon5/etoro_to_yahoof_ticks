
#include "stock_utils.h"
#include "matrix_utils.h"
#include "curl_utils.hpp"
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

string file = "../input/etoro_stocks.csv";
string lastCompanyFile = "../input/lastCompany.txt";
string outputFile = "../output/yfin_etoroTicks.csv";
string outLogs = "../output/outLogs.txt";

string set_yfin_curl(string const & pre_ticker_info) {
    string ticker_info;
    ticker_info = replaceChars(pre_ticker_info, ' ', '+');
    string yfin_command="https://query1.finance.yahoo.com/v1/finance/search?q="+ticker_info+"&lang=es-ES&region=ES&quotesCount=6&newsCount=4&enableFuzzyQuery=false&quotesQueryId=tss_match_phrase_query&multiQuoteQueryId=multi_quote_single_token_query&newsQueryId=news_cie_vespa&enableCb=true&enableNavLinks=true&enableEnhancedTrivialQuery=true&enableCulturalAssets=true&enableLogoUrl=true";
    // we got this info
    // from the yahoo finance webpage form, input places

    return yfin_command;
}



string yfin_response_buffer(string const & pre_ticker_info) {
    string url = set_yfin_curl(pre_ticker_info);
    
    cout << "url" << endl << url << std::endl;
    CURL* curl = curl_easy_init();
    std::string responseBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // set curl options and headers
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Write result into the buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
        
        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;


        // Cleanup
        curl_easy_cleanup(curl);
    }
    else {
        cout << "Curl could not be performed." << endl;
    }

    return responseBuffer;
}

string etoro_tick_to_yahoo(string const & pre_ticker_info) {
    string response = yfin_response_buffer(pre_ticker_info);
    size_t pos = 0;
    
    pos = response.find("symbol", pos); // start of the table of interest
    size_t endPos = response.find("index", pos); // start of the table of interest
    string symbol = response.substr(pos+9, endPos - pos-3-9);
    if(symbol.length() > 15 ) symbol="not_found";
    return symbol;
}

string deleteWeirdChars(string & oldName) {
    string newName = replaceChars(oldName, '/', 'x',true);
    return newName;
     
}

void all_etoro_to_yahoo(vector<string> const & compaNames, string const & outfile) {
    int n = LenghtOfVectorStr(compaNames);
    vector<string> yahooTicks;
    cout << n << " company names from etoro are being converted to a Yahoo symbol" << endl;
    for(string compaName : compaNames) {
        string lastCompany = GetFirstLineOfFile(lastCompanyFile);// get last company that was translated to yfin tick
        int posCompa = findPos(compaNames, compaName);
        int posLastUsed = findPos(compaNames, lastCompany);

        if(posCompa > posLastUsed) { // only work if we have not translated this company name already
            string newCompa = deleteWeirdChars(compaName);
            string ytick = etoro_tick_to_yahoo(newCompa);
            cout << "initial info about the company from etoro: " << newCompa << endl;
            cout << "symbol yfin: "  << ytick << endl;
            WriteToFileSimple(ytick, outfile); // write the ytick to an output file
            WriteToFileOver(compaName, lastCompanyFile); // keep track of the last company we analysed
            WriteToFileSimple("company name:" + newCompa + " and yfin: " + ytick, outLogs);
        }
        
    }
    
}

int main() {
    vector<string> etoro_compaNames;
    etoro_compaNames = extractNthColumnFromCsvString(file, 1, ';', true);
    // yahoo finance only allows around 600 requests per 3 min.

    for(int i=0; i<14; ++i) {
        int translationsChunck = 300;
        int sleepMinutes= 10;

        WriteToFileSimple("doing the first " + to_string(i) + " " +to_string(translationsChunck)+"  translations", outLogs);
        string lastCompany = GetFirstLineOfFile(lastCompanyFile);
        int posLastUsed = findPos(etoro_compaNames, lastCompany);
        cout << "posLastUsed: " << posLastUsed << endl;
        if(posLastUsed+translationsChunck > LenghtOfVectorStr(etoro_compaNames)) {
            //basically, if the chunck would make a segmentation fault, shorten it to fit 
            //the remaining elements of the array
            translationsChunck = posLastUsed+translationsChunck-LenghtOfVectorStr(etoro_compaNames) ;
        }
        vector<string> part(etoro_compaNames.begin()+ posLastUsed, 
                        etoro_compaNames.begin() + posLastUsed+translationsChunck);
        
        all_etoro_to_yahoo(part, outputFile);        

        // check if we reached the end
        if(LenghtOfVectorStr(etoro_compaNames) <= posLastUsed+translationsChunck) {
            cout << "We finished the analysis." << endl;
            return 0;
        }

        WriteToFileSimple("sleeping for "+to_string(sleepMinutes)+" min ...", outLogs);
        sleep(sleepMinutes*60);// wait 8 min, i think the allowed rate is around 100 requests per minute.
    }
    

    
}

