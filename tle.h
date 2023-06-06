#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <curl/curl.h>

#define CELESTRACK_URL "http://celestrak.org/NORAD/elements/gp.php?CATNR=%s&FORMAT=TLE" 
#define CELESTRACK_URL_C "http://celestrak.org/NORAD/elements/gp.php?CATNR=25544&FORMAT=TLE"

// TODO: make a function to acquire NORAD catalog number to fetch API data (add to database of satellites we are tracking?) 
    /* within that -> TODO: get user input through front end*/
    /* use libcurl to make request (that is what gpredict uses) */
    
// TODO: make a function to make API call then parse and store TLE data

// TODO: figure out if we need a database to store NORAD catalog numbers 

// TODO: make function to delete catalog number from database

/* Two-line-element satellite orbital data */
// typedef struct
// {
//   double
// 	epoch, xndt2o, xndd6o, bstar,
// 	xincl, xnodeo, eo, omegao, xmo, xno;
//   int
// 	catnr,  /* Catalogue Number  */
// 	elset,  /* Element Set       */
// 	revnum; /* Revolution Number */
//   char
// 	sat_name[25], /* Satellite name string    */
// 	idesg[9];     /* International Designator */
// } tle_t;

typedef struct {
    const char* line0;
    const char* line1;
    const char* line2;
} TLEData;


// typedef struct {

//     char name[25];
//     // int line_number;
//     int catalog_number;
//     // char classification;
//     int international_designator_launch_year;
//     int international_designator_launch_number;
//     // char international_designator_launch_piece[3];
//     int epoch_year;
//     double epoch_day;
//     double first_time_derivative;
//     double second_time_derivative;
//     double bstar;
//     // int ephemeris;
//     // int element_set_type;
//     // int element_number;
//     // int checksum_line1;
//     double inclination;
//     double right_ascension;
//     int eccentricity;
//     double argument_of_perigee;
//     double mean_anomaly;
//     double mean_motion;
//     int revolution_number;
//     int checksum_line2;
// } TLEData;

// typedef struct {
//   ///Satellite number (line 1, field 2)
//   int satellite_number;
//   ///Element number (line 1, field 13)
//   long element_number;
//   ///International designator (line 1, fields 4, 5, 6)
//   char designator[10];
//   ///Epoch year (last two digits) (line 1, field 7)
//   int epoch_year;
//   ///Epoch day (day of year and fractional portion of day, line 1, field 8)
//   double epoch_day;
//   ///Inclination (line 2, field 3)
//   double inclination;
//   ///Right Ascension of the Ascending Node [Degrees] (line 2, field 4)
//   double right_ascension;
//   ///Eccentricity (line 2, field 5)
//   double eccentricity;
//   ///Argument of Perigee [Degrees] (line 2, field 6)
//   double argument_of_perigee;
//   ///Mean Anomaly [Degrees] (line 2, field 7)
//   double mean_anomaly;
//   ///Mean Motion [Revs per day] (line 2, field 8)
//   double mean_motion;
//   ///First Time Derivative of the Mean Motion divided by two (line 1, field 9)
//   double derivative_mean_motion;
//   ///Second Time Derivative of Mean Motion divided by six (line 1, field 10)
//   double second_derivative_mean_motion;
//   ///BSTAR drag term (decimal point assumed, line 1, field 11)
//   double bstar_drag_term;
//   ///Number of revolutions around Earth at epoch (line 2, field 9)
//   int revolutions_at_epoch;
//   ///Ephemeris data structure pointer
//   void *ephemeris_data;
// } predict_orbital_elements_t;


typedef struct {
	char sat_name [12];
	char idesg [20];
    int sat_num;
    char class;
    char* launch_yr;
    int launch_num;
    char* launch_piece;
    char* epoch_yr;
    double epoch_day;
    double d_mean_motion;
    double dd_mean_motion;
    double drag;
    char ephemeris;
    int element_num;
    double inclination;
    double r_node_ascension;
    double eccentricity;
    double perigee_arg;
    double mean_anomaly;
    double rev_per_day;
    int rev_num;
} tle_t;

/* Geodetic position s     tructure */
typedef struct
{
  double
	lat, lon, alt, theta;
} geodetic_t;

/* General three-dimensional vector structure */
typedef struct
{
  double
	x, y, z, w;
} vector_t;

// int set_cat_num(char cat_num []);
// int get_tle(char url [], tle_t *tle);
int set_cat_num(char cat_num [], TLEData *tle);
int get_tle(char url [], TLEData *tle);