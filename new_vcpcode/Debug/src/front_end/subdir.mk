################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/front_end/parser_utils.cpp \
../src/front_end/vcp_lex.cpp \
../src/front_end/vcp_yacc.cpp 

OBJS += \
./src/front_end/parser_utils.o \
./src/front_end/vcp_lex.o \
./src/front_end/vcp_yacc.o 

CPP_DEPS += \
./src/front_end/parser_utils.d \
./src/front_end/vcp_lex.d \
./src/front_end/vcp_yacc.d 


# Each subdirectory must supply rules for building sources it contributes
src/front_end/%.o: ../src/front_end/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


