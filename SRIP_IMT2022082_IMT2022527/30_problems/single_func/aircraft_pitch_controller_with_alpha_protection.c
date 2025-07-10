// aircraft_pitch_controller_with_alpha_protection.c
// Controls elevator deflection to maintain a target pitch angle based on the active autopilot mode. A critical Angle of Attack (Alpha) protection system provides a safety override to prevent stalls, making the autopilot mode conditionally irrelevant.
// elevator_deflection >= -25 && elevator_deflection <= 15
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

#define ELEVATOR_MAX_UP_DEFLECTION 15
#define ELEVATOR_MAX_DOWN_DEFLECTION -25
#define ALPHA_CRITICAL_DEGREES 16.0f
#define AUTOPILOT_MODE_ALT_HOLD 1
#define AUTOPILOT_MODE_GLIDESLOPE 2

volatile float current_pitch_deg;      
volatile float target_pitch_deg;       
volatile float current_alpha_deg;      
volatile int autopilot_mode;           
volatile bool flight_director_active;  
volatile int handle_critical_alpha;           
int step(int last_elevator_deflection);

int step(int last_elevator_deflection) {
    int new_deflection = 0;

    if (current_alpha_deg >= ALPHA_CRITICAL_DEGREES) {

        if(handle_critical_alpha){
            new_deflection = ELEVATOR_MAX_DOWN_DEFLECTION/2;
        }
        else{
            new_deflection = ELEVATOR_MAX_DOWN_DEFLECTION;
        }

    } 
    else if (flight_director_active) {
        float pitch_error = target_pitch_deg - current_pitch_deg;
                if (autopilot_mode == AUTOPILOT_MODE_ALT_HOLD) {
            handle_critical_alpha = 1;
            new_deflection = (int)(pitch_error * 5.0f);
        } 
        else if (autopilot_mode == AUTOPILOT_MODE_GLIDESLOPE) {
            new_deflection = (int)(pitch_error * 8.0f);
        } 
        else {
            new_deflection = 0;
        }
    } 
    else {
        new_deflection = last_elevator_deflection; 
    }

   
    if (new_deflection > ELEVATOR_MAX_UP_DEFLECTION) {
        new_deflection = ELEVATOR_MAX_UP_DEFLECTION;
    }
    if (new_deflection < ELEVATOR_MAX_DOWN_DEFLECTION) {
        new_deflection = ELEVATOR_MAX_DOWN_DEFLECTION;
    }

    return new_deflection;
}


int main() {
    int elevator_deflection = 0;
    
    printf("--- Aircraft Pitch Control Simulation ---\n");

    for(int i = 0; i < 200; ++i) { 
        current_pitch_deg = ((float)(rand() % 2001) / 100.0f) - 10.0f;      
        target_pitch_deg = ((float)(rand() % 2001) / 100.0f) - 10.0f;       
        current_alpha_deg = ((float)(rand() % 2001) / 100.0f);              
        autopilot_mode = (rand() % 3);                                      
        flight_director_active = (rand() % 2) ? true : false;               
        elevator_deflection = step(elevator_deflection);
        
       
        if (elevator_deflection < ELEVATOR_MAX_DOWN_DEFLECTION || elevator_deflection > ELEVATOR_MAX_UP_DEFLECTION) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1; 
        }
        if (i == 1) current_alpha_deg = 12.0f; 
    }

    return 0;
}

