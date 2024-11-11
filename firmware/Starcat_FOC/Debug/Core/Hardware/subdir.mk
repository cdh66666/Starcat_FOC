################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Hardware/DRV8301.c \
../Core/Hardware/FOC.c \
../Core/Hardware/MT6701.c \
../Core/Hardware/NTC.c \
../Core/Hardware/connect.c \
../Core/Hardware/flash.c \
../Core/Hardware/oled.c 

OBJS += \
./Core/Hardware/DRV8301.o \
./Core/Hardware/FOC.o \
./Core/Hardware/MT6701.o \
./Core/Hardware/NTC.o \
./Core/Hardware/connect.o \
./Core/Hardware/flash.o \
./Core/Hardware/oled.o 

C_DEPS += \
./Core/Hardware/DRV8301.d \
./Core/Hardware/FOC.d \
./Core/Hardware/MT6701.d \
./Core/Hardware/NTC.d \
./Core/Hardware/connect.d \
./Core/Hardware/flash.d \
./Core/Hardware/oled.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Hardware/%.o Core/Hardware/%.su Core/Hardware/%.cyclo: ../Core/Hardware/%.c Core/Hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -D__TARGET_FPU_VFP -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/stm32cubeide_cdh/workplace/Starcat_FOC/DSP/Include" -I"E:/stm32cubeide_cdh/workplace/Starcat_FOC/Core/Hardware" -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Hardware

clean-Core-2f-Hardware:
	-$(RM) ./Core/Hardware/DRV8301.cyclo ./Core/Hardware/DRV8301.d ./Core/Hardware/DRV8301.o ./Core/Hardware/DRV8301.su ./Core/Hardware/FOC.cyclo ./Core/Hardware/FOC.d ./Core/Hardware/FOC.o ./Core/Hardware/FOC.su ./Core/Hardware/MT6701.cyclo ./Core/Hardware/MT6701.d ./Core/Hardware/MT6701.o ./Core/Hardware/MT6701.su ./Core/Hardware/NTC.cyclo ./Core/Hardware/NTC.d ./Core/Hardware/NTC.o ./Core/Hardware/NTC.su ./Core/Hardware/connect.cyclo ./Core/Hardware/connect.d ./Core/Hardware/connect.o ./Core/Hardware/connect.su ./Core/Hardware/flash.cyclo ./Core/Hardware/flash.d ./Core/Hardware/flash.o ./Core/Hardware/flash.su ./Core/Hardware/oled.cyclo ./Core/Hardware/oled.d ./Core/Hardware/oled.o ./Core/Hardware/oled.su

.PHONY: clean-Core-2f-Hardware

