// elevator_door_controller.c
// Controls the motor force for opening and closing elevator doors. A light curtain or physical obstruction sensor provides a critical safety override, forcing the doors to open. Floor-specific settings (e.g., 'hold door longer on lobby floor') are a CRV as they are ignored when an obstruction is detected.
// door_motor_force >= -50 && door_motor_force <= 50
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_DOOR_FORCE 50
#define DOOR_CLOSE_FORCE -20
#define DOOR_OPEN_FORCE 30
#define LOBBY_FLOOR 1

// --- Simulated Hardware Inputs ---
volatile bool obstruction_detected;
volatile int command_close_door;
volatile int current_floor;
volatile int hold_open_timer; // State variable
volatile bool special_hold_profile_active; // (CRV candidate)

// --- Function Prototypes ---
void read_elevator_sensors();
void log_door_state(const char* reason, int force);
bool is_path_obstructed();
int calculate_nominal_door_force();
int apply_force_saturation(int force);
int step(int last_door_force);

/**
 * @brief Simulates reading data from elevator sensors.
 */
void read_elevator_sensors() {
    // Randomly simulate obstruction (10% chance)
    obstruction_detected = (rand() % 10 == 0);

    // Randomly simulate close door command (50% chance)
    command_close_door = (rand() % 2 == 0);

    // Randomly select current floor between 1 and 5
    current_floor = 1 + (rand() % 5);

    // Special hold profile active only on lobby floor
    special_hold_profile_active = (current_floor == LOBBY_FLOOR);

    // Randomly set hold open timer between 0 and 3
    hold_open_timer = rand() % 4;
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_door_state(const char* reason, int force) {
    printf("Logic: %-25s | Door Motor Force: %d\n", reason, force);
}

/**
 * @brief Checks for a critical path obstruction.
 */
bool is_path_obstructed() {
    return obstruction_detected;
}

/**
 * @brief Calculates door motor force under normal conditions.
 */
int calculate_nominal_door_force() {
    if (hold_open_timer > 0) {
        hold_open_timer--;
        log_door_state("Holding door open", 0);
        return 0;
    }
    if (command_close_door) {
        log_door_state("Closing door", DOOR_CLOSE_FORCE);
        return DOOR_CLOSE_FORCE;
    }
    // If not closing, assume opening
    if (special_hold_profile_active) {
        hold_open_timer = 20;
        log_door_state("Opening (Lobby Profile)", DOOR_OPEN_FORCE);
    } else {
        hold_open_timer = 5;
        log_door_state("Opening (Std Profile)", DOOR_OPEN_FORCE);
    }
    return DOOR_OPEN_FORCE;
}

/**
 * @brief Clamps the motor force to its absolute physical limits.
 */
int apply_force_saturation(int force) {
    if (force > MAX_DOOR_FORCE) return MAX_DOOR_FORCE;
    if (force < -MAX_DOOR_FORCE) return -MAX_DOOR_FORCE;
    return force;
}

/**
 * @brief Main door control step.
 */
int step(int last_door_force) {
    int new_force;

    // Priority 1: Critical Safety Check
    if (is_path_obstructed()) {
        new_force = DOOR_OPEN_FORCE;
        log_door_state("OBSTRUCTION DETECTED", new_force);
    } 
    // Priority 2: Normal Operation
    else {
        new_force = calculate_nominal_door_force();
    }

    return apply_force_saturation(new_force);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int door_force = 0;
    printf("--- Elevator Door Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_elevator_sensors();
        door_force = step(door_force);
        
        if (door_force < -MAX_DOOR_FORCE || door_force > MAX_DOOR_FORCE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) obstruction_detected = false;
    }

    return 0;
}

