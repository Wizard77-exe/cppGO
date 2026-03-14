#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

// curl write callback
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// fetch latest SpaceX launch
json fetchLatestLaunch() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.spacexdata.com/v4/launches/latest");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res == CURLE_OK) {
            try {
                return json::parse(readBuffer);
            } catch (json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return nullptr;
}

int main() {
    json launch = fetchLatestLaunch();

    if(!launch.is_null()) {
        std::cout << "=== Latest SpaceX Launch ===" << std::endl;

        std::string name = launch.value("name", "N/A");
        std::string date = launch.value("date_utc", "N/A");
        bool success = launch.value("success", false);
        auto payloads = launch.value("payloads", json::array());

        std::cout << "Name: " << name << std::endl;
        std::cout << "Date (UTC): " << date << std::endl;
        std::cout << "Success: " << (success ? "Yes" : "No") << std::endl;
        std::cout << "Number of payloads: " << payloads.size() << std::endl;
    } else {
        std::cerr << "Failed to fetch SpaceX launch data." << std::endl;
    }

    return 0;
}