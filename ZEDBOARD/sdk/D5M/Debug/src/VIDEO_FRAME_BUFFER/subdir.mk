################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/VIDEO_FRAME_BUFFER/video_frame_buffer.c 

OBJS += \
./src/VIDEO_FRAME_BUFFER/video_frame_buffer.o 

C_DEPS += \
./src/VIDEO_FRAME_BUFFER/video_frame_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
src/VIDEO_FRAME_BUFFER/%.o: ../src/VIDEO_FRAME_BUFFER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../D5M_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


