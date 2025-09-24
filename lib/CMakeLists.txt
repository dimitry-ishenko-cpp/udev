# lib

include(GNUInstallDirs)

####################
set(HEADERS
    udev++/device.hpp
    udev++/enumerate.hpp
    udev++/monitor.hpp
    udev++/udev.hpp
)
set(OVERALL_HEADER udev++.hpp)

set(SOURCES
    device.cpp
    enumerate.cpp
    monitor.cpp
    udev.cpp
)

set(DEPENDS )

####################
set(name ${PROJECT_NAME})

add_library(${name}_objects OBJECT ${HEADERS} ${SOURCES})
set_target_properties(${name}_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${name}_objects PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${name}_objects PRIVATE ${DEPENDS})

##
add_library(${name}_static STATIC $<TARGET_OBJECTS:${name}_objects>)
set_target_properties(${name}_static PROPERTIES
    OUTPUT_NAME ${name}
)
target_include_directories(${name}_static INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${name}_static INTERFACE ${DEPENDS})

##
add_library(${name}_shared SHARED $<TARGET_OBJECTS:${name}_objects>)
set_target_properties(${name}_shared PROPERTIES
    OUTPUT_NAME ${name}
    VERSION     ${PROJECT_VERSION}
    SOVERSION   ${PROJECT_VERSION_MAJOR}
)
target_include_directories(${name}_shared INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${name}_shared PRIVATE ${DEPENDS})

##
add_library(${name}::${name}_static ALIAS ${name}_static)
add_library(${name}::${name}_shared ALIAS ${name}_shared)
add_library(${name}::${name} ALIAS ${name}_shared)

####################
install(TARGETS ${name}_shared ${name}_static
    EXPORT ${name}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${name})
install(FILES ${OVERALL_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${name}Targets
    FILE ${name}Targets.cmake
    NAMESPACE ${name}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${name}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${name}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${name}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${name}ConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${name}
)
