################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPBPriorityQueue.c \
../SPList.c \
../SPListElement.c \
../SPLogger.c \
../SPPoint.c 

O_SRCS += \
../SPBPriorityQueue.o \
../SPList.o \
../SPListElement.o \
../sp_bpqueue_unit_test.o 

OBJS += \
./SPBPriorityQueue.o \
./SPList.o \
./SPListElement.o \
./SPLogger.o \
./SPPoint.o 

C_DEPS += \
./SPBPriorityQueue.d \
./SPList.d \
./SPListElement.d \
./SPLogger.d \
./SPPoint.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0  -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


