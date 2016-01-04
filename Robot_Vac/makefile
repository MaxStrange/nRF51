
TARGET = build/test
LINKER_SCRIPT=robotvac_gcc_nrf51.ld

LIB_PATH = ../CommonIncludes/components/toolchain/gcc

# Toolchain commands
CC       		:= "arm-none-eabi-gcc"
AS       		:= "arm-none-eabi-as"
AR       		:= "arm-none-eabi-ar" -r
LD       		:= "arm-none-eabi-ld"
NM       		:= "arm-none-eabi-nm"
OBJDUMP  		:= "arm-none-eabi-objdump"
OBJCOPY  		:= "arm-none-eabi-objcopy"
SIZE    		:= "arm-none-eabi-size"


#assembly files common to all targets
ASMSRCS  = ../CommonIncludes/components/toolchain/gcc/gcc_startup_nrf51.s

#includes common to all targets
INC_PATHS += -I../CommonIncludes/bsp
INC_PATHS += -I../CommonIncludes/components/device
INC_PATHS += -I../CommonIncludes/components/drivers_nrf/hal
INC_PATHS += -I../CommonIncludes/components/toolchain
INC_PATHS += -I../CommonIncludes/components/toolchain/gcc

#flags common to all targets
CFLAGS  = -DNRF51
CFLAGS += -DBOARD_PCA10028
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O0
CFLAGS += -mfloat-abi=soft
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
CFLAGS += -g

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(TARGET).map
LDFLAGS += -mthumb -mabi=aapcs -L $(LIB_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys
LDFLAGS += -I../CommonIncludes/components/toolchain/gcc
LDFLAGS += -g

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF51
ASMFLAGS += -DBOARD_PCA10028
ASMFLAGS += -DBSP_DEFINES_ONLY


#default target - first one defined
vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = build/gcc_startup_nrf51.o build/system_nrf51.o build/main.o \
build/circular_queue.o build/console.o build/my_led.o build/my_strings.o \
build/my_uart.o build/nrf_delay.o build/my_lcd.o build/my_timer.o \
build/my_clock.o build/range_finder.o build/servo.o build/user_input.o \
build/my_gpiote.o build/random_numbers.o build/drive_system.o build/bumper.o


$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJECTS)
	@echo Linking target: $(TARGET).bin
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@


# Create objects from C SRC files
build/main.o : main.c
build/circular_queue.o : circular_queue.c
build/console.o : console.c
build/my_led.o : my_led.c
build/my_strings.o : my_strings.c
build/my_uart.o : my_uart.c
build/my_lcd.o : my_lcd.c
build/my_timer.o : my_timer.c
build/my_clock.o : my_timer.c
build/range_finder.o : range_finder.c
build/servo.o : servo.c
build/user_input.o : user_input.c
build/my_gpiote.o : my_gpiote.c
build/random_numbers.o : random_numbers.c
build/drive_system.o : drive_system.c
build/bumper.o : bumper.c

build/system_nrf51.o : ../CommonIncludes/components/toolchain/system_nrf51.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

build/nrf_delay.o : ../CommonIncludes/components/drivers_nrf/hal/nrf_delay.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

build/%.o: %.c
	mkdir -p $(dir $@)
	@echo Compiling file: $(notdir $<)
	$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
build/gcc_startup_nrf51.o: ../CommonIncludes/components/toolchain/gcc/gcc_startup_nrf51.s
	mkdir -p $(dir $@)
	@echo Compiling file: $(notdir $<)
	$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


## Runs the arm gdb server
debug:
	arm-none-eabi-gdb -ex "target ext localhost:2345" build/test.elf

clean:
	rm -rf build
