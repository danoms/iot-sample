##
# Project Title
#
# @file
# @version 0.1

# Flags - Overall Options
CPPFLAGS += -specs=nosys.specs

# # Flags - C Language Options
# CFLAGS += -ffreestanding

# # Flags - C++ Language Options
# CXXFLAGS += -fno-threadsafe-statics
# CXXFLAGS += -fno-rtti
# CXXFLAGS += -fno-exceptions
# CXXFLAGS += -fno-unwind-tables

CPPFLAGS += -Wl,-L./STM32-base/STM32-base/linker,-T./STM32-base/STM32-base/linker/STM32F7xx/STM32F746xG.ld

# Flags - Warning Options
CPPFLAGS += -Wall
CPPFLAGS += -Wextra
# remove deprecated-register warnings
# CPPFLAGS += -Dregister= 

# Flags - Debugging Options
CPPFLAGS += -g

# Flags - Optimization Options
# CPPFLAGS += -ffunction-sections
# CPPFLAGS += -fdata-sections

# Flags - Preprocessor options
CPPFLAGS += -D STM32F746xx 

# Flags - Assembler Options
ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
    CPPFLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
endif

# Flags - Linker Options
# CPPFLAGS += -nostdlib
# CPPFLAGS += -Wl,-L$(BASE_LINKER),-T$(BASE_LINKER)/$(SERIES_FOLDER)/$(DEVICE).ld

# Flags - Directory Options
CPPFLAGS += -I Inc
CPPFLAGS += -I ./Startup
CPPFLAGS += -I ./STM32-base/STM32-base-STM32Cube/CMSIS/ARM/inc

# Flags - Machine-dependant options
CPPFLAGS += -mcpu=cortex-m7
CPPFLAGS += -march=armv7e-m+fp.dp
# CPPFLAGS += -mlittle-endian
# CPPFLAGS += -mthumb
# CPPFLAGS += -masm-syntax-unified


out/bin: out/elf
	arm-none-eabi-objcopy -O binary $^ $@

out/elf: Src/* out/startup.o
	arm-none-eabi-gcc $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

out/startup.o: Startup/STM32F746xx.s 
	arm-none-eabi-gcc -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

.phony: clean
clean:
	- rm -f out/*

# end
