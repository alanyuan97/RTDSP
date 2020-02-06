################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv8/tools/compiler/c6000_7.4.23

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := -l"./dsp_bios_cfg.cmd"

ORDERED_OBJS += \
"./dsp_bios_cfg.obj" \
"./dsp_bios_cfg_c.obj" \
"./sine.obj" \
$(GEN_CMDS__FLAG) \
-lrts6700.lib \
-l"C:/ti/ccsv8/C6000/csl/lib/csl6713.lib" \
-l"C:/ti/ccsv8/C6000/dsk6713/lib/dsk6713bslccsv4.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Sine_wave_generator.out \

EXE_OUTPUTS__QUOTED += \
"Sine_wave_generator.out" \

BIN_OUTPUTS += \
Sine_wave_generator.hex \

BIN_OUTPUTS__QUOTED += \
"Sine_wave_generator.hex" \


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Sine_wave_generator.out"

# Tool invocations
Sine_wave_generator.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6700 --abi=coffabi -g --define=c6713 --define="_DEBUG" --define="CHIP_6713" --diag_wrap=off --display_error_number --diag_warning=225 -z --stack_size=0x800 -m"Sine_wave_generator.map" --heap_size=0x800 -i"C:/ti/ccsv8/tools/compiler/c6000_7.4.23/lib" -i"C:/ti/ccsv8/tools/compiler/c6000_7.4.23/include" -i"C:/ti/bios_5_42_02_10/packages/ti/rtdx/lib/c6000" -i"C:/ti/bios_5_42_02_10/packages/ti/bios/lib" --reread_libs --display_error_number --warn_sections --diag_wrap=off --xml_link_info="Sine_wave_generator_linkInfo.xml" --rom_model -o "Sine_wave_generator.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Sine_wave_generator.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C6000 Hex Utility'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.23/bin/hex6x"  -o "Sine_wave_generator.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_CMDS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_HDRS__QUOTED)
	-$(RM) "dsp_bios_cfg.d" 
	-$(RM) "dsp_bios_cfg.obj" "dsp_bios_cfg_c.obj" "sine.obj" 
	-$(RM) "dsp_bios_cfg_c.d" "sine.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

