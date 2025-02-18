
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

string data_exec = "02_2025";
string file = "../input/etoro_stocks_"+data_exec+".csv";
string lastCompanyFile = "../input/lastCompany.txt";
string outputFile = "../output/yfin_etoroTicks"+data_exec+".csv";
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
    CURL* curl;
    CURLcode res;
    
    // URL a la qual farem la petició
    string url = set_yfin_curl(pre_ticker_info);
    
    // Buffer per guardar la resposta
    std::string responseBuffer;  
    
    cout << "url" << endl << url << std::endl;

    // url = "https://query1.finance.yahoo.com/v1/finance/search?q=PepsiCo&lang=es-ES&region=ES&quotesCount=6&newsCount=4&enableFuzzyQuery=false&quotesQueryId=tss_match_phrase_query&multiQuoteQueryId=multi_quote_single_token_query&newsQueryId=news_cie_vespa&enableCb=true&enableNavLinks=true&enableEnhancedTrivialQuery=true&enableCulturalAssets=true&enableLogoUrl=true";
    
    // Inicialitzem curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Estableix la URL per a la petició
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        cout << "url2 " << endl << url << std::endl;
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");

        
        // Estableix la funció callback per processar la resposta
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
        
        // Realitza la petició
        res = curl_easy_perform(curl);
        
        // Comprova si la petició ha estat correcta
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() fallat: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Si la petició ha estat exitosa, mostrem la resposta
            std::cout << "Resposta de la petició:\n" << responseBuffer << std::endl;
        }
        
        // Allibera els recursos de curl
        curl_easy_cleanup(curl);

    }
    else {
        cout << "Curl could not be performed." << endl;
    }

    return responseBuffer;
}

string etoro_tick_to_yahoo(string const & pre_ticker_info) {
    cout << "pre ticker info: " << pre_ticker_info << endl;
    string response = yfin_response_buffer(pre_ticker_info);
    size_t pos = 0;
    cout << "response:" << endl;
    cout << response << endl;
    pos = response.find("symbol", pos); // start of the table of interest
    cout << "begin pos: " << pos << endl;
    size_t endPos = response.find(",", pos); // start of the table of interest
    cout << "end pos: " << endPos << endl;
    string symbol = response.substr(pos+9, endPos - pos-1-9);
    if(symbol.length() > 15 ) symbol="not_found";
    else cout << "symbol found!" << endl;
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
    int translationsChunck = 300;
    int sleepMinutes= 10; // temps d'espera entre chunk i chunk
    
    string lastCompany = GetFirstLineOfFile(lastCompanyFile);
    int posLastUsed = findPos(etoro_compaNames, lastCompany);
    int i = 0;

    while(true) {        
        ++i;
        WriteToFileSimple("doing the first " + to_string(i) + " " +to_string(translationsChunck)+"  translations", outLogs);
        lastCompany = GetFirstLineOfFile(lastCompanyFile);
        posLastUsed = findPos(etoro_compaNames, lastCompany);
        cout << "posLastUsed: " << posLastUsed << endl;
        if(posLastUsed+translationsChunck > LenghtOfVectorStr(etoro_compaNames)) {
            //basically, if the chunck would make a segmentation fault, shorten it to fit 
            //the remaining elements of the array
            translationsChunck = posLastUsed+translationsChunck-LenghtOfVectorStr(etoro_compaNames) ;
        }
        else if(lastCompany == "") posLastUsed = 0; // if we have empty file, it means start from first company on
        cout << "no peta" << endl;
        vector<string> part(etoro_compaNames.begin()+ posLastUsed, 
                        etoro_compaNames.begin() + posLastUsed+translationsChunck);
        cout << "si peta" << endl;
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

