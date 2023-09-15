################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../POWER_CONTROL/POWER_CONTROL.c \
../POWER_CONTROL/POWER_CONTROL_LL.c 

OBJS += \
./POWER_CONTROL/POWER_CONTROL.o \
./POWER_CONTROL/POWER_CONTROL_LL.o 

C_DEPS += \
./POWER_CONTROL/POWER_CONTROL.d \
./POWER_CONTROL/POWER_CONTROL_LL.d 


# Each subdirectory must supply rules for building sources it contributes
POWER_CONTROL/%.o POWER_CONTROL/%.su POWER_CONTROL/%.cyclo: ../POWER_CONTROL/%.c POWER_CONTROL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../../Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/MCLib/Any/Inc -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/MCLib/F4xx/Inc -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/UILibrary/Inc -I../../MCSDK_v5.4.8-Full/MotorControl/MCSDK/SystemDriveParams -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/DSP/Include -I"C:/Users/Public/Documents/Motor_Control/MOTOR_GO/ADVANCED_HUB_MOTOR/STM32CubeIDE/ESCOOTER" -I"C:/Users/Public/Documents/Motor_Control/MOTOR_GO/ADVANCED_HUB_MOTOR/STM32CubeIDE/POWER_CONTROL" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-POWER_CONTROL

clean-POWER_CONTROL:
	-$(RM) ./POWER_CONTROL/POWER_CONTROL.cyclo ./POWER_CONTROL/POWER_CONTROL.d ./POWER_CONTROL/POWER_CONTROL.o ./POWER_CONTROL/POWER_CONTROL.su ./POWER_CONTROL/POWER_CONTROL_LL.cyclo ./POWER_CONTROL/POWER_CONTROL_LL.d ./POWER_CONTROL/POWER_CONTROL_LL.o ./POWER_CONTROL/POWER_CONTROL_LL.su

.PHONY: clean-POWER_CONTROL

