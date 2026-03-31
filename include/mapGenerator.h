#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  mapGenerator.h
//  AI Map Generator klijent za LaserTankClone
//
//  Zavisnosti:
//    - libcurl  (CURL/ folder + -lcurl u Makefile-u)  ← već imaš
//    - NI JEDNA dodatna biblioteka nije potrebna
//
//  Upotreba:
//    #include "mapGenerator.h"
//    MapGeneratorClient gen;
//    GeneratedMap result = gen.generateMap("medium");
// ─────────────────────────────────────────────────────────────────────────────

#include <curl/curl.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// ─────────────────────────────────────────────────────────────────────────────
//  Rezultat generisanja mape
// ─────────────────────────────────────────────────────────────────────────────
struct GeneratedMap {
    bool        success     = false;
    std::string filename;       // npr. "level5.tmx"
    std::string difficulty;
    std::string errorMessage;

    // 16×16 grid tile kodova, direktno spreman za tileMap[][]
    std::vector<std::vector<int>> grid;
};

// ─────────────────────────────────────────────────────────────────────────────
//  Interni helpers – ručno parsiramo JSON bez biblioteka
// ─────────────────────────────────────────────────────────────────────────────
namespace mgdetail {

    // CURL write callback
    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
        std::string* buf = static_cast<std::string*>(userdata);
        buf->append(ptr, size * nmemb);
        return size * nmemb;
    }

    // Izvuci string vrijednost: "key": "value"
    static std::string extractString(const std::string& json, const std::string& key) {
        std::string search = "\"" + key + "\"";
        size_t kpos = json.find(search);
        if (kpos == std::string::npos) return "";

        size_t colon = json.find(':', kpos + search.size());
        if (colon == std::string::npos) return "";

        size_t q1 = json.find('"', colon + 1);
        if (q1 == std::string::npos) return "";

        size_t q2 = json.find('"', q1 + 1);
        if (q2 == std::string::npos) return "";

        return json.substr(q1 + 1, q2 - q1 - 1);
    }

    // Izvuci bool vrijednost: "key": true/false
    static bool extractBool(const std::string& json, const std::string& key, bool defaultVal = false) {
        std::string search = "\"" + key + "\"";
        size_t kpos = json.find(search);
        if (kpos == std::string::npos) return defaultVal;

        size_t colon = json.find(':', kpos + search.size());
        if (colon == std::string::npos) return defaultVal;

        size_t vpos = colon + 1;
        while (vpos < json.size() && (json[vpos] == ' ' || json[vpos] == '\n')) vpos++;

        if (json.substr(vpos, 4) == "true")  return true;
        if (json.substr(vpos, 5) == "false") return false;
        return defaultVal;
    }

    // Izvuci 16×16 grid iz:  "grid": [[11,11,...],[...],...]
    static std::vector<std::vector<int>> extractGrid(const std::string& json) {
        std::vector<std::vector<int>> grid;

        size_t gpos = json.find("\"grid\"");
        if (gpos == std::string::npos) return grid;

        // Preskoci do prve '[' (vanjski niz)
        size_t outer = json.find('[', gpos);
        if (outer == std::string::npos) return grid;

        size_t pos = outer + 1;

        while (pos < json.size()) {
            // Nađi početak unutrašnjeg niza
            size_t rowStart = json.find('[', pos);
            if (rowStart == std::string::npos) break;

            size_t rowEnd = json.find(']', rowStart);
            if (rowEnd == std::string::npos) break;

            // Parsiraj brojeve između [ i ]
            std::string rowStr = json.substr(rowStart + 1, rowEnd - rowStart - 1);
            std::vector<int> row;
            std::stringstream ss(rowStr);
            std::string token;

            while (std::getline(ss, token, ',')) {
                // Ukloni whitespace
                token.erase(
                    std::remove_if(token.begin(), token.end(),
                        [](char c){ return c==' '||c=='\n'||c=='\r'||c=='\t'; }),
                    token.end()
                );
                if (!token.empty()) {
                    try   { row.push_back(std::stoi(token)); }
                    catch (...) { row.push_back(1); } // fallback: walkable ground
                }
            }

            if (!row.empty()) grid.push_back(row);

            pos = rowEnd + 1;

            // Provjeri da li smo izašli iz vanjskog niza
            size_t next = json.find_first_not_of(" \n\r\t,", pos);
            if (next != std::string::npos && json[next] == ']') break;
        }

        return grid;
    }

    // Napravi JSON request body: {"difficulty":"medium"}
    static std::string makeRequestBody(const std::string& difficulty) {
        return "{\"difficulty\":\"" + difficulty + "\"}";
    }

} // namespace mgdetail

// ─────────────────────────────────────────────────────────────────────────────
//  MapGeneratorClient
// ─────────────────────────────────────────────────────────────────────────────
class MapGeneratorClient {
public:
    // host i port Node.js servera (default: localhost:5000)
    MapGeneratorClient(const std::string& host = "localhost", int port = 5000)
        : m_baseUrl("http://" + host + ":" + std::to_string(port)) {}

    // ─────────────────────────────────────────────────────────────────────────
    //  generateMap()
    //  Poziva POST /generate-map, parsira odgovor, vraća GeneratedMap.
    //  Uvijek provjeri .success prije upotrebe .grid!
    // ─────────────────────────────────────────────────────────────────────────
    GeneratedMap generateMap(const std::string& difficulty = "medium") {
        GeneratedMap result;
        result.difficulty = difficulty;

        if (difficulty != "easy" && difficulty != "medium" && difficulty != "hard") {
            result.errorMessage = "difficulty mora biti: easy, medium ili hard";
            std::cerr << "[MapGenerator] " << result.errorMessage << "\n";
            return result;
        }

        std::string body = mgdetail::makeRequestBody(difficulty);
        std::string responseBody;
        long httpStatus = 0;

        if (!httpPost(m_baseUrl + "/generate-map", body, responseBody, httpStatus)) {
            result.errorMessage = "CURL greska – provjeri da li server radi na " + m_baseUrl;
            std::cerr << "[MapGenerator] " << result.errorMessage << "\n";
            return result;
        }

        if (httpStatus != 200) {
            result.errorMessage = "Server vratio HTTP " + std::to_string(httpStatus);
            std::cerr << "[MapGenerator] " << result.errorMessage << "\n";
            return result;
        }

        // Parsiraj odgovor
        result.success    = mgdetail::extractBool(responseBody, "success");
        result.filename   = mgdetail::extractString(responseBody, "filename");
        result.difficulty = mgdetail::extractString(responseBody, "difficulty");
        result.grid       = mgdetail::extractGrid(responseBody);

        if (!result.success) {
            result.errorMessage = mgdetail::extractString(responseBody, "error");
            if (result.errorMessage.empty())
                result.errorMessage = "Server prijavio neuspjeh bez poruke";
            std::cerr << "[MapGenerator] " << result.errorMessage << "\n";
            return result;
        }

        // Validacija dimenzija
        if (result.grid.size() != 16) {
            result.success = false;
            result.errorMessage = "Neispravan grid: " + std::to_string(result.grid.size()) + " redova (ocekivano 16)";
            std::cerr << "[MapGenerator] " << result.errorMessage << "\n";
            return result;
        }
        for (size_t i = 0; i < result.grid.size(); i++) {
            if (result.grid[i].size() != 16) {
                result.success = false;
                result.errorMessage = "Red " + std::to_string(i) + " ima "
                                    + std::to_string(result.grid[i].size()) + " kolona (ocekivano 16)";
                std::cerr << "[MapGenerator] " << result.errorMessage << "\n";
                return result;
            }
        }

        return result;
    }

    // ─────────────────────────────────────────────────────────────────────────
    //  listMaps() – vraća listu .tmx fajlova sa servera
    // ─────────────────────────────────────────────────────────────────────────
    std::vector<std::string> listMaps() {
        std::vector<std::string> maps;
        std::string responseBody;
        long httpStatus = 0;

        if (!httpGet(m_baseUrl + "/maps", responseBody, httpStatus) || httpStatus != 200) {
            std::cerr << "[MapGenerator] Ne mogu dohvatiti listu mapa\n";
            return maps;
        }

        // Parsiraj sve stringove koji završavaju na .tmx
        size_t pos = 0;
        while (pos < responseBody.size()) {
            size_t q1 = responseBody.find('"', pos);
            if (q1 == std::string::npos) break;
            size_t q2 = responseBody.find('"', q1 + 1);
            if (q2 == std::string::npos) break;

            std::string val = responseBody.substr(q1 + 1, q2 - q1 - 1);
            if (val.size() > 4 && val.substr(val.size() - 4) == ".tmx")
                maps.push_back(val);

            pos = q2 + 1;
        }

        return maps;
    }

    // ─────────────────────────────────────────────────────────────────────────
    //  downloadMap() – preuzima .tmx sa servera i snima lokalno
    // ─────────────────────────────────────────────────────────────────────────
    bool downloadMap(const std::string& filename, const std::string& localPath) {
        std::string responseBody;
        long httpStatus = 0;

        if (!httpGet(m_baseUrl + "/maps/" + filename, responseBody, httpStatus) || httpStatus != 200) {
            std::cerr << "[MapGenerator] Ne mogu preuzeti: " << filename << "\n";
            return false;
        }

        std::ofstream f(localPath, std::ios::binary);
        if (!f.is_open()) {
            std::cerr << "[MapGenerator] Ne mogu otvoriti: " << localPath << "\n";
            return false;
        }

        f << responseBody;
        return true;
    }

private:
    std::string m_baseUrl;

    bool httpPost(const std::string& url,
                  const std::string& body,
                  std::string& responseOut,
                  long& statusOut)
    {
        CURL* curl = curl_easy_init();
        if (!curl) return false;

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");

        curl_easy_setopt(curl, CURLOPT_URL,            url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,     headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,     body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,  (long)body.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  mgdetail::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &responseOut);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT,        90L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL,       1L);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusOut);
        else
            std::cerr << "[MapGenerator] CURL greska: " << curl_easy_strerror(res) << "\n";

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return (res == CURLE_OK);
    }

    bool httpGet(const std::string& url,
                 std::string& responseOut,
                 long& statusOut)
    {
        CURL* curl = curl_easy_init();
        if (!curl) return false;

        curl_easy_setopt(curl, CURLOPT_URL,            url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  mgdetail::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &responseOut);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT,        30L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL,       1L);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusOut);
        else
            std::cerr << "[MapGenerator] CURL greska: " << curl_easy_strerror(res) << "\n";

        curl_easy_cleanup(curl);
        return (res == CURLE_OK);
    }
};
