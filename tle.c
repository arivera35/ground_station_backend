#include "tle.h"

int set_cat_num(char cat_num []){
    char buffer [1000] = CELESTRACK_URL;
    snprintf(buffer, sizeof(CELESTRACK_URL)+4, CELESTRACK_URL, cat_num);
    printf("URL: %s\n", buffer);
    get_tle(buffer);
    return 1;
}

size_t write_to_tle(void *ptr, size_t size, size_t nmemb, tle_t tle){
    size_t num_bytes = size * nmemb;
    char *line = (char *)ptr;

    if (line[0] == '1'){
        printf("Doind something...\n");
    }

    return num_bytes;

}

int get_tle(char url []){
    // initializing curl pointer
    CURL *curl = curl_easy_init();
    if (!curl){
        printf("Error initializing curl\n");
        return -1;
    }
    // set options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_tle);
    // perform action
    CURLcode result = curl_easy_perform(curl);
    if (result > 0 | result > 0){
        printf("Error downloading data\n");
        return -1;
    }
    curl_easy_cleanup(curl);
    return 1;
}