################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ESCOOTER/Cruise_Control.c \
../ESCOOTER/ESCOOTER_BrakeAndThrottle.c \
../ESCOOTER/ESCOOTER_DRIVING.c \
../ESCOOTER/ESCOOTER_MainTask.c \
../ESCOOTER/ESCOOTER_MonitorTask.c \
../ESCOOTER/escooter_config.c \
../ESCOOTER/escooter_control.c 

OBJS += \
./ESCOOTER/Cruise_Control.o \
./ESCOOTER/ESCOOTER_BrakeAndThrottle.o \
./ESCOOTER/ESCOOTER_DRIVING.o \
./ESCOOTER/ESCOOTER_MainTask.o \
./ESCOOTER/ESCOOTER_MonitorTask.o \
./ESCOOTER/escooter_config.o \
./ESCOOTER/escooter_control.o 

C_DEPS += \
./ESCOOTER/Cruise_Control.d \
./ESCOOTER/ESCOOTER_BrakeAndThrottle.d \
./ESCOOTER/ESCOOTER_DRIVING.d \
./ESCOOTER/ESCOOTER_MainTask.d \
./ESCOOTER/ESCOOTER_MonitorTask.d \
./ESCOOTER/escooter_config.d \
./ESCOOTER/escooter_control.d 


# Each subdirectory must supply rules for building sources it contributes
ESCOOTER/%.o ESCOOTER/%.su ESCOOTER/%.cyclo: ../ESCOOTER/%.c ESCOOTER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../../Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/MCLib/Any/Inc -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/MCLib/F4xx/Inc -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/UILibrary/Inc -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/SystemDriveParams -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/DSP/Include -I"C:/Users/Public/Documents/Motor_Control/MOTOR_GO/ADVANCED_HUB_MOTOR/STM32CubeIDE/ESCOOTER" -I"C:/Users/Public/Documents/Motor_Control/MOTOR_GO/ADVANCED_HUB_MOTOR/STM32CubeIDE/POWER_CONTROL" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ESCOOTER

clean-ESCOOTER:
	-$(RM) ./ESCOOTER/Cruise_Control.cyclo ./ESCOOTER/Cruise_Control.d ./ESCOOTER/Cruise_Control.o ./ESCOOTER/Cruise_Control.su ./ESCOOTER/ESCOOTER_BrakeAndThrottle.cyclo ./ESCOOTER/ESCOOTER_BrakeAndThrottle.d ./ESCOOTER/ESCOOTER_BrakeAndThrottle.o ./ESCOOTER/ESCOOTER_BrakeAndThrottle.su ./ESCOOTER/ESCOOTER_DRIVING.cyclo ./ESCOOTER/ESCOOTER_DRIVING.d ./ESCOOTER/ESCOOTER_DRIVING.o ./ESCOOTER/ESCOOTER_DRIVING.su ./ESCOOTER/ESCOOTER_MainTask.cyclo ./ESCOOTER/ESCOOTER_MainTask.d ./ESCOOTER/ESCOOTER_MainTask.o ./ESCOOTER/ESCOOTER_MainTask.su ./ESCOOTER/ESCOOTER_MonitorTask.cyclo ./ESCOOTER/ESCOOTER_MonitorTask.d ./ESCOOTER/ESCOOTER_MonitorTask.o ./ESCOOTER/ESCOOTER_MonitorTask.su ./ESCOOTER/escooter_config.cyclo ./ESCOOTER/escooter_config.d ./ESCOOTER/escooter_config.o ./ESCOOTER/escooter_config.su ./ESCOOTER/escooter_control.cyclo ./ESCOOTER/escooter_control.d ./ESCOOTER/escooter_control.o ./ESCOOTER/escooter_control.su

.PHONY: clean-ESCOOTER

