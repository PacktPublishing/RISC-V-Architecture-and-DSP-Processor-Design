################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../system/fdm_start.s 

C_SRCS += \
../system/RV335_Adc.c \
../system/RV335_GlobalVariableDefs.c \
../system/RV335_SysCtrl.c \
../system/intexc.c 

S_UPPER_SRCS += \
../system/trap.S 

OBJECT_FILE += \
./system/RV335_Adc.o \
./system/RV335_GlobalVariableDefs.o \
./system/RV335_SysCtrl.o \
./system/fdm_start.o \
./system/intexc.o \
./system/trap.o 


# Each subdirectory must supply rules for building sources it contributes
system/%.o: ../system/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"C:\Users\xrz\Desktop\240117\240117\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/%.o: ../system/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"C:\Users\xrz\Desktop\240117\240117\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/%.o: ../system/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"C:\Users\xrz\Desktop\240117\240117\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


