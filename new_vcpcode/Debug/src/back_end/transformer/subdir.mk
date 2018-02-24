################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/back_end/transformer/trans_struct.cpp 

OBJS += \
./src/back_end/transformer/trans_struct.o 

CPP_DEPS += \
./src/back_end/transformer/trans_struct.d 


# Each subdirectory must supply rules for building sources it contributes
src/back_end/transformer/%.o: ../src/back_end/transformer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


