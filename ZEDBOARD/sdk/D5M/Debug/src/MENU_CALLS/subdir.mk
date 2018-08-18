################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MENU_CALLS/menu_calls.c 

OBJS += \
./src/MENU_CALLS/menu_calls.o 

C_DEPS += \
./src/MENU_CALLS/menu_calls.d 


# Each subdirectory must supply rules for building sources it contributes
src/MENU_CALLS/%.o: ../src/MENU_CALLS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../D5M_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


