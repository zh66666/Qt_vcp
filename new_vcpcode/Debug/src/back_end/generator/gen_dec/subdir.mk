################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/back_end/generator/gen_dec/gen_fun_dec.cpp \
../src/back_end/generator/gen_dec/gen_var_dec.cpp 

OBJS += \
./src/back_end/generator/gen_dec/gen_fun_dec.o \
./src/back_end/generator/gen_dec/gen_var_dec.o 

CPP_DEPS += \
./src/back_end/generator/gen_dec/gen_fun_dec.d \
./src/back_end/generator/gen_dec/gen_var_dec.d 


# Each subdirectory must supply rules for building sources it contributes
src/back_end/generator/gen_dec/%.o: ../src/back_end/generator/gen_dec/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


