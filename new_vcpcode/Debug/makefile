################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/front_end/subdir.mk
-include src/create_extra/subdir.mk
-include src/back_end/transformer/typedef/subdir.mk
-include src/back_end/transformer/struct_array/subdir.mk
-include src/back_end/transformer/split_stmts/subdir.mk
-include src/back_end/transformer/enum/subdir.mk
-include src/back_end/transformer/subdir.mk
-include src/back_end/printer/subdir.mk
-include src/back_end/generator/gen_stmt/subdir.mk
-include src/back_end/generator/gen_op/subdir.mk
-include src/back_end/generator/gen_dec/subdir.mk
-include src/back_end/generator/subdir.mk
-include src/back_end/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: VitalEncoded.exe

# Tool invocations
VitalEncoded.exe: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cygwin C++ Linker'
	g++  -o "VitalEncoded.exe" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) VitalEncoded.exe
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
