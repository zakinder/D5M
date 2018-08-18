################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HDMI_DISPLAY/hdmi_display.c \
../src/HDMI_DISPLAY/hdmi_display_bars.c 

OBJS += \
./src/HDMI_DISPLAY/hdmi_display.o \
./src/HDMI_DISPLAY/hdmi_display_bars.o 

C_DEPS += \
./src/HDMI_DISPLAY/hdmi_display.d \
./src/HDMI_DISPLAY/hdmi_display_bars.d 


# Each subdirectory must supply rules for building sources it contributes
src/HDMI_DISPLAY/%.o: ../src/HDMI_DISPLAY/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../D5M_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


