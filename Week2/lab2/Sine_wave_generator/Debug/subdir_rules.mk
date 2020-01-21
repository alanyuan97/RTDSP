################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-410347057:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-410347057-inproc

build-410347057-inproc: ../dsp_bios_.tcf
	@echo 'Building file: "$<"'
	@echo 'Invoking: TConf'
	"C:/ti/bios_5_42_02_10/xdctools/tconf" -b -Dconfig.importPath="C:/ti/bios_5_42_02_10/packages;" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

dsp_bios_cfg.cmd: build-410347057 ../dsp_bios_.tcf
dsp_bios_cfg.s??: build-410347057
dsp_bios_cfg_c.c: build-410347057
dsp_bios_cfg.h: build-410347057
dsp_bios_cfg.h??: build-410347057
dsp_bios_.cdb: build-410347057

dsp_bios_cfg.obj: ./dsp_bios_cfg.s?? $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="H:/RTDSPlab/lab2/Sine_wave_generator" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.23/include" --include_path="H:/RTDSPlab/lab2/Sine_wave_generator/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --include_path="C:/ti/ccsv8/C6000/dsk6713/include" --include_path="C:/ti/ccsv8/C6000/csl/include" --define=c6713 --define="_DEBUG" --define="CHIP_6713" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="dsp_bios_cfg.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

dsp_bios_cfg_c.obj: ./dsp_bios_cfg_c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="H:/RTDSPlab/lab2/Sine_wave_generator" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.23/include" --include_path="H:/RTDSPlab/lab2/Sine_wave_generator/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --include_path="C:/ti/ccsv8/C6000/dsk6713/include" --include_path="C:/ti/ccsv8/C6000/csl/include" --define=c6713 --define="_DEBUG" --define="CHIP_6713" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="dsp_bios_cfg_c.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sine.obj: ../sine.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="H:/RTDSPlab/lab2/Sine_wave_generator" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.23/include" --include_path="H:/RTDSPlab/lab2/Sine_wave_generator/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --include_path="C:/ti/ccsv8/C6000/dsk6713/include" --include_path="C:/ti/ccsv8/C6000/csl/include" --define=c6713 --define="_DEBUG" --define="CHIP_6713" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="sine.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


