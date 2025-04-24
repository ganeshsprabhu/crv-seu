#include <stdio.h>
#include <stdbool.h>
#include "simulate_seu.h"  // Include simulate_seu_main()

// Sliced version (used in both original and prime)
int control_fan_speed(int temperature, int fan_speed, int system_mode, int operating_hours)
{
  if (!(system_mode == 0)) 
    if (system_mode == 1) 
      if (fan_speed < 90) 
        if (temperature > 30) fan_speed += 5;
        else 
          if (temperature < 20) fan_speed -= 5;
  if (operating_hours > 100) 
    if (fan_speed < 95) fan_speed++;
  return fan_speed;
}

// Prime version with SEU introduced on one variable
int control_fan_speed_prime(int temperature, int fan_speed, int system_mode, int operating_hours)
{
  if (!(system_mode == 0)) 
    if (system_mode == 1) 
      if (fan_speed < 90) 
        if (temperature > 30) fan_speed += 5;
        else 
          if (temperature < 20) fan_speed -= 5;

  simulate_seu_main(&operating_hours);  // fit flip        
  if (operating_hours > 100) 
    if (fan_speed < 95) fan_speed++;
  return fan_speed;
}

int main() {
  int temperature;
  int fan_speed;
  int system_mode;
  int operating_hours;

  int final_speed = control_fan_speed(temperature, fan_speed, system_mode, operating_hours);
  int final_speed_prime = control_fan_speed_prime(temperature, fan_speed, system_mode, operating_hours);

  printf("Final fan speed: %d\n", final_speed);
  printf("Final fan speed (after SEU): %d\n", final_speed_prime);

  // Safety condition: fan_speed must not exceed 100
  int phi = final_speed <= 100;
  int phi_prime = final_speed_prime <= 100;

  // Assertion: if phi flips → the variable under test is a CRV
  __CPROVER_assert(!(phi ^ phi_prime), "CRV Result => if FAILURE, variable is a CRV");

  return 0;
}
