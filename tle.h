#include <stdio.h>
#include <string.h>

typedef struct {
    int sat_num;
    float bstar_drag_term;
    float inclination;
    float eccentricity;
    float perigee;
    float mean_anomaly;
    float mean_motion;
}tle_t;