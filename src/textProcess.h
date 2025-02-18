using namespace std;
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
//#include <string.h> // dont use string.h, use cstring instead
#include <cstring>
#include <dirent.h>
#include <sys/types.h>
#include <iomanip>      // std::setprecision


#include <curl/curl.h>
// compile by the command: g++ yh_download_data.cpp -o prova -lcurl
// hint: Yahoo data won't show stocks if the list is greater than 9000. Trick: inspect this 
// list by pieces.
// this guy got a list of all US stock symbols, apparently: https://github.com/rreichel3/US-Stock-Symbols/tree/main
// it is worth checking it out



string PrintNumberWithXDecimalsDoub(const double& number, const int& precision) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << number;
    string rounded = ss.str();
    return rounded;
}

bool compareStrings(const std::string& str1, const std::string& str2) {
  return str1 < str2;
}

void sortarray(vector<std::string>& arrayDeStrings) {
    // Ejemplo de un array de strings
    // std::vector<std::string> arrayDeStrings = {"baababa","zzz", "abc", "aaaaa", "xyz", "def"};

    // Ordenar el array utilizando la función compareStrings como criterio de ordenamiento
    std::sort(arrayDeStrings.begin(), arrayDeStrings.end(), compareStrings);

    // Imprimir el array ordenado
    // std::cout << "Array ordenado:" << std::endl;
    // for (const auto& str : arrayDeStrings) {
    //     std::cout << str << " ";
    // }

}

class sort_indices
{
    // sorts the indices instead of the array!
    // how to use it:
    // int arr[5]={4,1,3,6,2}
    // string arr1[5]={"a1","b1","c1","d1","e1"};
    // int indices[5]={0,1,2,3,4};
    // arr[indices[0]]
    // std::sort(indices, indices+5, sort_indices(arr));
    //
   private:
     int* mparr;
   public:
     sort_indices(int* parr) : mparr(parr) {}
     bool operator()(int i, int j) const { return mparr[i]<mparr[j]; }
};

string DownloadWebBody(const string& url) {
    CURL *curl = curl_easy_init();
    string webContent;
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &webContent);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);     
        if (res != CURLE_OK) {
            cerr << "Failed to download web content: " << curl_easy_strerror(res) << endl;
            webContent.clear();
        }   
    }
    
    return webContent;
}

// Define the write callback function
size_t WriteCallback(void* ptr, size_t size, size_t nmemb, string* userdata) {
    size_t totalSize = size * nmemb;
    userdata->append(static_cast<char*>(ptr), totalSize);
    return totalSize;
}


// Function to download content from a URL using libcurl
std::string DownloadWebContent(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string webContent;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &webContent);
        // cout << "all curl_easy_setopt() done efore easy_perform()" << endl;
        res = curl_easy_perform(curl);
        // cout << "curl_easy_perform() done" << endl;
        if (res != CURLE_OK) {
            cerr << "Failed to download web content: " << curl_easy_strerror(res) << endl;
            webContent.clear();
        }

        curl_easy_cleanup(curl);
    }
    std::cout << "DownloadWebContent() succeded" << endl;
    return webContent;
}

std::string getTableStocks(const std::string& body) {
    size_t pos = 0;
    std::string table;
    pos = body.find("lookup-table", pos); // start of the table of interest
    size_t endPos = body.find("<span>Next", pos); // end of the table of interest
    if(int(pos) < body.length() && int(endPos) < body.length()){
            table = body.substr(pos, endPos - pos);
    } else{
        cout << "ERROR in getTableStocks(): no stock data found for this url" << endl;
        table = "";
    }
    return table;
}

std::string DirectWebContent(std::string& searchGroup, std::string B, std::string  C) {
    std::string baseUrl = "https://finance.yahoo.com/lookup/all?s=";
    std::string url = baseUrl + searchGroup + "&t=A&b="+B+"&c="+C;
    std::string webContent = DownloadWebContent(url);
    return webContent;
}

// String filtering for the tickers:
bool StringInVector(const std::string& ticker, const vector<std::string>& tickers) {
    // return true if ticker in string vector
    return find(begin(tickers), end(tickers), ticker) != end(tickers);
}

void LenghtOfVector(const vector<std::string>& tickers) {
    int n = 0;
    for(std::string str : tickers) {
        n++;
    }
    cout << endl;
    cout << "length of vector is: " << to_string(n) << endl;

}

int LenghtOfVectorStr(const vector<string>& tickers) {
    int n = 0;
    for(string str : tickers) {
        n++;
    }
    return n;

}

int LenghtOfMatStr(const vector<vector<string>>& tickers) {
    int n = 0;
    for(vector<string> str : tickers) {
        n++;
    }
    return n;

}

int LenghtOfVectorInt(const vector<int>& tickers) {
    int n = 0;
    for(int str : tickers) {
        n++;
    }
    return n;

}

int LenghtOfVectorDoub(const vector<double>& tickers) {
    int n = 0;
    for(int str : tickers) {
        n++;
    }
    return n;

}


void OutputVector( const vector<std::string>& tickers) {
    int n = 0;
    for(std::string str : tickers) {
        cout << str << " - ";
        n++;
    }
    cout << endl;
    cout << "length of vector is: " << to_string(n) << endl;

}

void OutputVectorInt( const vector<int>& tickers) {
    for(int str : tickers) {
        cout << str << " - ";
    }
    cout << endl;
}

void OutputVectorDouble( const vector<double>& tickers) {
    for(double str : tickers) {
        cout << str << " - ";
    }
    cout << endl;
}

void Output2DVectorDouble( const vector<vector<double>>& matrix) {
    for (vector<double> vect : matrix) {
        for(double str : vect) {
            cout << str << " - ";
    }
        cout << endl;
    }
}

void Output2Dvector_custom1( const vector<vector<double>>& matrix, const vector<string>& strings) {
    for (vector<double> vect : matrix) {
        cout << vect[0] << " - " << vect[1] << " - " << strings[int(vect[2])] << endl;
    
    }
}

void Output2Dvector_firstFew( const vector<vector<double>>& matrix, const vector<string>& strings, int show=5) {
    for (vector<double> vect : matrix) {
        if(show==0) return;
        cout << vect[0] << " - " << vect[1] << " - " << strings[int(vect[2])] << endl;
        show --;
    }
}

void WriteToFileSimple( const std::string& output, const std::string& outputFile, bool printall=false) {
    ofstream outFile;
    outFile.open(outputFile, ios_base::app); // append instead of overwrite
    if (!outFile.is_open()) {
        cerr << "Failed to open the output file: " << outputFile << endl;
        return;
    }
    if(printall) cout << "writing output to ouput file: " << outputFile << endl;
    outFile << output << endl;
    outFile.close();
}

void WriteToFileOver( const std::string& output, const std::string& outputFile, bool printall=false) {
    ofstream outFile;
    outFile.open(outputFile, ios_base::trunc); // Obre el fitxer per escriptura, truncant el contingut existent
    if (!outFile.is_open()) {
        cerr << "Failed to open the output file: " << outputFile << endl;
        return;
    }
    if(printall) cout << "writing output to ouput file: " << outputFile << endl;
    outFile << output << endl;
    outFile.close();
}

void WriteToFile(const vector<std::string>& tickers, const std::string& outputFile, bool showVector=false) {
    ofstream outFile;
    bool first=true;
    outFile.open(outputFile, ios_base::app); // append instead of overwrite
    if (!outFile.is_open()) {
        cerr << "Failed to open the output file: " << outputFile << endl;
        return;
    }
    cout << "writing output to ouput file: " << outputFile << endl;
    cout << "there are " << size(tickers) << " tickers:" << endl;
    // int len = sizeof(tickers)/sizeof(tickers[0]);
    // tickers = sort(tickers.begin(), tickers.end());
    // Write tickers to the output file
    for (const std::string& ticker : tickers) {
        if(showVector) cout << ticker << ", ";
        if(first) { 
            outFile << ticker;
            first = false;
        }
        else outFile << "," << ticker;
    }
    outFile.close();
}

string ExtractDateFromFile(const string & filename) {
    // we assume the filename is of format "filename_yyyy-mm-dd.XXX". That's to say, the last 15 characters of the filename must be
    //"_yyyy-mm-dd.XXX"
    // returns date in format "yyyy-mm-dd"
    int n = filename.length();
    string date = filename.substr(n-14,10);
    return date;
}

bool TodayFileExists(const string & dirname, const string & filenameSuffix) {
    // check whether a file in directory "dirname" with file suffix filenameSuffix, containing the current date, exists or not
    string currentDate = getCurrentDate();
    string targetFile = filenameSuffix + currentDate + ".csv";
    
    DIR *dr;
    struct dirent *en;
    int files = 0;
    int lastn=1;
    dr = opendir(dirname.c_str()); //open the directory, convert first string to char
                                  // how to use this function: https://pubs.opengroup.org/onlinepubs/009604599/functions/opendir.html
    //    if(printAll) cout << "opening directory: " << dirname << endl;                               
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            string filename = en->d_name;
            
            if( targetFile == filename ) return true;
        }
    }
    else cout << "error while opening dir at TodayFileExists()" << endl;
    return false;
}


string removeNulls(const std::string& input) { // elimina caràcters nulls de l'string
    std::string result = input;
    result.erase(std::remove(result.begin(), result.end(), '\0'), result.end());
    return result;
}

string urlencode(const std::string &value) {// codifica caràcters extranys
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    string copy = removeNulls(value);

    for (unsigned char c : copy) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::setw(2) << int(c);
        }
    }

    return encoded.str();
}

// Funció per eliminar els elements que siguin 'not_found' o continguin el substring "nt":
void removeInvalidStrings(std::vector<std::string>& arr) {
    arr.erase(std::remove_if(arr.begin(), arr.end(), [](const std::string& s) {
        return s == "not_found" || s.find("nt\":") != std::string::npos;
    }), arr.end());
}

void printCharCodes(const string& str) {
    for (char c : str) {
        cout << "[" << int(c) << "] ";  // Mostra el codi ASCII de cada caràcter
    }
    cout << endl;
}

std::string removeTextAfterParenthesis(const std::string& input) {
    size_t pos = input.find('('); // Cerca el primer parèntesi
    if (pos != std::string::npos) {
        return input.substr(0, pos); // Retorna només la part abans del parèntesi
    }
    return input; // Si no troba cap parèntesi, retorna el string original
}

string trim(const string& str) {
    string str2 = removeNulls(str);
    str2 = removeTextAfterParenthesis(str2);
    size_t first = str2.find_first_not_of(" \t\r\n"); // Troba el primer caràcter no espai
    size_t last = str2.find_last_not_of(" \t\r\n");  // Troba l'últim caràcter no espai
    return (first == string::npos) ? "" : str2.substr(first, (last - first + 1));
}


void DeleteOlderFiles(const string & dirname, const string & filenameSuffix, const int & daysOld, bool printAll=true) {
    // This function: 
    //  1. gets the file names containing the filenameSuffix.
    //  2. Checks its date (which should be in format yyyy-mm-dd and be in the filename after the suffix)
    //  3. returns the files that are older than the given number of days daysOld


   DIR *dr;
   struct dirent *en;
   int files = 0;
   int lastn=1;
   dr = opendir(dirname.c_str()); //open the directory, convert first string to char
                                  // how to use this function: https://pubs.opengroup.org/onlinepubs/009604599/functions/opendir.html
   if(printAll) cout << "opening directory: " << dirname << endl;                               
   if (dr) {
      while ((en = readdir(dr)) != NULL) {
        string filename = en->d_name;
        // if(printAll) cout << filename << endl;
        int n = filename.length();

        if( filename.find(filenameSuffix) < n ) {
            if(printAll) cout << filename << endl;
            string date = ExtractDateFromFile(filename); //read the last 14 characters, but skip the last 4 since they are ".csv";
            if(printAll) cout << "reading date: " << date << endl;
            
            string currentDate = getCurrentDate();
            int dies = DiesEntreDates(date, currentDate);
            if(printAll) cout << "Han passat " << dies << " dies entre la data del fitxer i avui" << endl;
            
            if(daysOld < dies) {
                // delete file if it is older than daysOld days.
                string file = dirname + filename;
                int res = remove(file.c_str());
                if(res == 0) cout << "file " << filename << " removed successfully" << endl;
                else cout << "error while deleting the file " << filename << endl;
            }
        }
      }
      closedir(dr); //close the directory
      
   }

}

vector <std::string> getLastSearchGroup() {
    std::string line;
    std::string block;
    std::string filename = "last_save_ticks.txt";
    std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int size = sizeof(characters);
    int first;
    int second;

    ifstream file(filename);
    if (file.is_open()) {
        getline(file, line);
        getline(file, block);
        file.close();
        first = characters.find(line[0]);
        second = characters.find(line[1]);
    } else {
        cout << endl << "ERROR! Could not open file " + filename;
        line = "";
        first = 0;
        second = 0;
    }
    cout << endl << "Opened file for last tickers used, got this: " + line << endl;
    
    std::string characters1 = characters.substr(first, size - first);
    std::string characters2 = characters.substr(second, size - second);
    if (block=="") block="0";
    vector <std::string> vect;
    vect.push_back(line);
    vect.push_back(characters1);
    vect.push_back(characters2);
    vect.push_back(block);
    
    return vect;
}

int getLastFileNumber() {
   DIR *dr;
   struct dirent *en;
   int files = 0;
   int lastn=1;
   dr = opendir("."); //open all directory
   if (dr) {
      while ((en = readdir(dr)) != NULL) {
        //  cout<<" \n"<<en->d_name; //print all directory name
        std::string filename = en->d_name;
        int n = filename.length();


        if( filename.find(".csv") < n && filename.find("_") <  n) {
            filename = filename.substr(0, n-4);
            std::string number = filename.substr(filename.find("_")+1, filename.length()-1);
            if(stoi(number) > lastn) lastn = stoi(number);
            files = files + 1;
        }
      }
      closedir(dr); //close all directory
      
   }
   return lastn;
}


vector<double> extractNthColumnFromString(const string& text, const int & columna) {
    vector<double> columnData;
    
    stringstream file(text); // we need a stringstream to operated the getline functional
    string line;


    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        int columnIndex = 0;
        double value;
        
        while (getline(ss, item, ',')) {
            if (columnIndex == (columna-1)) {
                try {
                    value = stod(item);
                    columnData.push_back(value);
                } catch (const invalid_argument& e) {
                    cerr << "Error de conversió a double: " << item << endl;
                } catch (const out_of_range& e) {
                    cerr << "Valor fora de rang per a double: " << item << endl;
                }
                break;
            }
            ++columnIndex;
        }
    }

   
    return columnData;
}


vector<string> extractNthColumnFromCsvString(const string& filename, const int & columna, 
const char sep = ',', bool deleteFirstRow=false) {
    vector<string> columnData;
    
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "No s'ha pogut obrir el fitxer: " << filename << endl;
        return columnData;
    }
    string item;
    int columnIndex;

    while (getline(file, line)) {
        stringstream ss(line);
        columnIndex = 0;        
        
        while (getline(ss, item, sep)) {
            if (columnIndex == (columna-1)) {
                try {
                    
                    columnData.push_back(trim(item));
                } catch (const invalid_argument& e) {
                    cerr << "Error de conversió a string: " << item << endl;
                } catch (const out_of_range& e) {
                    cerr << "Valor fora de rang per a string: " << item << endl;
                }
                break;
            }
            ++columnIndex;
        }
    }

    file.close();

    if(deleteFirstRow) columnData.erase(columnData.begin()); // eliminem el primer element ja que no és el nom d'una companyia

 
    return columnData;
}

vector<vector<string>> readCsvToMatrix(const string& filename, const int & columns )
{
    vector<vector<string>>   result;
    string                line;
    ifstream file(filename);
    
    int index;
    while(getline(file,line)) {

        stringstream          lineStream(line);
        string                cell;
       
        index = 0;
        vector<string> liniaVec;
        while(getline(lineStream,cell, ','))
        {
                liniaVec.push_back(cell);
                index = index + 1; // hem llegit 1 columna
        }
        if(index == columns) {
            result.push_back(liniaVec); // si no llegim exactament el numero de columnes, no guardis la linia
        }
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty())
        {
            // If there was a trailing comma then add nothing
        }
    }
    return result;
}

vector<std::string> readCsv(const std::string& filename)
{
    vector<std::string>   result;
    std::string                line;
    ifstream file(filename);
    
    while(getline(file,line)) {

        stringstream          lineStream(line);
        std::string                cell;

        while(getline(lineStream,cell, ','))
        {
                result.push_back(cell);
     
        }
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty())
        {
            // If there was a trailing comma then add nothing
        }
    }
    return result;
}

string GetFirstLineOfFile(const std::string& inputFile, bool printall=false) {
    // obten la primera linia del fitxer.
    std::ifstream archivo(inputFile); // Abrir el archivo para lectura
    std::string primeraLinea = "";
    if (archivo.is_open()) {        
        if (std::getline(archivo, primeraLinea)) { // Leer la primera línea del archivo
            if(printall) std::cout << "La primera línea del archivo es: " << primeraLinea << std::endl;
        } else {
            std::cout << "GetFirstLineOfFile(): El archivo está vacío." << std::endl;
        }
        archivo.close(); // Cerrar el archivo después de su uso
    } else {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
    }
    return primeraLinea;
}

vector<std::string> readPartialCsvFromCertainLine(const std::string& filename, 
                const int& elems, const string firstTick, bool printOutput=false) {
    // elems són els elements a llegir
    // firstTick és la primera ticker a partir de la qual comença a llegir noms d'accions
    ifstream fitxer(filename);
    vector<std::string> elements;
    
    std::string tick;
    bool validTick=false;
    // for (int i=0; i<elems;++i) { 
    int i = 0;
    while(getline(fitxer, tick, ',')) {
        if(i==elems) break;

        if(tick == firstTick) {
                validTick=true;
                continue;
            }
        if(!validTick) {
            continue;
        }
        if (tick.length() > 2) elements.push_back(tick); // if the tick doesn't have at least 2 characters, it is not a real tick, it is an empty string or something else
    
        if(printOutput) cout << "ticker que hem llegit: " << tick << endl;

        i++;
    }
    
    fitxer.close();
    return(elements);
}

vector<std::string> readPartialCsv(const std::string& filename, const int& elems, bool printOutput=false) {
    // elems són els elements a llegir
    ifstream fitxer(filename);
    vector<std::string> elements;
    
    std::string tick;
    for (int i=0; i<elems;++i) { 
  
  
        // read an entire row and 
        // store it in a string variable 'line' 
        getline(fitxer, tick, ',');
        elements.push_back(tick);
  
       if(printOutput) cout << "ticker que hem llegit: " << tick << endl;
    }

    fitxer.close();
    return(elements);
}

vector<std::string> CsvFilterDuplicates(const std::string& filename)
{
    vector<std::string>   result;
    std::string                line;
    ifstream file(filename);
    int repeated = 0;
    
    while(getline(file,line)) {

        stringstream          lineStream(line);
        std::string                cell;

        while(getline(lineStream,cell, ','))
        {
            if(!StringInVector(cell, result)) {
                result.push_back(cell);
            }
            else {
                repeated++;
            }
        }
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty())
        {
            // If there was a trailing comma then add nothing
        }
    }
    cout << to_string(repeated) << " repetitions deleted." << endl;
    return result;
}

string replaceChars(string text, char const & oldChar, char const & newChar, bool elimina=false)
{   
    if(elimina) {
        text.erase(remove(text.begin(), text.end(), oldChar), text.end());
    }
    else {
        replace(text.begin(), text.end(), oldChar, newChar);
    }
    
    return text;
}

bool file_exists(const string fileName)
{
    // checks whether a file exists or not
    std::ifstream infile(fileName);
    return infile.good();
}