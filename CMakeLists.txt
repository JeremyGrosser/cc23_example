cmake_minimum_required(VERSION 3.21.3)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi.cmake)

project(example C)

include(ExternalProject)
ExternalProject_Add(simplelink
    GIT_REPOSITORY      https://github.com/TexasInstruments/simplelink-lowpower-f3-sdk
    GIT_TAG             lpf3-8.10.01.02
    GIT_SHALLOW         true
    GIT_PROGRESS        true
    SOURCE_DIR          simplelink-src
    BINARY_DIR          simplelink-build
    CMAKE_ARGS          "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/simplelink-src/source/cmake/toolchains/gcc.cmake"
    BUILD_BYPRODUCTS    ${SOURCE_DIR}/source/ti/devices/cc23x0r5/startup_files/startup_gcc.c
                        ${SOURCE_DIR}/source/ti/devices/cc23x0r5/driverlib/interrupt.c
                        ${SOURCE_DIR}/source/ti/devices/cc23x0r5/driverlib/setup.c
)
ExternalProject_Get_Property(simplelink SOURCE_DIR BINARY_DIR)

set_source_files_properties(
    ${SOURCE_DIR}/source/ti/devices/cc23x0r5/startup_files/startup_gcc.c
    ${SOURCE_DIR}/source/ti/devices/cc23x0r5/driverlib/interrupt.c
    ${SOURCE_DIR}/source/ti/devices/cc23x0r5/driverlib/setup.c
    PROPERTIES GENERATED TRUE
)

add_executable(example
    ${SOURCE_DIR}/source/ti/devices/cc23x0r5/startup_files/startup_gcc.c
    ${SOURCE_DIR}/source/ti/devices/cc23x0r5/driverlib/interrupt.c
    ${SOURCE_DIR}/source/ti/devices/cc23x0r5/driverlib/setup.c
    ti_drivers_config.c
    main.c
)

target_compile_options(example PRIVATE
    -DDeviceFamily_CC23X0R5=1
    -mcpu=cortex-m0plus
    -mthumb
    -ffunction-sections
    -fdata-sections
    -Os
    -g
)

add_library(drivers_cc23x0r5 STATIC IMPORTED)
set_target_properties(drivers_cc23x0r5 PROPERTIES IMPORTED_LOCATION
    ${BINARY_DIR}/source/ti/drivers/lib/gcc/m0p/drivers_cc23x0r5/drivers_cc23x0r5.a
)
add_library(driverlib STATIC IMPORTED)
set_target_properties(driverlib PROPERTIES IMPORTED_LOCATION
    ${BINARY_DIR}/source/ti/devices/cc23x0r5/lib/gcc/m0p/driverlib/driverlib.a
)
add_library(nortos STATIC IMPORTED)
set_target_properties(nortos PROPERTIES IMPORTED_LOCATION
    ${BINARY_DIR}/kernel/nortos/lib/gcc/m0p/nortos_cc23x0r5/nortos_cc23x0r5.a
)
target_link_libraries(example
    drivers_cc23x0r5 driverlib nortos
)

target_include_directories(example PRIVATE
    ${SOURCE_DIR}/source
)

target_link_options(example PRIVATE
    -Wl,-T${SOURCE_DIR}/source/ti/devices/cc23x0r5/linker_files/cc23x0r5.lds
    -Wl,-gc-sections
    -nostartfiles
    #    -specs=nano.specs
)

