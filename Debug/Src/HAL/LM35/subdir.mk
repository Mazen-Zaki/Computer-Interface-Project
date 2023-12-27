################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/HAL/LM35/LM35_program.c 

OBJS += \
./Src/HAL/LM35/LM35_program.o 

C_DEPS += \
./Src/HAL/LM35/LM35_program.d 


# Each subdirectory must supply rules for building sources it contributes
Src/HAL/LM35/%.o Src/HAL/LM35/%.su Src/HAL/LM35/%.cyclo: ../Src/HAL/LM35/%.c Src/HAL/LM35/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F401RCTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-HAL-2f-LM35

clean-Src-2f-HAL-2f-LM35:
	-$(RM) ./Src/HAL/LM35/LM35_program.cyclo ./Src/HAL/LM35/LM35_program.d ./Src/HAL/LM35/LM35_program.o ./Src/HAL/LM35/LM35_program.su

.PHONY: clean-Src-2f-HAL-2f-LM35

