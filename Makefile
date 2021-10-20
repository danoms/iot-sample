CXXFLAGS += -D DEBUG

CPPFLAGS += -specs=nosys.specs
LDFLAGS += -Wl,-LLinker,-T ./Linker/STM32F746NGHx_FLASH.ld

CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -g

ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
    CXXFLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
endif

CXXFLAGS += -D STM32F746xx

CXXFLAGS += -I Inc
CXXFLAGS += -I ./STMicroelectronics/cmsis_device_f7/Include/
CXXFLAGS += -I ./STMicroelectronics/STM32CubeF7/Drivers/CMSIS/Include

# Flags - Machine-dependant options
CXXFLAGS += -mcpu=cortex-m7
CXXFLAGS += -march=armv7e-m+fp.dp



out/bin: out/elf
	arm-none-eabi-objcopy -O binary $^ $@

out/elf: Src/* ./STMicroelectronics/cmsis_device_f7/Source/Templates/system_stm32f7xx.c out/startup.o
	arm-none-eabi-gcc $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

out/startup.o: ./STMicroelectronics/cmsis_device_f7/Source/Templates/gcc/startup_stm32f746xx.s
	@mkdir -p $(@D)
	arm-none-eabi-gcc -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

.phony: clean mfd
clean:
	- rm -f out/*

# make, flash, debug
mfd:
	make
	st-flash --connect-under-reset write out/bin 0x8000000
	st-util
