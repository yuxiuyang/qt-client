################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../analyseco2.cpp \
../analyseecg.cpp \
../analyseibp.cpp \
../analysenarco.cpp \
../analysenibp.cpp \
../analysespo2.cpp \
../co2mgr.cpp \
../ecgmgr.cpp \
../ibpmgr.cpp \
../main.cpp \
../mgrdev.cpp \
../narcomgr.cpp \
../netdev.cpp \
../network.cpp \
../nibpmgr.cpp \
../spo2mgr.cpp 

OBJS += \
./analyseco2.o \
./analyseecg.o \
./analyseibp.o \
./analysenarco.o \
./analysenibp.o \
./analysespo2.o \
./co2mgr.o \
./ecgmgr.o \
./ibpmgr.o \
./main.o \
./mgrdev.o \
./narcomgr.o \
./netdev.o \
./network.o \
./nibpmgr.o \
./spo2mgr.o 

CPP_DEPS += \
./analyseco2.d \
./analyseecg.d \
./analyseibp.d \
./analysenarco.d \
./analysenibp.d \
./analysespo2.d \
./co2mgr.d \
./ecgmgr.d \
./ibpmgr.d \
./main.d \
./mgrdev.d \
./narcomgr.d \
./netdev.d \
./network.d \
./nibpmgr.d \
./spo2mgr.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/root/fltk-1.3.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


