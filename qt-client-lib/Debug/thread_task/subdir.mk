################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thread_task/l_jn.cpp \
../thread_task/l_mpm.cpp \
../thread_task/rkthread.cpp 

OBJS += \
./thread_task/l_jn.o \
./thread_task/l_mpm.o \
./thread_task/rkthread.o 

CPP_DEPS += \
./thread_task/l_jn.d \
./thread_task/l_mpm.d \
./thread_task/rkthread.d 


# Each subdirectory must supply rules for building sources it contributes
thread_task/%.o: ../thread_task/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/root/workdir/qt-client-lib/thread_task -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


