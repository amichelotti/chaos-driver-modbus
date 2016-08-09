cmake_minimum_required(VERSION 2.8)
project(chaos_driver_modbus_femto)
SET(CHAOS_CUS ModbusFemtoUnit.cpp)

SET(modbusdriver_src ${CHAOS_CUS})

ADD_LIBRARY(${PROJECT_NAME} STATIC ${modbusdriver_src})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} chaos_driver_modbus chaos_cutoolkit common_modbus modbus chaos_common pthread)
SET(DRIVERS_LIB ${DRIVERS_LIB} ${PROJECT_NAME} PARENT_SCOPE)

INSTALL_TARGETS(/lib ${PROJECT_NAME})

