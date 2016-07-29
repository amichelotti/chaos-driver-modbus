cmake_minimum_required(VERSION 2.6)
SET(CHAOS_CUS source/ModbusControlUnit.cpp source/ModbusFemtoUnit.cpp)

SET(modbusdriver_src core/modbusDriver.cpp core/ChaosModbusInterface.cpp ${CHAOS_CUS})
SET(modbuscu_src source/main.cpp )

ADD_EXECUTABLE(modbusControlUnit ${modbuscu_src})

IF(BUILD_FORCE_STATIC)
  ADD_LIBRARY(chaos_driver_modbus STATIC ${modbusdriver_src})
  SET(CMAKE_EXE_LINKER_FLAGS "-static -Wl,--whole-archive -lchaos_common -Wl,--no-whole-archive")
  TARGET_LINK_LIBRARIES(modbusControlUnit chaos_driver_modbus chaos_cutoolkit common_modbus modbus ${FrameworkLib})
  TARGET_LINK_LIBRARIES(chaos_driver_modbus chaos_cutoolkit common_modbus modbus ${FrameworkLib})
ELSE()
  
  ADD_LIBRARY(chaos_driver_modbus SHARED ${modbusdriver_src})
  TARGET_LINK_LIBRARIES(modbusControlUnit chaos_driver_modbus chaos_cutoolkit common_modbus modbus chaos_common pthread)
  TARGET_LINK_LIBRARIES(chaos_driver_modbus chaos_cutoolkit common_modbus modbus chaos_common pthread)
ENDIF()



INSTALL_TARGETS(/bin modbusControlUnit)
INSTALL_TARGETS(/lib chaos_driver_modbus)

