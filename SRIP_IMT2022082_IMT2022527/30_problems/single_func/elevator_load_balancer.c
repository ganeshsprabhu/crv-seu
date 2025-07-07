// elevator_load_balancer.c
// Dispatches elevators in a multi-car system to minimize wait times. Normal operation uses a complex algorithm based on passenger calls and car positions (a CRV). A fire alarm signal provides a critical safety override, recalling all elevators to the ground floor and parking them with doors open, completely ignoring the dispatch algorithm.
// car_command >= 0 && car_command <= 2
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define CMD_NORMAL_SERVICE 0
#define CMD_FIRE_RECALL 1
#define CMD_MAINTENANCE_HOLD 2

// Simulated hardware inputs
volatile bool fire_alarm_active;
volatile int passenger_wait_time; // (CRV candidate)
volatile int current_floor;

void read_elevator_system_sensors() {
    fire_alarm_active = (rand() % 10 == 0); // 10% chance of fire alarm
    passenger_wait_time = rand() % 120; // 0-119 seconds
    current_floor = rand() % 20;
}

void log_elevator_command(const char* reason, int command) {
    const char* cmd_str = "UNKNOWN";
    if (command == CMD_NORMAL_SERVICE) cmd_str = "NORMAL SERVICE";
    if (command == CMD_FIRE_RECALL) cmd_str = "FIRE RECALL";
    if (command == CMD_MAINTENANCE_HOLD) cmd_str = "HOLD";
    printf("Reason: %-20s | Command: %s\n", reason, cmd_str);
}

int step_control_logic() {
    int new_command;

    // 1. Critical Safety Override: Fire Alarm
    // This path makes 'passenger_wait_time' irrelevant.
    if (fire_alarm_active) {
        new_command = CMD_FIRE_RECALL;
        log_elevator_command("FIRE ALARM ACTIVE", new_command);
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'passenger_wait_time'
        if (passenger_wait_time > 60) {
            // Simplified logic: just log that we would dispatch based on wait time
            log_elevator_command("High Wait Time", CMD_NORMAL_SERVICE);
        } else {
            log_elevator_command("Normal Wait Time", CMD_NORMAL_SERVICE);
        }
        new_command = CMD_NORMAL_SERVICE;
    }

    return new_command;
}

int main() {
    srand(time(NULL));
    int car_command = CMD_NORMAL_SERVICE;
    printf("--- Elevator Load Balancer Simulation (Single Function) ---\n");

    for (int i = 0; i < 5; ++i) {
        read_elevator_system_sensors();
        car_command = step_control_logic();
        
        if (car_command < 0 || car_command > 2) {
            printf("!!! SAFETY VIOLATION: Invalid elevator command !!!\n");
            return -1;
        }
    }

    return 0;
}

