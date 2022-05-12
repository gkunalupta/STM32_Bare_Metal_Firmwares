################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/GB_ESP8266_Basic.c \
../Src/GB_ESP8266_client.c \
../Src/GB_millis_systick.c \
../Src/GB_uart.c \
../Src/gb_dht11.c \
../Src/gb_gpio.c \
../Src/gb_i2c.c \
../Src/gb_rcc.c \
../Src/gb_ssd1306.c \
../Src/gb_t_delay.c \
../Src/main.c \
../Src/spi.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f1xx.c \
../Src/w25q128jv.c 

OBJS += \
./Src/GB_ESP8266_Basic.o \
./Src/GB_ESP8266_client.o \
./Src/GB_millis_systick.o \
./Src/GB_uart.o \
./Src/gb_dht11.o \
./Src/gb_gpio.o \
./Src/gb_i2c.o \
./Src/gb_rcc.o \
./Src/gb_ssd1306.o \
./Src/gb_t_delay.o \
./Src/main.o \
./Src/spi.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f1xx.o \
./Src/w25q128jv.o 

C_DEPS += \
./Src/GB_ESP8266_Basic.d \
./Src/GB_ESP8266_client.d \
./Src/GB_millis_systick.d \
./Src/GB_uart.d \
./Src/gb_dht11.d \
./Src/gb_gpio.d \
./Src/gb_i2c.d \
./Src/gb_rcc.d \
./Src/gb_ssd1306.d \
./Src/gb_t_delay.d \
./Src/main.d \
./Src/spi.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f1xx.d \
./Src/w25q128jv.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/GB_ESP8266_Basic.d ./Src/GB_ESP8266_Basic.o ./Src/GB_ESP8266_client.d ./Src/GB_ESP8266_client.o ./Src/GB_millis_systick.d ./Src/GB_millis_systick.o ./Src/GB_uart.d ./Src/GB_uart.o ./Src/gb_dht11.d ./Src/gb_dht11.o ./Src/gb_gpio.d ./Src/gb_gpio.o ./Src/gb_i2c.d ./Src/gb_i2c.o ./Src/gb_rcc.d ./Src/gb_rcc.o ./Src/gb_ssd1306.d ./Src/gb_ssd1306.o ./Src/gb_t_delay.d ./Src/gb_t_delay.o ./Src/main.d ./Src/main.o ./Src/spi.d ./Src/spi.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32f1xx.d ./Src/system_stm32f1xx.o ./Src/w25q128jv.d ./Src/w25q128jv.o

.PHONY: clean-Src

