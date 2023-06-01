#include "tle.h"

int set_cat_num(char cat_num [], TLEData *tle){
    char buffer [1000] = CELESTRACK_URL;
    snprintf(buffer, sizeof(CELESTRACK_URL)+4, CELESTRACK_URL, cat_num);
    printf("URL: %s\n", buffer);
    return get_tle(buffer, tle);
}

// size_t write_to_tle(void *ptr, size_t size, size_t nmemb, tle_t tle){
//     printf("In parser...\n");
//     size_t num_bytes = size * nmemb;
//     char *line = (char *)ptr;
//     printf("Line: %c\n", line[3]);

//     if (line[0] == '1'){
//         printf("Doing something...\n");    
//     }
//     return num_bytes;
// }

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)

{

    size_t total_size = size * nmemb;

    TLEData *tle_data = (TLEData *)userp;

    // Convert the received data to a string
    char *tle_string = malloc(total_size + 1);

    memcpy(tle_string, contents, total_size);

    tle_string[total_size] = '\0';

    // Parse the TLE string and populate the TLEData object
    sscanf(tle_string, "%24[^\n]\n%d %5d%c %2d%3d%3c %2d%12lf %10lf %8lf %8lf %d %4d%d",
           tle_data->name, &tle_data->line_number, &tle_data->catalog_number,
           &tle_data->classification, &tle_data->international_designator_launch_year,
           &tle_data->international_designator_launch_number, tle_data->international_designator_launch_piece, &tle_data->epoch_year,
           &tle_data->epoch_day, &tle_data->first_time_derivative,
           &tle_data->second_time_derivative, &tle_data->bstar, &tle_data->ephemeris, &tle_data->element_number, &tle_data->checksum_line1);
    sscanf(tle_string + 69, "%lf %lf %lf %lf %lf %lf %d",
           &tle_data->inclination, &tle_data->right_ascension, &tle_data->eccentricity,
           &tle_data->argument_of_perigee, &tle_data->mean_anomaly,
           &tle_data->mean_motion, &tle_data->revolution_number);
    // Free the temporary TLE string
    free(tle_string);

    // Return the number of bytes handled by this callback
    return total_size;

}


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