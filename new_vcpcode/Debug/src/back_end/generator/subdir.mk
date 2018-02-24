################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/back_end/generator/gen_global_var.cpp \
../src/back_end/generator/gen_pro.cpp \
../src/back_end/generator/gen_secondary.cpp \
../src/back_end/generator/gen_utils.cpp 

OBJS += \
./src/back_end/generator/gen_global_var.o \
./src/back_end/generator/gen_pro.o \
./src/back_end/generator/gen_secondary.o \
./src/back_end/generator/gen_utils.o 

CPP_DEPS += \
./src/back_end/generator/gen_global_var.d \
./src/back_end/generator/gen_pro.d \
./src/back_end/generator/gen_secondary.d \
./src/back_end/generator/gen_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/back_end/generator/%.o: ../src/back_end/generator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


