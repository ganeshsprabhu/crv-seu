/* Generated by Frama-C */
#include "stddef.h"
#include "stdio.h"
int control_temperature_slice_1(float temperature, int fan_speed,
                                int humidity_level)
{
  int __retres;
  temperature -= (float)fan_speed;
  temperature = (float)((double)temperature + (double)humidity_level * 0.05);
  __retres = (int)temperature;
  return __retres;
}

int main(void)
{
  int __retres;
  float temperature = (float)25;
  int fan_speed = 5;
  int mode = 1;
  int humidity_level = 60;
  int final_temperature =
    control_temperature_slice_1(temperature,fan_speed,humidity_level);
  __retres = 0;
  return __retres;
}


