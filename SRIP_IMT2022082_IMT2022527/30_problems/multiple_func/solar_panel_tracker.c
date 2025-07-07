// solar_panel_tracker.c
// Adjusts the panel's angle to track the sun. A high wind speed sensor forces the panel to a safe, flat (stowed) position, providing a critical safety override. The tracking algorithm mode (e.g., 'light sensor-based' vs. 'time-based predictive'), is a CRV as it is ignored during high winds.
// actuator_speed >= -100 && actuator_speed <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// --- Constants and Definitions ---
#define MAX_ACTUATOR_SPEED 100
#define WIND_SPEED_STOW_MPH 50
#define STOW_POSITION_DEGREES 0
#define LDR_THRESHOLD 50

// --- Simulated Hardware Inputs ---
volatile int panel_angle_deg;
volatile int wind_speed_mph;
volatile int ldr_east_lumens;
volatile int ldr_west_lumens;
volatile int tracking_mode; // (CRV candidate)

// --- Function Prototypes ---
void read_environmental_sensors();
void log_tracker_state(const char* reason, int speed);
bool is_wind_speed_critical();
int calculate_stow_speed();
int calculate_normal_tracking_speed();
int apply_speed_saturation(int speed);
int step(int last_actuator_speed);

/**
 * @brief Simulates reading data from sensors.
 */
void read_environmental_sensors() {
    panel_angle_deg = rand() % 91; // 0 to 90 degrees
    wind_speed_mph = 10 + rand() % 71; // 10 to 80 mph
    ldr_east_lumens = 500 + rand() % 1001; // 500 to 1500 lumens
    ldr_west_lumens = 500 + rand() % 1001; // 500 to 1500 lumens
    tracking_mode = rand() % 2; // 0 or 1
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_tracker_state(const char* reason, int speed) {
    printf("Logic: %-25s | Actuator Speed: %d\n", reason, speed);
}

/**
 * @brief Checks for critically high wind speed.
 */
bool is_wind_speed_critical() {
    return wind_speed_mph > WIND_SPEED_STOW_MPH;
}

/**
 * @brief Calculates the actuator speed required to move to the stow position.
 */
int calculate_stow_speed() {
    log_tracker_state("HIGH WIND - STOWING", 0);
    if (panel_angle_deg > STOW_POSITION_DEGREES) return -MAX_ACTUATOR_SPEED;
    if (panel_angle_deg < STOW_POSITION_DEGREES) return MAX_ACTUATOR_SPEED;
    return 0;
}

/**
 * @brief Calculates tracking speed based on the selected mode.
 */
int calculate_normal_tracking_speed() {
    if (tracking_mode == 0) { // LDR-based
        int light_diff = ldr_east_lumens - ldr_west_lumens;
        log_tracker_state("LDR Tracking Mode", 0);
        if (abs(light_diff) > LDR_THRESHOLD) {
            return (light_diff > 0) ? 50 : -50;
        }
    } else { // Predictive
        log_tracker_state("Predictive Tracking Mode", 10);
        return 10;
    }
    return 0;
}

/**
 * @brief Clamps the speed to its absolute physical limits.
 */
int apply_speed_saturation(int speed) {
    if (speed > MAX_ACTUATOR_SPEED) return MAX_ACTUATOR_SPEED;
    if (speed < -MAX_ACTUATOR_SPEED) return -MAX_ACTUATOR_SPEED;
    return speed;
}

/**
 * @brief Main tracker control step.
 */
int step(int last_actuator_speed) {
    int new_speed;

    // Priority 1: Critical Safety Check
    if (is_wind_speed_critical()) {
        new_speed = calculate_stow_speed();
    } 
    // Priority 2: Normal Operation
    else {
        new_speed = calculate_normal_tracking_speed();
    }

    return apply_speed_saturation(new_speed);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int actuator_speed = 0;
    printf("--- Solar Panel Tracker Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_environmental_sensors();
        actuator_speed = step(actuator_speed);
        
        if (actuator_speed < -MAX_ACTUATOR_SPEED || actuator_speed > MAX_ACTUATOR_SPEED) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        if (i == 1) wind_speed_mph = 20;
    }

    return 0;
}

