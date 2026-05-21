################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/source/fdm_start.s 

C_SRCS += \
../src/source/RV335_Adc.c \
../src/source/RV335_CpuTimers.c \
../src/source/RV335_DMA.c \
../src/source/RV335_DefaultIsr.c \
../src/source/RV335_ECan.c \
../src/source/RV335_ECap.c \
../src/source/RV335_EPwm.c \
../src/source/RV335_EQep.c \
../src/source/RV335_Examples.c \
../src/source/RV335_Flash_API.c \
../src/source/RV335_GlobalVariableDefs.c \
../src/source/RV335_Gpio.c \
../src/source/RV335_I2C.c \
../src/source/RV335_Mcbsp.c \
../src/source/RV335_MemCopy.c \
../src/source/RV335_PieCtrl.c \
../src/source/RV335_PieVect.c \
../src/source/RV335_Sci.c \
../src/source/RV335_Spi.c \
../src/source/RV335_SysCtrl.c \
../src/source/RV335_Xintf.c \
../src/source/intexc.c 

S_UPPER_SRCS += \
../src/source/trap.S 

OBJECT_FILE += \
./src/source/RV335_Adc.o \
./src/source/RV335_CpuTimers.o \
./src/source/RV335_DMA.o \
./src/source/RV335_DefaultIsr.o \
./src/source/RV335_ECan.o \
./src/source/RV335_ECap.o \
./src/source/RV335_EPwm.o \
./src/source/RV335_EQep.o \
./src/source/RV335_Examples.o \
./src/source/RV335_Flash_API.o \
./src/source/RV335_GlobalVariableDefs.o \
./src/source/RV335_Gpio.o \
./src/source/RV335_I2C.o \
./src/source/RV335_Mcbsp.o \
./src/source/RV335_MemCopy.o \
./src/source/RV335_PieCtrl.o \
./src/source/RV335_PieVect.o \
./src/source/RV335_Sci.o \
./src/source/RV335_Spi.o \
./src/source/RV335_SysCtrl.o \
./src/source/RV335_Xintf.o \
./src/source/fdm_start.o \
./src/source/intexc.o \
./src/source/trap.o 


# Each subdirectory must supply rules for building sources it contributes
src/source/%.o: ../src/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"D:\CodeCanvas_2406041022\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -mcpu=springcore-rv32 -mllvm -enable-misched -mllvm -misched-topdown=true -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -I"D:\FDMTEK_RV335\dsp_fastrts\example\rnd_SP_RS\src\include" -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/source/%.o: ../src/source/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\CodeCanvas_2406041022\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/source/%.o: ../src/source/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\CodeCanvas_2406041022\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


