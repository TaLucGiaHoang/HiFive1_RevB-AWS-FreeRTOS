################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include stdio/subdir.mk
-include full_demo/Common_Demo_Tasks/subdir.mk
-include full_demo/subdir.mk
-include freedom-metal/src/drivers/subdir.mk
-include freedom-metal/src/subdir.mk
-include freedom-metal/gloss/subdir.mk
-include blinky_demo/subdir.mk
-include FreeRTOS_Source/portable/MemMang/subdir.mk
-include FreeRTOS_Source/portable/GCC/RISC-V/subdir.mk
-include FreeRTOS_Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: RTOSDemo

# Tool invocations
RTOSDemo: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	riscv64-unknown-elf-gcc.exe -nostartfiles -Xlinker --gc-sections -Wl,-Map,RTOSDemo.map -T../bsp/metal.default.lds -march=rv32imac -mabi=ilp32 -mcmodel=medlow -Wl,--start-group -lc -lgcc -Wl,--end-group --specs=nano.specs -o "RTOSDemo" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) RTOSDemo
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
