################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-612325855:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-612325855-inproc

build-612325855-inproc: ../release.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/ccs930/xdctools_3_60_02_34_core/xs" --xdcpath="C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/source;C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC1352 -r release -c "C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS" --compileOptions " -DDeviceFamily_CC13X2 " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-612325855 ../release.cfg
configPkg/compiler.opt: build-612325855
configPkg/: build-612325855


