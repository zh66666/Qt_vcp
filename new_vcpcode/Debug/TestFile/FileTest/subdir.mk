################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TestFile/FileTest/test_case.c 

OBJS += \
./TestFile/FileTest/test_case.o 

C_DEPS += \
./TestFile/FileTest/test_case.d 


# Each subdirectory must supply rules for building sources it contributes
TestFile/FileTest/%.o: ../TestFile/FileTest/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


