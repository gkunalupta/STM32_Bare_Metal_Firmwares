/*
 * hlw8012.h
 *
 *  Created on: May 16, 2022
 *      Author: acer
 */

#ifndef HLW8012_BAREMETAL_H_
#define HLW8012_BAREMETAL_H_
//
//#define V_REF               2.43
//#define R_CURRENT           0.001
//#define R_VOLTAGE           2821
//#define F_OSC               3579000
//#define READING_INTERVAL    3000
//#define PULSE_TIMEOUT       2000000
//
//double _current_resistor = R_CURRENT;
//double _voltage_resistor = R_VOLTAGE;
//
//
//double _current_multiplier; // Unit: us/A
//double _voltage_multiplier; // Unit: us/V
//double _power_multiplier;   // Unit: us/W
//
//unsigned long _pulse_timeout = PULSE_TIMEOUT;    //Unit: us
//  volatile unsigned long _voltage_pulse_width = 0; //Unit: us
//  volatile unsigned long _current_pulse_width = 0; //Unit: us
//  volatile unsigned long _power_pulse_width = 0;   //Unit: us
//  volatile unsigned long _pulse_count = 0;
//
//  // variables that store the current voltage power readings from hlw8012 after calculation
//  double _current = 0;
//  unsigned int _voltage = 0;
//  unsigned int _power = 0;
//
//// Check values every 2 seconds
//#define UPDATE_TIME                     2000
// will be used in set resistors function



   double hlw8012_getcurrent_multiplier();
   double hlw8012_getvoltage_multiplier();
   double hlw8012_getpower_multiplier();


void hlw8012_config();

   void hlw8012_sel_low_volatge();
   void hlw8012_sel_high_current();
   void hlw8012_pins_config();
   void gb_unblockingDelay(uint32_t mseconds);
   void hlw8012_calibrate();
   float hlw8012_getcurrent();
   float pulseIN_current();
   float hlw8012_getvoltage();
   float pulseIN_voltage();
   float hlw8012_getactivepower();
   void hlw8012_setResistors( double current, double voltage_upstream, double voltage_downstream);
   void hlw8012_expected_ActiveCurrent( double value);

#endif /* HLW8012_BAREMETAL_H_ */
