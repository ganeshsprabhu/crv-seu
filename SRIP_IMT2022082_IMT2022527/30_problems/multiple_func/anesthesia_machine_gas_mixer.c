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
    o2_supply_pressure = 25 + (rand() % 20);
    patient_age = 45;
    target_anesthetic_percent = (patient_age > 60) ? 2 : 3;
}

void log_gas_mix(const char* reason, int percent) {
    printf("Reason: %-25s | Anesthetic Gas: %d%%, Oxygen: %d%%\n", reason, percent, 100 - percent);
}

bool is_o2_pressure_low() {
    return o2_supply_pressure < LOW_O2_PRESSURE_THRESHOLD;
}

int get_normal_anesthetic_percent() {
    log_gas_mix("Patient Setting", target_anesthetic_percent);
    return target_anesthetic_percent;
}

int apply_concentration_limits(int percent) {
    if (percent > MAX_ANESTHETIC_PERCENT) return MAX_ANESTHETIC_PERCENT;
    if (percent < MIN_ANESTHETIC_PERCENT) return MIN_ANESTHETIC_PERCENT;
    return percent;
}

int step_control_logic() {
    int new_anesthetic_percent;
    if (is_o2_pressure_low()) {
        new_anesthetic_percent = MIN_ANESTHETIC_PERCENT;
        log_gas_mix("LOW O2 PRESSURE", new_anesthetic_percent);
    } else {
        new_anesthetic_percent = get_normal_anesthetic_percent();
    }
    return apply_concentration_limits(new_anesthetic_percent);
}

int main() {
    srand(time(NULL));
    int anesthetic_percentage = 0;
    printf("--- Anesthesia Gas Mixer Simulation (Multi Function) ---\n");

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

