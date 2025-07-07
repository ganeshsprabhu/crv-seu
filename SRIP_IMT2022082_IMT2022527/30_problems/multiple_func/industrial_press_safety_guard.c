// industrial_press_safety_guard.c
// Controls the operation of a hydraulic press. Normal operation uses a part counter (a CRV) to determine when the press should stop for a batch change. A light curtain sensor, which detects if an object (like a hand) is in the press area, provides a critical safety override that immediately stops the press, ignoring the part count.
// press_state == 0 || press_state == 1
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define STATE_STOPPED 0
#define STATE_RUNNING 1
#define BATCH_SIZE 1000

// Simulated hardware inputs
volatile bool light_curtain_breached;
volatile int part_counter; // (CRV candidate)
volatile bool start_command;

void read_press_sensors() {
    light_curtain_breached = (rand() % 20 == 0);
    start_command = true;
}

void log_press_state(const char* reason, int state) {
    printf("Reason: %-20s | Press State: %s\n", reason, state == STATE_RUNNING ? "RUNNING" : "STOPPED");
}

bool is_safety_override_active() {
    return light_curtain_breached;
}

int calculate_normal_state(int current_state) {
    if (current_state == STATE_STOPPED && start_command) {
        if (part_counter < BATCH_SIZE) {
            log_press_state("Start Command", STATE_RUNNING);
            return STATE_RUNNING;
        } else {
            log_press_state("Batch Complete", STATE_STOPPED);
            return STATE_STOPPED;
        }
    } else if (current_state == STATE_RUNNING) {
        if (part_counter >= BATCH_SIZE) {
             log_press_state("Batch Size Reached", STATE_STOPPED);
             return STATE_STOPPED;
        } else {
            part_counter++;
            return STATE_RUNNING;
        }
    }
    return current_state;
}

int step_control_logic(int current_state) {
    if (is_safety_override_active()) {
        log_press_state("LIGHT CURTAIN FAULT", STATE_STOPPED);
        return STATE_STOPPED;
    } else {
        return calculate_normal_state(current_state);
    }
}

int main() {
    srand(time(NULL));
    int press_state = STATE_STOPPED;
    part_counter = 995;
    printf("--- Industrial Press Control Simulation (Multi Function) ---\n");

    for (int i = 0; i < 10; ++i) {
        read_press_sensors();
        press_state = step_control_logic(press_state);
        
        if (press_state != STATE_RUNNING && press_state != STATE_STOPPED) {
            printf("!!! SAFETY VIOLATION: Invalid press state !!!\n");
            return -1;
        }
        if (press_state == STATE_STOPPED && light_curtain_breached) {
            printf("System stopped safely due to breach.\n");
        }
    }
    return 0;
}

