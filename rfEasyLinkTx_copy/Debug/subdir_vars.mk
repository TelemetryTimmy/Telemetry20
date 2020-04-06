################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../CC1352P1_LAUNCHXL_TIRTOS.cmd 

SYSCFG_SRCS += \
../rfEasyLinkTx.syscfg 

C_SRCS += \
../CC1352P1_LAUNCHXL_fxns.c \
../mainThread.c \
../main_tirtos.c \
../rfEasyLinkTx.c \
./syscfg/ti_easylink_config.c \
./syscfg/ti_devices_config.c \
./syscfg/ti_radio_config.c \
./syscfg/ti_drivers_config.c \
../rfThread.c \
../uartThread.c 

GEN_FILES += \
./syscfg/ti_easylink_config.c \
./syscfg/ti_devices_config.c \
./syscfg/ti_radio_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./CC1352P1_LAUNCHXL_fxns.d \
./mainThread.d \
./main_tirtos.d \
./rfEasyLinkTx.d \
./syscfg/ti_easylink_config.d \
./syscfg/ti_devices_config.d \
./syscfg/ti_radio_config.d \
./syscfg/ti_drivers_config.d \
./rfThread.d \
./uartThread.d 

OBJS += \
./CC1352P1_LAUNCHXL_fxns.obj \
./mainThread.obj \
./main_tirtos.obj \
./rfEasyLinkTx.obj \
./syscfg/ti_easylink_config.obj \
./syscfg/ti_devices_config.obj \
./syscfg/ti_radio_config.obj \
./syscfg/ti_drivers_config.obj \
./rfThread.obj \
./uartThread.obj 

GEN_MISC_FILES += \
./syscfg/ti_easylink_config.h \
./syscfg/ti_radio_config.h \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"CC1352P1_LAUNCHXL_fxns.obj" \
"mainThread.obj" \
"main_tirtos.obj" \
"rfEasyLinkTx.obj" \
"syscfg\ti_easylink_config.obj" \
"syscfg\ti_devices_config.obj" \
"syscfg\ti_radio_config.obj" \
"syscfg\ti_drivers_config.obj" \
"rfThread.obj" \
"uartThread.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_easylink_config.h" \
"syscfg\ti_radio_config.h" \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"CC1352P1_LAUNCHXL_fxns.d" \
"mainThread.d" \
"main_tirtos.d" \
"rfEasyLinkTx.d" \
"syscfg\ti_easylink_config.d" \
"syscfg\ti_devices_config.d" \
"syscfg\ti_radio_config.d" \
"syscfg\ti_drivers_config.d" \
"rfThread.d" \
"uartThread.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_easylink_config.c" \
"syscfg\ti_devices_config.c" \
"syscfg\ti_radio_config.c" \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../CC1352P1_LAUNCHXL_fxns.c" \
"../mainThread.c" \
"../main_tirtos.c" \
"../rfEasyLinkTx.c" \
"./syscfg/ti_easylink_config.c" \
"./syscfg/ti_devices_config.c" \
"./syscfg/ti_radio_config.c" \
"./syscfg/ti_drivers_config.c" \
"../rfThread.c" \
"../uartThread.c" 

SYSCFG_SRCS__QUOTED += \
"../rfEasyLinkTx.syscfg" 


