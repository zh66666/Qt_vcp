################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/back_end/transformer/split_stmts/split_exp.cpp \
../src/back_end/transformer/split_stmts/split_pro.cpp \
../src/back_end/transformer/split_stmts/split_stmt.cpp \
../src/back_end/transformer/split_stmts/split_sub_exp.cpp \
../src/back_end/transformer/split_stmts/split_utils.cpp 

OBJS += \
./src/back_end/transformer/split_stmts/split_exp.o \
./src/back_end/transformer/split_stmts/split_pro.o \
./src/back_end/transformer/split_stmts/split_stmt.o \
./src/back_end/transformer/split_stmts/split_sub_exp.o \
./src/back_end/transformer/split_stmts/split_utils.o 

CPP_DEPS += \
./src/back_end/transformer/split_stmts/split_exp.d \
./src/back_end/transformer/split_stmts/split_pro.d \
./src/back_end/transformer/split_stmts/split_stmt.d \
./src/back_end/transformer/split_stmts/split_sub_exp.d \
./src/back_end/transformer/split_stmts/split_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/back_end/transformer/split_stmts/%.o: ../src/back_end/transformer/split_stmts/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


