#include <iostream>
#include <string>
#include <curl/curl.h>

// Funció de callback per escriure les dades de la resposta en un buffer (std::string)
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    // Convertim els continguts a std::string i els afegim al buffer
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    
    // URL a la qual farem la petició
    std::string url = "https://query1.finance.yahoo.com/v1/finance/search?q=PepsiCo&lang=es-ES&region=ES&quotesCount=6&newsCount=4&enableFuzzyQuery=false&quotesQueryId=tss_match_phrase_query&multiQuoteQueryId=multi_quote_single_token_query&newsQueryId=news_cie_vespa&enableCb=true&enableNavLinks=true&enableEnhancedTrivialQuery=true&enableCulturalAssets=true&enableLogoUrl=true";
    url = "https://query1.finance.yahoo.com/v1/finance/search?q=Vanguard+Short-Term+Treasury+Index+Fund&lang=es-ES&region=ES&quotesCount=6&newsCount=4&enableFuzzyQuery=false&quotesQueryId=tss_match_phrase_query&multiQuoteQueryId=multi_quote_single_token_query&newsQueryId=news_cie_vespa&enableCb=true&enableNavLinks=true&enableEnhancedTrivialQuery=true&enableCulturalAssets=true&enableLogoUrl=true";

    // Buffer per guardar la resposta
    std::string responseBuffer;
    
    // Inicialitzem curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
        // Estableix la URL per a la petició
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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

    // Allibera els recursos globals de curl
    curl_global_cleanup();
    
    return 0;
}
