#!/bin/bash

##Takes inputs to run the automate_create_files.sh, this creates the *_cbmc_ready.c which we then insert the queue statements and then check to see if the variable under consideration is infact a CRV or a non-CRV.
entries=(
    "automated_greenhouse_vent_controller.c|step_control_logic|new_vent_opening|current_temp"
    "automated_greenhouse_vent_controller.c|step_control_logic|new_vent_opening|target_temp"
    "automotive_abs_controller.c|step|new_pressure|slip_target"
    "automotive_abs_controller.c|step|new_pressure|driver_brake_request"
    "batter_management_system.c|step|new_current|target_current"
    "batter_management_system.c|step|new_current|pack_temp_c"
    "chemical_reactor_controller.c|step_heater|new_heater_duty|target_temp"
    "chemical_reactor_controller.c|step_heater|new_heater_duty|reaction_phase"
    "data_center_cooling_unit.c|step|new_state|water_leak"
    "data_center_cooling_unit.c|step|new_state|server_cpu_load_percent"
    "data_center_cooling_unit.c|step|new_state|rack_inlet_temp_c"
    "drone_motor_controller.c|step|new_throttle|battery_voltage_mv"
    "drone_motor_controller.c|step|new_throttle|armed_flight_mode"
    "elevateor_door_controller.c|step|new_force|special_hold_profile_active"
    "elevateor_door_controller.c|step|new_force|close_door"
    "elevateor_door_controller.c|step|new_force|obstruction_detected"
    "elevateor_door_controller.c|step|new_force|hold_open_timer"
)

for entry in "${entries[@]}"; do
  IFS='|' read -r filename step_name output_name var_name <<< "$entry"
  echo "--------------------------------------------------------------------------------------------------------------------------------------"
  ./automate_create_files.sh filename step_name output_name var_name

  while true; do
    read -p "Type NEXT to continue: " input
    if [[ "$input" == "NEXT" ]]; then
      break
    fi
  done
done

