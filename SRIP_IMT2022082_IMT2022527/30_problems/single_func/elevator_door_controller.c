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
volatile int hold_open_timer; // A timer for holding doors open
volatile bool special_hold_profile_active; // (CRV candidate)

// --- Function Prototypes ---
void read_elevator_sensors();
void log_door_state(const char* reason, int force);
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
 * @brief Main door control logic step.
 * @param last_door_force The previous motor force command.
 * @return The new calculated motor force.
 */
int step(int last_door_force) {
    int new_force = 0;

    // 1. CRITICAL SAFETY OVERRIDE: Obstruction Detected
    // This path makes 'special_hold_profile_active' and close commands irrelevant.
    if (obstruction_detected) {
        new_force = DOOR_OPEN_FORCE; // Force doors open
        log_door_state("OBSTRUCTION DETECTED", new_force);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else {
        // Check for hold-open timer, which could be set by the CRV
        if (hold_open_timer > 0) {
            hold_open_timer--;
            new_force = 0; // Hold position
            log_door_state("Holding door open", new_force);
        } else if (command_close_door) {
            new_force = DOOR_CLOSE_FORCE;
            log_door_state("Closing door", new_force);
        } else { // (e.g. command_open_door is true)
            new_force = DOOR_OPEN_FORCE;
             // CRV logic: Set a longer timer if the special profile is active
            if (special_hold_profile_active) {
                hold_open_timer = 20; // Long hold for lobby
                log_door_state("Opening Door (Lobby Profile)", new_force);
            } else {
                hold_open_timer = 5; // Standard hold
                log_door_state("Opening Door (Std Profile)", new_force);
            }
        }
    }

    // 3. FINAL SAFETY SATURATION
    if (new_force > MAX_DOOR_FORCE) {
        new_force = MAX_DOOR_FORCE;
    }
    if (new_force < -MAX_DOOR_FORCE) {
        new_force = -MAX_DOOR_FORCE;
    }

    return new_force;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int door_force = 0;
    printf("--- Elevator Door Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
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
        door_force = step(door_force);
        
        // Safety property check
        if (door_force < -MAX_DOOR_FORCE || door_force > MAX_DOOR_FORCE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) obstruction_detected = false;
    }

    return 0;
}

