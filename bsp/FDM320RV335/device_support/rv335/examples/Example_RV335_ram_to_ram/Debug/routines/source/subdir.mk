################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../routines/source/RV335_CpuTimers.c \
../routines/source/RV335_DMA.c \
../routines/source/RV335_DefaultIsr.c \
../routines/source/RV335_ECan.c \
../routines/source/RV335_ECap.c \
../routines/source/RV335_EPwm.c \
../routines/source/RV335_EQep.c \
../routines/source/RV335_Flash_API.c \
../routines/source/RV335_Gpio.c \
../routines/source/RV335_I2C.c \
../routines/source/RV335_Mcbsp.c \
../routines/source/RV335_MemCopy.c \
../routines/source/RV335_Sci.c \
../routines/source/RV335_Spi.c \
../routines/source/RV335_Xintf.c 

OBJECT_FILE += \
./routines/source/RV335_CpuTimers.o \
./routines/source/RV335_DMA.o \
./routines/source/RV335_DefaultIsr.o \
./routines/source/RV335_ECan.o \
./routines/source/RV335_ECap.o \
./routines/source/RV335_EPwm.o \
./routines/source/RV335_EQep.o \
./routines/source/RV335_Flash_API.o \
./routines/source/RV335_Gpio.o \
./routines/source/RV335_I2C.o \
./routines/source/RV335_Mcbsp.o \
./routines/source/RV335_MemCopy.o \
./routines/source/RV335_Sci.o \
./routines/source/RV335_Spi.o \
./routines/source/RV335_Xintf.o 


# Each subdirectory must supply rules for building sources it contributes
routines/source/%.o: ../routines/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"C:\Users\xrz\Desktop\240117\240117\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


