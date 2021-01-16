# define the project name
project(indi-gamepad C CXX)
cmake_minimum_required(VERSION 2.8)

include(GNUInstallDirs)

# add our cmake_modules folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

# find our required packages
find_package(INDI 1.8.9 REQUIRED)
find_package(Nova REQUIRED)
find_package(ZLIB REQUIRED)
find_package(GSL REQUIRED)
find_package(SDL2 REQUIRED)

# these will be used to set the version number in config.h and our driver's xml file
set(CDRIVER_VERSION_MAJOR 1)
set(CDRIVER_VERSION_MINOR 2)

# do the replacement in the config.h
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# do the replacement in the driver's xml file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/indi_gamepad.xml.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/indi_gamepad.xml
)

# set our include directories to look for header files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${INDI_INCLUDE_DIR} )
include_directories( ${NOVA_INCLUDE_DIR} )
include_directories( ${EV_INCLUDE_DIR} )
include_directories( ${SDL2_INCLUDE_DIR} )

include(CMakeCommon)

# tell cmake to build our executable
add_executable(
    indi_gamepad
    indi_gamepad.cpp
)

# and link it to these libraries
target_link_libraries(
    indi_gamepad
    ${INDI_LIBRARIES}
    ${NOVA_LIBRARIES}
    ${GSL_LIBRARIES}
    ${SDL2_LIBRARY}
)

# tell cmake where to install our executable
install(TARGETS indi_gamepad RUNTIME DESTINATION bin)

# and where to put the driver's xml file.
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/indi_gamepad.xml
    DESTINATION ${INDI_DATA_DIR}
)