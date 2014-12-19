cmake_minimum_required(VERSION 2.6)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)

SET(modbusdriver_src core/modbusDriver.cpp core/ChaosModbusInterface.cpp)
SET(modbuscu_src source/main.cpp source/ModbusControlUnit.cpp source/ModbusFemtoUnit.cpp ${modbusdriver_src})


IF(BUILD_FORCE_STATIC)
  ADD_LIBRARY(chaos_modbus STATIC ${modbusdriver_src})
ELSE()
  
  ADD_LIBRARY(chaos_modbus SHARED ${modbusdriver_src})
ENDIF()

ADD_EXECUTABLE(modbusControlUnit ${modbuscu_src})



#SET_TARGET_PROPERTIES(chaos_modbus_static PROPERTIES OUTPUT_NAME chaos_modbus)

TARGET_LINK_LIBRARIES(modbusControlUnit chaos_cutoolkit chaos_common common_modbus modbus ${FrameworkLib})
TARGET_LINK_LIBRARIES(chaos_modbus chaos_cutoolkit chaos_common ${FrameworkLib})



INSTALL_TARGETS(/bin modbusControlUnit)
INSTALL_TARGETS(/lib chaos_modbus)

FILE(GLOB headers "core/*.h")
INSTALL(FILES ${headers} DESTINATION include/driver/modbus/core)
