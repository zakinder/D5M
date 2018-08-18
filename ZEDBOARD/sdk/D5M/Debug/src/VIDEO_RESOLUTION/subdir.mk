################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/VIDEO_RESOLUTION/video_resolution.c 

OBJS += \
./src/VIDEO_RESOLUTION/video_resolution.o 

C_DEPS += \
./src/VIDEO_RESOLUTION/video_resolution.d 


# Each subdirectory must supply rules for building sources it contributes
src/VIDEO_RESOLUTION/%.o: ../src/VIDEO_RESOLUTION/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../D5M_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


