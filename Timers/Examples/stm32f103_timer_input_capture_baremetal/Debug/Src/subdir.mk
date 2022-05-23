################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/GB_millis_systick.c \
../Src/GB_uart.c \
../Src/gb_dht11.c \
../Src/gb_gpio.c \
../Src/gb_rcc.c \
../Src/gb_t_delay.c \
../Src/gb_timer_input_capture.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f1xx.c \
../Src/t_pwm.c 

OBJS += \
./Src/GB_millis_systick.o \
./Src/GB_uart.o \
./Src/gb_dht11.o \
./Src/gb_gpio.o \
./Src/gb_rcc.o \
./Src/gb_t_delay.o \
./Src/gb_timer_input_capture.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f1xx.o \
./Src/t_pwm.o 

C_DEPS += \
./Src/GB_millis_systick.d \
./Src/GB_uart.d \
./Src/gb_dht11.d \
./Src/gb_gpio.d \
./Src/gb_rcc.d \
./Src/gb_t_delay.d \
./Src/gb_timer_input_capture.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f1xx.d \
./Src/t_pwm.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/GB_millis_systick.d ./Src/GB_millis_systick.o ./Src/GB_uart.d ./Src/GB_uart.o ./Src/gb_dht11.d ./Src/gb_dht11.o ./Src/gb_gpio.d ./Src/gb_gpio.o ./Src/gb_rcc.d ./Src/gb_rcc.o ./Src/gb_t_delay.d ./Src/gb_t_delay.o ./Src/gb_timer_input_capture.d ./Src/gb_timer_input_capture.o ./Src/main.d ./Src/main.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32f1xx.d ./Src/system_stm32f1xx.o ./Src/t_pwm.d ./Src/t_pwm.o

.PHONY: clean-Src

