#include "chess_server.hpp"
#include <curl/curl.h>



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string chess_server_get_last_move() {
	CURL *curl;
	std::string read_buffer;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/last-move");
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return read_buffer;
}