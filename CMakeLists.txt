cmake_minimum_required(VERSION 2.6)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)

SET(modbusdriver_src core/modbusDriver.cpp core/ChaosModbusInterface.cpp)
SET(modbuscu_src source/main.cpp source/ModbusControlUnit.cpp source/ModbusFemtoUnit.cpp ${modbusdriver_src})

ADD_EXECUTABLE(modbusControlUnit ${modbuscu_src})

IF(BUILD_FORCE_STATIC)
  ADD_LIBRARY(chaos_modbus STATIC ${modbusdriver_src})
  SET(CMAKE_EXE_LINKER_FLAGS "-static -Wl,--whole-archive -lchaos_common -Wl,--no-whole-archive")
  TARGET_LINK_LIBRARIES(modbusControlUnit chaos_cutoolkit common_modbus modbus ${FrameworkLib})
  TARGET_LINK_LIBRARIES(chaos_modbus chaos_cutoolkit common_modbus modbus ${FrameworkLib})
ELSE()
  
  ADD_LIBRARY(chaos_modbus SHARED ${modbusdriver_src})
  TARGET_LINK_LIBRARIES(modbusControlUnit chaos_cutoolkit common_modbus modbus chaos_common ${FrameworkLib})
  TARGET_LINK_LIBRARIES(chaos_modbus chaos_cutoolkit common_modbus modbus chaos_common ${FrameworkLib})
ENDIF()



INSTALL_TARGETS(/bin modbusControlUnit)
INSTALL_TARGETS(/lib chaos_modbus)

FILE(GLOB headers_core "core/*.h")
FILE(GLOB headers "source/*.h")
INSTALL(FILES ${headers_core} DESTINATION include/driver/modbus/core)
INSTALL(FILES ${headers} DESTINATION include/driver/modbus)
