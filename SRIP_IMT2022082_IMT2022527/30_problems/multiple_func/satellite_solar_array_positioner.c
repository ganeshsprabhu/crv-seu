// satellite_solar_array_positioner.c
// Controls the orientation of a satellite's solar arrays. Normal operation adjusts the array angle based on a sun-tracking algorithm and an ephemeris table (a CRV). A critical override is triggered by high battery temperature, which moves the arrays to a 'safe mode' off-sun orientation to reduce charging, ignoring the sun-tracking data.
// array_angle >= -90 && array_angle <= 90
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SAFE_MODE_ANGLE 90 // Perpendicular to sun
#define MAX_ANGLE 90
#define MIN_ANGLE -90
#define HIGH_BATTERY_TEMP_THRESHOLD 60 // Celsius

// Simulated hardware inputs
volatile int battery_temperature;
volatile int sun_position_angle; // (CRV candidate)

void read_satellite_sensors() {
    battery_temperature = 50 + (rand() % 15);
    sun_position_angle = (rand() % 181) - 90;
}

void log_array_angle(const char* reason, int angle) {
    printf("Reason: %-25s | Array Angle: %d degrees\n", reason, angle);
}

bool is_battery_overheating() {
    return battery_temperature > HIGH_BATTERY_TEMP_THRESHOLD;
}

int get_sun_tracking_angle() {
    log_array_angle("Sun Tracking", sun_position_angle);
    return sun_position_angle;
}

int apply_angle_limits(int angle) {
    if (angle > MAX_ANGLE) return MAX_ANGLE;
    if (angle < MIN_ANGLE) return MIN_ANGLE;
    return angle;
}

int step_control_logic() {
    int new_array_angle;
    if (is_battery_overheating()) {
        new_array_angle = SAFE_MODE_ANGLE;
        log_array_angle("BATTERY OVERHEAT SAFE-MODE", new_array_angle);
    } else {
        new_array_angle = get_sun_tracking_angle();
    }
    return apply_angle_limits(new_array_angle);
}

int main() {
    srand(time(NULL));
    int array_angle = 0;
    printf("--- Satellite Array Positioner Simulation (Multi Function) ---\n");

    for (int i = 0; i < 5; ++i) {
        read_satellite_sensors();
        array_angle = step_control_logic();
        
        if (array_angle < MIN_ANGLE || array_angle > MAX_ANGLE) {
            printf("!!! SAFETY VIOLATION: Array angle out of bounds !!!\n");
            return -1;
        }
    }

    return 0;
}

