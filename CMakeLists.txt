cmake_policy(VERSION 3.0)
cmake_minimum_required(VERSION 3.0)

project(
    indi_gamepad
    VERSION 1.2.0
    DESCRIPTION "INDI Driver for GamePads"
    HOMEPAGE_URL "https://github.com/rickbassham/indi_gamepad"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

# add our cmake_modules folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

# find our required packages
find_package(INDI REQUIRED)
find_package(SDL2 REQUIRED)

# these will be used to set the version number in config.h and our driver's xml file
set(CDRIVER_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(CDRIVER_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
set(CDRIVER_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})

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
    ${SDL2_LIBRARY}
)

if(APPLE)
    install(
        TARGETS indi_gamepad RUNTIME DESTINATION "/Applications/KStars.app/Contents/MacOS/indi"
    )

    install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/indi_gamepad.xml
        DESTINATION "/Applications/KStars.app/Contents/Resources/DriverSupport"
    )
endif(APPLE)

install(TARGETS indi_gamepad RUNTIME DESTINATION bin)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/indi_gamepad.xml
    DESTINATION ${INDI_DATA_DIR}
)

if(UNIX AND NOT APPLE)
    execute_process(
        COMMAND bash -c "cat /etc/*release | grep ^ID= | awk -F= '{printf \"%s\",\$2}'"
        OUTPUT_VARIABLE DISTRIB_ID
    )
    execute_process(
        COMMAND bash -c "cat /etc/*release | grep ^VERSION_CODENAME= | awk -F= '{printf \"%s\",\$2}'"
        OUTPUT_VARIABLE DISTRIB_CODENAME
    )
    execute_process(
        COMMAND bash -c "dpkg-architecture -q DEB_BUILD_ARCH | awk '{printf \"%s\",\$1}'"
        OUTPUT_VARIABLE DISTRIB_ARCH
    )
    set(CPACK_PACKAGE_CONTACT "Rick Bassham brodrick.bassham@gmail.com")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_PACKAGE_SECTION "science")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CMAKE_PROJECT_HOMEPAGE_URL})
    set(CPACK_RPM_PACKAGE_GROUP "Applications/Engineering")
    set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${DISTRIB_ID}_${DISTRIB_CODENAME}_${DISTRIB_ARCH})

endif(UNIX AND NOT APPLE)

set(CPACK_SOURCE_GENERATOR "TGZ")

list(APPEND CPACK_SOURCE_IGNORE_FILES ".DS_Store")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".gitignore")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/build/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/.git/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/.vscode/")

set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION})

include(CPack)
