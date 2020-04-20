################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../CC1352P1_LAUNCHXL_TIRTOS.cmd 

SYSCFG_SRCS += \
../rfEasyLinkRx.syscfg 

C_SRCS += \
../CC1352P1_LAUNCHXL_fxns.c \
../RFCombinded.c \
../main_tirtos.c \
./syscfg/Board.c \
../uartThread.c 

GEN_FILES += \
./syscfg/Board.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./CC1352P1_LAUNCHXL_fxns.d \
./RFCombinded.d \
./main_tirtos.d \
./syscfg/Board.d \
./uartThread.d 

OBJS += \
./CC1352P1_LAUNCHXL_fxns.obj \
./RFCombinded.obj \
./main_tirtos.obj \
./syscfg/Board.obj \
./uartThread.obj 

GEN_MISC_FILES += \
./syscfg/Board.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"CC1352P1_LAUNCHXL_fxns.obj" \
"RFCombinded.obj" \
"main_tirtos.obj" \
"syscfg\Board.obj" \
"uartThread.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\Board.h" 

C_DEPS__QUOTED += \
"CC1352P1_LAUNCHXL_fxns.d" \
"RFCombinded.d" \
"main_tirtos.d" \
"syscfg\Board.d" \
"uartThread.d" 

GEN_FILES__QUOTED += \
"syscfg\Board.c" 

C_SRCS__QUOTED += \
"../CC1352P1_LAUNCHXL_fxns.c" \
"../RFCombinded.c" \
"../main_tirtos.c" \
"./syscfg/Board.c" \
"../uartThread.c" 

SYSCFG_SRCS__QUOTED += \
"../rfEasyLinkRx.syscfg" 


