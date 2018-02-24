################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/back_end/generator/gen_stmt/gen_array_stmt.cpp \
../src/back_end/generator/gen_stmt/gen_compd_stmt.cpp \
../src/back_end/generator/gen_stmt/gen_fun_stmt.cpp \
../src/back_end/generator/gen_stmt/gen_iterat_stmt.cpp \
../src/back_end/generator/gen_stmt/gen_ret_stmt.cpp \
../src/back_end/generator/gen_stmt/gen_select_stmt.cpp \
../src/back_end/generator/gen_stmt/gen_stmt.cpp \
../src/back_end/generator/gen_stmt/gen_var_stmt.cpp \
../src/back_end/generator/gen_stmt/while_if_utils.cpp 

OBJS += \
./src/back_end/generator/gen_stmt/gen_array_stmt.o \
./src/back_end/generator/gen_stmt/gen_compd_stmt.o \
./src/back_end/generator/gen_stmt/gen_fun_stmt.o \
./src/back_end/generator/gen_stmt/gen_iterat_stmt.o \
./src/back_end/generator/gen_stmt/gen_ret_stmt.o \
./src/back_end/generator/gen_stmt/gen_select_stmt.o \
./src/back_end/generator/gen_stmt/gen_stmt.o \
./src/back_end/generator/gen_stmt/gen_var_stmt.o \
./src/back_end/generator/gen_stmt/while_if_utils.o 

CPP_DEPS += \
./src/back_end/generator/gen_stmt/gen_array_stmt.d \
./src/back_end/generator/gen_stmt/gen_compd_stmt.d \
./src/back_end/generator/gen_stmt/gen_fun_stmt.d \
./src/back_end/generator/gen_stmt/gen_iterat_stmt.d \
./src/back_end/generator/gen_stmt/gen_ret_stmt.d \
./src/back_end/generator/gen_stmt/gen_select_stmt.d \
./src/back_end/generator/gen_stmt/gen_stmt.d \
./src/back_end/generator/gen_stmt/gen_var_stmt.d \
./src/back_end/generator/gen_stmt/while_if_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/back_end/generator/gen_stmt/%.o: ../src/back_end/generator/gen_stmt/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


