// railway_signal_controller.c
// Controls a trackside signal light based on a predefined schedule. A track occupancy sensor provides a critical safety override, forcing the signal to RED if an unscheduled train is detected. The train schedule data is a CRV, as it's completely ignored when the track is unexpectedly occupied.
// signal_state >= 0 && signal_state <= 2
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define STATE_RED 0
#define STATE_YELLOW 1
#define STATE_GREEN 2

// --- Simulated Hardware Inputs ---
volatile bool track_occupied;
volatile int time_of_day;
volatile bool scheduled_train_due; // (CRV candidate)
volatile bool comms_link_ok;

// --- Function Prototypes ---
void read_railway_sensors();
void log_signal_state(const char* reason, int state);
const char* state_to_string(int state);
bool is_safety_override_active();
int get_failsafe_state();
int calculate_scheduled_state();
int step(int last_signal_state);

/**
 * @brief Simulates reading data from track and system sensors.
 */
void read_railway_sensors() {
    // Randomize sensor values for simulation
    track_occupied = rand() % 2; // true or false
    time_of_day = 480 + rand() % 20; // 480 to 499 (8:00 AM to 8:19 AM)
    // Is a train scheduled for 8:05 AM? (simulate schedule window)
    scheduled_train_due = (time_of_day > 480 && time_of_day < 490);
    comms_link_ok = (rand() % 10 != 0); // 90% chance comms are OK
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_signal_state(const char* reason, int state) {
    printf("Logic: %-25s | Signal State: %s\n", reason, state_to_string(state));
}

const char* state_to_string(int state) {
    if (state == STATE_RED) return "RED";
    if (state == STATE_YELLOW) return "YELLOW";
    if (state == STATE_GREEN) return "GREEN";
    return "INVALID";
}

/**
 * @brief Checks for any condition that requires a safety override.
 */
bool is_safety_override_active() {
    if (track_occupied && !scheduled_train_due) return true;
    if (!comms_link_ok) return true;
    return false;
}

/**
 * @brief Determines the appropriate failsafe state based on the override reason.
 */
int get_failsafe_state() {
    if (track_occupied && !scheduled_train_due) {
        log_signal_state("UNSCHEDULED OCCUPANCY", STATE_RED);
    } else if (!comms_link_ok) {
        log_signal_state("COMMS FAIL - FAILSAFE", STATE_RED);
    }
    return STATE_RED;
}

/**
 * @brief Calculates the signal state based on the schedule.
 */
int calculate_scheduled_state() {
    if (scheduled_train_due) {
        log_signal_state("Scheduled Train Due", STATE_GREEN);
        return STATE_GREEN;
    } else {
        log_signal_state("No Scheduled Train", STATE_RED);
        return STATE_RED;
    }
}

/**
 * @brief Main signal control step.
 */
int step(int last_signal_state) {
    int new_state;

    // Priority 1: Critical Safety Check
    if (is_safety_override_active()) {
        new_state = get_failsafe_state();
    } 
    // Priority 2: Normal Operation
    else {
        new_state = calculate_scheduled_state();
    }

    if (new_state < STATE_RED || new_state > STATE_GREEN) {
        return STATE_RED;
    }
    return new_state;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int signal_state = STATE_RED;
    printf("--- Railway Signal Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_railway_sensors();
        signal_state = step(signal_state);
        
        if (signal_state < 0 || signal_state > 2) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) scheduled_train_due = false;
        if (i == 2) track_occupied = false;
    }

    return 0;
}

