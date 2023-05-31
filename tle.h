#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
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
typedef struct
{
  double
	epoch, xndt2o, xndd6o, bstar,
	xincl, xnodeo, eo, omegao, xmo, xno;
  int
	catnr,  /* Catalogue Number  */
	elset,  /* Element Set       */
	revnum; /* Revolution Number */
  char
	sat_name[25], /* Satellite name string    */
	idesg[9];     /* International Designator */
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

int set_cat_num(char cat_num []);
int get_tle();