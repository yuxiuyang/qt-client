################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp \
../netdev.cpp \
../network.cpp \
../nibpmgr.cpp 

OBJS += \
./main.o \
./netdev.o \
./network.o \
./nibpmgr.o 

CPP_DEPS += \
./main.d \
./netdev.d \
./network.d \
./nibpmgr.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/root/fltk-1.1.0b13_ut_x86_release -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


