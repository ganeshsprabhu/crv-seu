// anesthesia_machine_gas_mixer.c
// Controls the mixture of anesthetic gas (e.g., Sevoflurane) and Oxygen. The mixture percentage is normally determined by settings based on patient data (a CRV). A low oxygen supply pressure alarm provides a critical safety override, forcing the machine to deliver 100% oxygen and shut off the anesthetic gas, ignoring the patient settings.
// anesthetic_percentage >= 0 && anesthetic_percentage <= 8
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ANESTHETIC_PERCENT 8
#define MIN_ANESTHETIC_PERCENT 0
#define LOW_O2_PRESSURE_THRESHOLD 30 // PSI

// Simulated hardware inputs
volatile int o2_supply_pressure;
volatile int patient_age;
volatile int target_anesthetic_percent; // (CRV candidate)

void read_medical_sensors() {
    o2_supply_pressure = 25 + (rand() % 20); // Pressure around the threshold
    patient_age = 45;
    // Set target based on age normally
    target_anesthetic_percent = (patient_age > 60) ? 2 : 3;
}

void log_gas_mix(const char* reason, int percent) {
    printf("Reason: %-25s | Anesthetic Gas: %d%%, Oxygen: %d%%\n", reason, percent, 100 - percent);
}

int step_control_logic() {
    int new_anesthetic_percent;

    // 1. Critical Safety Override: Low O2 Supply
    // This path makes 'target_anesthetic_percent' irrelevant.
    if (o2_supply_pressure < LOW_O2_PRESSURE_THRESHOLD) {
        new_anesthetic_percent = MIN_ANESTHETIC_PERCENT;
        log_gas_mix("LOW O2 PRESSURE", new_anesthetic_percent);
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'target_anesthetic_percent'
        new_anesthetic_percent = target_anesthetic_percent;
        log_gas_mix("Patient Setting", new_anesthetic_percent);
    }

    // 3. Final Safety Saturation
    if (new_anesthetic_percent > MAX_ANESTHETIC_PERCENT) new_anesthetic_percent = MAX_ANESTHETIC_PERCENT;
    if (new_anesthetic_percent < MIN_ANESTHETIC_PERCENT) new_anesthetic_percent = MIN_ANESTHETIC_PERCENT;

    return new_anesthetic_percent;
}

int main() {
    srand(time(NULL));
    int anesthetic_percentage = 0;
    printf("--- Anesthesia Gas Mixer Simulation (Single Function) ---\n");

    for (int i = 0; i < 5; ++i) {
        read_medical_sensors();
        anesthetic_percentage = step_control_logic();
        
        if (anesthetic_percentage < MIN_ANESTHETIC_PERCENT || anesthetic_percentage > MAX_ANESTHETIC_PERCENT) {
            printf("!!! SAFETY VIOLATION: Anesthetic concentration out of safe range !!!\n");
            return -1;
        }
    }

    return 0;
}

