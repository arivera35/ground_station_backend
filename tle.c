#include "tle.h"

int set_cat_num(char cat_num [], TLEData *tle){
    char buffer [1000] = CELESTRACK_URL;
    snprintf(buffer, sizeof(CELESTRACK_URL)+4, CELESTRACK_URL, cat_num);
    return get_tle(buffer, tle);
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    TLEData *tle_data = (TLEData *)userp;
    char *tle_string = malloc(total_size + 1);
    memcpy(tle_string, contents, total_size);
    tle_string[total_size] = '\0';

    // Extract line1 and line2 from the TLE string
    char* line0 = strtok(tle_string, "\n");
    char* line1 = strtok(NULL, "\n");
    char* line2 = strtok(NULL, "\n");

    // Store the TLE lines in the TLEData structure
    tle_data->line0 = strdup(line0);
    tle_data->line1 = strdup(line1);
    tle_data->line2 = strdup(line2);

    free(tle_string);

    return total_size;
}


// size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp){
//     size_t total_size = size * nmemb;
//     TLEData *tle_data = (TLEData *)userp;
//     // Convert the received data to a string
//     char *tle_string = malloc(total_size + 1);
//     memcpy(tle_string, contents, total_size);
//     tle_string[total_size] = '\0';
//     // Parse the TLE string and populate the TLEData object
//     sscanf(tle_string, "%24[^\n]\n%*d %5d%*c %2d%3d%*s %2d%12lf %9lf%lf %*d %*d%*d",
//            tle_data->name, 
//            &tle_data->catalog_number, 
//            &tle_data->international_designator_launch_year,
//            &tle_data->international_designator_launch_number, 
//            &tle_data->epoch_year,
//            &tle_data->epoch_day, 
//            &tle_data->first_time_derivative,
//            &tle_data->second_time_derivative, 
//            &tle_data->bstar
//            );
//     sscanf(tle_string + 106, "%lf %lf %7lf %lf %lf %11lf%5d%*d",
//            &tle_data->inclination, &tle_data->right_ascension, &tle_data->eccentricity,
//            &tle_data->argument_of_perigee, &tle_data->mean_anomaly,
//            &tle_data->mean_motion, &tle_data->revolution_number);
//     // Free the temporary TLE string
//     free(tle_string);

//     // Return the number of bytes handled by this callback
//     return total_size;
// }


int get_tle(char url [], TLEData *tle){
    // initializing curl pointer
    CURL *curl = curl_easy_init();
    if (!curl){
        printf("Error initializing curl\n");
        return -1;
    }
    // set options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, tle);
    // perform action
    CURLcode result = curl_easy_perform(curl);
    if (result > 0 | result > 0){
        printf("Error downloading data\n");
        return -1;
    }
    curl_easy_cleanup(curl);
    return 1;
} 