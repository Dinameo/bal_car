################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/conf.c \
../libs/encoder.c \
../libs/hc05.c \
../libs/interrupt.c \
../libs/l298n.c \
../libs/mixer.c \
../libs/mpu6050.c \
../libs/pidcb.c \
../libs/utils.c 

OBJS += \
./libs/conf.o \
./libs/encoder.o \
./libs/hc05.o \
./libs/interrupt.o \
./libs/l298n.o \
./libs/mixer.o \
./libs/mpu6050.o \
./libs/pidcb.o \
./libs/utils.o 

C_DEPS += \
./libs/conf.d \
./libs/encoder.d \
./libs/hc05.d \
./libs/interrupt.d \
./libs/l298n.d \
./libs/mixer.d \
./libs/mpu6050.d \
./libs/pidcb.d \
./libs/utils.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o libs/%.su libs/%.cyclo: ../libs/%.c libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/Documents/NCKH/TASK 6/bal_car/libs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libs

clean-libs:
	-$(RM) ./libs/conf.cyclo ./libs/conf.d ./libs/conf.o ./libs/conf.su ./libs/encoder.cyclo ./libs/encoder.d ./libs/encoder.o ./libs/encoder.su ./libs/hc05.cyclo ./libs/hc05.d ./libs/hc05.o ./libs/hc05.su ./libs/interrupt.cyclo ./libs/interrupt.d ./libs/interrupt.o ./libs/interrupt.su ./libs/l298n.cyclo ./libs/l298n.d ./libs/l298n.o ./libs/l298n.su ./libs/mixer.cyclo ./libs/mixer.d ./libs/mixer.o ./libs/mixer.su ./libs/mpu6050.cyclo ./libs/mpu6050.d ./libs/mpu6050.o ./libs/mpu6050.su ./libs/pidcb.cyclo ./libs/pidcb.d ./libs/pidcb.o ./libs/pidcb.su ./libs/utils.cyclo ./libs/utils.d ./libs/utils.o ./libs/utils.su

.PHONY: clean-libs

