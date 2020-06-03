cmake_minimum_required(VERSION 2.8)
project(chaos_driver_modbus)
find_package(chaos MODULE REQUIRED)
include_directories(../../ ${chaos_INCLUDE_DIRS})

SET(CHAOS_CUS core/ModbusControlUnit.cpp)

SET(modbusdriver_src core/modbusDriver.cpp core/ChaosModbusInterface.cpp ${CHAOS_CUS})
SET(modbuscu_src test/main.cpp )



IF(BUILD_FORCE_STATIC)
  ADD_LIBRARY(${PROJECT_NAME} STATIC ${modbusdriver_src})
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -Wl,--whole-archive -lchaos_common -Wl,--no-whole-archive")
ELSE()
  ADD_LIBRARY(${PROJECT_NAME} SHARED ${modbusdriver_src})
ENDIF()

  
TARGET_LINK_LIBRARIES(${PROJECT_NAME} chaos_cutoolkit common_modbus modbus ${FrameworkLib})
SET(DRIVERS_LIB ${PROJECT_NAME})
ADD_SUBDIRECTORY(models)

ADD_EXECUTABLE(modbusControlUnit ${modbuscu_src})
TARGET_LINK_LIBRARIES(modbusControlUnit ${DRIVERS_LIB} chaos_cutoolkit common_modbus chaos_common modbus ${FrameworkLib})
FILE(GLOB files "core/*.h")
INSTALL(FILES ${files} DESTINATION include/driver/modbus/core)

#INSTALL_TARGETS(/bin modbusControlUnit)
INSTALL(TARGETS modbusControlUnit
        DESTINATION "bin"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
INSTALL(TARGETS ${PROJECT_NAME}
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

