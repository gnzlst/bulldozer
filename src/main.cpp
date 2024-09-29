#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

// Define PWM Channels
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;

// Define PWM Frequency and Resolution
const int freq = 5000;
const int resolution = 8;

// WiFi Credentials
const char* ssid = "vlad";
const char* password = "vlad12345";

// RFID Reader
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Function to set LED color
void setColor(int redValue, int greenValue, int blueValue) {
    // Implementation to set LED color
}

// LED flash functions
void ledFlashPass(int times, int delay) {
    std::cout << "LED Flash Pass: " << times << " times with " << delay << "ms delay." << std::endl;
}

void ledFlashFail(int times, int delay) {
    std::cout << "LED Flash Fail: " << times << " times with " << delay << "ms delay." << std::endl;
}

// Callback function to handle data received from the API
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to make API call
bool makeApiCall() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/todos/1");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return false;
        }

        // Parse JSON response
        Json::CharReaderBuilder readerBuilder;
        Json::Value jsonData;
        std::string errs;

        std::istringstream s(readBuffer);
        std::string errs;
        if (!Json::parseFromStream(readerBuilder, s, &jsonData, &errs)) {
            std::cerr << "Failed to parse JSON: " << errs << std::endl;
            return false;
        }

        // Check the value of the "completed" field
        return jsonData["completed"].asBool();
    }

    return false;
}

int main() {
    // Make API call
    bool apiResult = makeApiCall();

    // Call appropriate LED flash function based on API result
    if (apiResult) {
        ledFlashPass(5, 50);
    } else {
        ledFlashFail(20, 50);
    }

    return 0;
}