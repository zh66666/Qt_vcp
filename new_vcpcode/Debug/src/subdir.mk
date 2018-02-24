################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/data_struct.cpp \
../src/gen_io.cpp \
../src/get_config_info.cpp \
../src/main_utils.cpp \
../src/red_code.cpp \
../src/resolve_file_name.cpp 

OBJS += \
./src/data_struct.o \
./src/gen_io.o \
./src/get_config_info.o \
./src/main_utils.o \
./src/red_code.o \
./src/resolve_file_name.o 

CPP_DEPS += \
./src/data_struct.d \
./src/gen_io.d \
./src/get_config_info.d \
./src/main_utils.d \
./src/red_code.d \
./src/resolve_file_name.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"E:\nanche\code\vcp_code\src\include" -I"E:\nanche\code\vcp_code\src\back_end\generator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


