################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../atmega


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../atmega/adc_atmega.c \
../atmega/gpio_atmega.c \
../atmega/spi_atmega.c \
../atmega/systick_atmega.c \
../adc.c \
../atmega/timer_atmega.c \
../atmega/usart_atmega.c \
../blinky.c \
../echo.c \
../gpio.c \
../display.c \
../list.c \
../producer_consumer_demo.c \
../pwm.c \
../ring_buffer.c \
../task_mutex.c \
../timer.c \
../heap.c \
../main.c \
../uart.c \
../system.c \
../systick.c \
../task.c \
../vt100.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
atmega/adc_atmega.o \
atmega/gpio_atmega.o \
atmega/spi_atmega.o \
atmega/systick_atmega.o \
adc.o \
atmega/timer_atmega.o \
atmega/usart_atmega.o \
blinky.o \
echo.o \
gpio.o \
display.o \
list.o \
producer_consumer_demo.o \
pwm.o \
ring_buffer.o \
task_mutex.o \
timer.o \
heap.o \
main.o \
uart.o \
system.o \
systick.o \
task.o \
vt100.o

OBJS_AS_ARGS +=  \
atmega/adc_atmega.o \
atmega/gpio_atmega.o \
atmega/spi_atmega.o \
atmega/systick_atmega.o \
adc.o \
atmega/timer_atmega.o \
atmega/usart_atmega.o \
blinky.o \
echo.o \
gpio.o \
display.o \
list.o \
producer_consumer_demo.o \
pwm.o \
ring_buffer.o \
task_mutex.o \
timer.o \
heap.o \
main.o \
uart.o \
system.o \
systick.o \
task.o \
vt100.o

C_DEPS +=  \
atmega/adc_atmega.d \
atmega/gpio_atmega.d \
atmega/spi_atmega.d \
atmega/systick_atmega.d \
adc.d \
atmega/timer_atmega.d \
atmega/usart_atmega.d \
blinky.d \
echo.d \
gpio.d \
display.d \
list.d \
producer_consumer_demo.d \
pwm.d \
ring_buffer.d \
task_mutex.d \
timer.d \
heap.d \
main.d \
uart.d \
system.d \
systick.d \
task.d \
vt100.d

C_DEPS_AS_ARGS +=  \
atmega/adc_atmega.d \
atmega/gpio_atmega.d \
atmega/spi_atmega.d \
atmega/systick_atmega.d \
adc.d \
atmega/timer_atmega.d \
atmega/usart_atmega.d \
blinky.d \
echo.d \
gpio.d \
display.d \
list.d \
producer_consumer_demo.d \
pwm.d \
ring_buffer.d \
task_mutex.d \
timer.d \
heap.d \
main.d \
uart.d \
system.d \
systick.d \
task.d \
vt100.d

OUTPUT_FILE_PATH +=SCTS.elf

OUTPUT_FILE_PATH_AS_ARGS +=SCTS.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler

















































atmega/%.o: ../atmega/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega324p -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega324p -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="SCTS.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega324p 
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "SCTS.elf" "SCTS.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "SCTS.elf" "SCTS.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "SCTS.elf" > "SCTS.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "SCTS.elf" "SCTS.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-size.exe" "SCTS.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SCTS.elf" "SCTS.a" "SCTS.hex" "SCTS.lss" "SCTS.eep" "SCTS.map" "SCTS.srec"
	