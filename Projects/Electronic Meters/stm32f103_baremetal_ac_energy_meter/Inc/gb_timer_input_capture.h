#ifndef GB_TIMER_INPUT_CAPTURE_H_
#define GB_TIMER_INPUT_CAPTURE_H_




void T4C1_pin_conf();

void gb_t4_input_capture();

int gb_t4_calculate();

void tim4_ch1_capture_config();
void tim4_ch1_capture_unconfig();


float TIM4_getPeriod();
float TIM4_getwidth();
float TIM4_getFrequency();


void TIM4_setPeriod(uint32_t value);




void T1C1_pin_conf();

void gb_t1_input_capture();

//int gb_t4_calculate();

void tim1_ch1_capture_config();
void tim1_ch1_capture_unconfig();


float TIM1_getPeriod();
float TIM1_getwidth();
float TIM1_getFrequency();


void TIM1_setPeriod(uint32_t value);









#endif
