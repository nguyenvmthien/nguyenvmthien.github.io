#include <iostream>
#include <string>
#include <curl/curl.h>

class Crawler {
public:
    Crawler() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
    }

    ~Crawler() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }

    std::string fetchHTML(const std::string& url) {
        if (curl) {
            std::string response_data;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Response data will be stored here
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                return "";
            }

            return response_data;
        } else {
            std::cerr << "Failed to initialize libcurl." << std::endl;
            return "";
        }
    }

private:
    CURL* curl;

    static size_t writeCallback(char* contents, size_t size, size_t nmemb, std::string* data) {
        data->append(contents, size * nmemb);
        return size * nmemb;
    }
};

int main() {
    Crawler crawler;
    std::string url = "https://example.com"; // Replace with the desired URL
    std::string html = crawler.fetchHTML(url);

    if (!html.empty()) {
        std::cout << "HTML content of " << url << ":\n" << html << std::endl;
    }

    return 0;
}
