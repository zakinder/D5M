################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/I2C_D5M/i2c_d5m.c 

OBJS += \
./src/I2C_D5M/i2c_d5m.o 

C_DEPS += \
./src/I2C_D5M/i2c_d5m.d 


# Each subdirectory must supply rules for building sources it contributes
src/I2C_D5M/%.o: ../src/I2C_D5M/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../D5M_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


