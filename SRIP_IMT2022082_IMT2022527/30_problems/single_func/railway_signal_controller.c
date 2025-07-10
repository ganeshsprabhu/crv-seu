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
volatile int time_of_day; // minutes from midnight
volatile bool scheduled_train_due; // From timetable (CRV candidate)
volatile bool comms_link_ok;

// --- Function Prototypes ---
void read_railway_sensors();
void log_signal_state(const char* reason, int state);
const char* state_to_string(int state);
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
 * @brief Main signal control logic step.
 * @param last_signal_state The previous signal state.
 * @return The new calculated signal state.
 */
int step(int last_signal_state) {
    int new_state;

    // 1. CRITICAL SAFETY OVERRIDE: Track Occupancy
    // This path makes 'scheduled_train_due' irrelevant.
    if (track_occupied && !scheduled_train_due) { // Unscheduled occupancy
        new_state = STATE_RED;
        log_signal_state("UNSCHEDULED OCCUPANCY", new_state);
    } 
    // 2. CRITICAL SAFETY OVERRIDE: Comms Failure
    else if (!comms_link_ok) {
        new_state = STATE_RED;
        log_signal_state("COMMS FAIL - FAILSAFE", new_state);
    } 
    // 3. STANDARD OPERATIONAL LOGIC
    else {
        // Logic depends on the CRV 'scheduled_train_due'
        if (scheduled_train_due) {
            new_state = STATE_GREEN;
            log_signal_state("Scheduled Train Due", new_state);
        } else {
            new_state = STATE_RED;
            log_signal_state("No Scheduled Train", new_state);
        }
    }

    return new_state;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int signal_state = STATE_RED;
    printf("--- Railway Signal Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        track_occupied = rand() % 2; // true or false
        time_of_day = 480 + rand() % 20; // 480 to 499 (8:00 AM to 8:19 AM)
        // Is a train scheduled for 8:05 AM? (simulate schedule window)
        scheduled_train_due = (time_of_day > 480 && time_of_day < 490);
        comms_link_ok = (rand() % 10 != 0); // 90% chance comms are OK
        signal_state = step(signal_state);
        
        // Safety property check
        if (signal_state < 0 || signal_state > 2) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) scheduled_train_due = false; // The unscheduled train passes
        if (i == 2) track_occupied = false;
    }

    return 0;
}

