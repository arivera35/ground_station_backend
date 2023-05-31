#include "tle.h"

int set_cat_num(char cat_num [], tle_t *tle){
    char buffer [1000] = CELESTRACK_URL;
    snprintf(buffer, sizeof(CELESTRACK_URL)+4, CELESTRACK_URL, cat_num);
    printf("URL: %s\n", buffer);
    return get_tle(buffer, tle);
}

size_t write_to_tle(void *ptr, size_t size, size_t nmemb, tle_t tle){
    printf("In parser...\n");
    size_t num_bytes = size * nmemb;
    char *line = (char *)ptr;
    printf("Line: %c\n", line[3]);

    if (line[0] == '1'){
        printf("Doing something...\n");    
    }
    return num_bytes;
}

int get_tle(char url [], tle_t *tle){
    // initializing curl pointer
    CURL *curl = curl_easy_init();
    if (!curl){
        printf("Error initializing curl\n");
        return -1;
    }
    // set options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_tle);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, tle);
    // perform action
    CURLcode result = curl_easy_perform(curl);
    if (result > 0 | result > 0){
        printf("Error downloading data\n");
        return -1;
    }
    curl_easy_cleanup(curl);
    return 1;
}