################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/back_end/generator/gen_op/gen_add.cpp \
../src/back_end/generator/gen_op/gen_and.cpp \
../src/back_end/generator/gen_op/gen_assign.cpp \
../src/back_end/generator/gen_op/gen_bitInverse.cpp \
../src/back_end/generator/gen_op/gen_div.cpp \
../src/back_end/generator/gen_op/gen_mul.cpp \
../src/back_end/generator/gen_op/gen_not.cpp \
../src/back_end/generator/gen_op/gen_or.cpp \
../src/back_end/generator/gen_op/gen_relationcmp.cpp \
../src/back_end/generator/gen_op/gen_shift.cpp \
../src/back_end/generator/gen_op/gen_sub.cpp \
../src/back_end/generator/gen_op/inverse.cpp 

OBJS += \
./src/back_end/generator/gen_op/gen_add.o \
./src/back_end/generator/gen_op/gen_and.o \
./src/back_end/generator/gen_op/gen_assign.o \
./src/back_end/generator/gen_op/gen_bitInverse.o \
./src/back_end/generator/gen_op/gen_div.o \
./src/back_end/generator/gen_op/gen_mul.o \
./src/back_end/generator/gen_op/gen_not.o \
./src/back_end/generator/gen_op/gen_or.o \
./src/back_end/generator/gen_op/gen_relationcmp.o \
./src/back_end/generator/gen_op/gen_shift.o \
./src/back_end/generator/gen_op/gen_sub.o \
./src/back_end/generator/gen_op/inverse.o 

CPP_DEPS += \
./src/back_end/generator/gen_op/gen_add.d \
./src/back_end/generator/gen_op/gen_and.d \
./src/back_end/generator/gen_op/gen_assign.d \
./src/back_end/generator/gen_op/gen_bitInverse.d \
./src/back_end/generator/gen_op/gen_div.d \
./src/back_end/generator/gen_op/gen_mul.d \
./src/back_end/generator/gen_op/gen_not.d \
./src/back_end/generator/gen_op/gen_or.d \
./src/back_end/generator/gen_op/gen_relationcmp.d \
./src/back_end/generator/gen_op/gen_shift.d \
./src/back_end/generator/gen_op/gen_sub.d \
./src/back_end/generator/gen_op/inverse.d 


# Each subdirectory must supply rules for building sources it contributes
src/back_end/generator/gen_op/%.o: ../src/back_end/generator/gen_op/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


