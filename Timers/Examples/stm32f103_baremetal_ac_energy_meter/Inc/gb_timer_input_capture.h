#ifndef GB_TIMER_INPUT_CAPTURE_H_
#define GB_TIMER_INPUT_CAPTURE_H_





void T4C1_pin_conf();

void gb_t4_input_capture();

int gb_t4_calculate();

void tim4_ch1_capture_config();
void tim4_ch1_capture_unconfig();


float getPeriod();
float getwidth();
float getFrequency();


void setPeriod(uint32_t value);










#endif
