# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.9)
project(py32f0 VERSION "1.0" LANGUAGES ASM C)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(PY32_HEADER_PATH ${PROJECT_SOURCE_DIR}/include)

configure_file(
    ${CMAKE_MODULE_PATH}/config.h.in
    ${PROJECT_BINARY_DIR}/generated/config.h
)

set(CMAKE_SYSTEM_PROCESSOR "arm" CACHE STRING "")
set(CMAKE_SYSTEM_NAME "Generic" CACHE STRING "")

set(TOOLCHAIN_PREFIX "arm-none-eabi-")
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
set(CMAKE_AR "${TOOLCHAIN_PREFIX}ar")
set(CMAKE_LINKER "{TOOLCHAIN_PREFIX}ld")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_RANLIB "${TOOLCHAIN_PREFIX}ranlib")
set(CMAKE_SIZE "${TOOLCHAIN_PREFIX}size")
set(CMAKE_STRIP "${TOOLCHAIN_PREFIX}ld")

add_definitions(
    "-fno-builtin"
    "-ffreestanding"
    "--specs=nosys.specs"
    "-D ARM_MATH_CM0PLUS"
    "-D PY32F030x6"
)

set(PY32_CMSIS_PATH ${PROJECT_SOURCE_DIR}/drivers/CMSIS)
set(PY32_HAL_PATH ${PROJECT_SOURCE_DIR}/drivers/PY32F0xx_HAL_Driver)
set(PY32_LDS_PATH ${PROJECT_SOURCE_DIR}/ldscripts)

FILE(GLOB_RECURSE SRC_HEADER "include/*.h")
FILE(GLOB_RECURSE SRC_SOURCE "src/*.c")

FILE(GLOB_RECURSE DRIVERS_HEADER "drivers/*.h")
FILE(GLOB_RECURSE DRIVERS_SOURCE "drivers/*.c")

set(DRIVERS_SOURCE
    ${DRIVERS_SOURCE}
    ${PY32_CMSIS_PATH}/Device/PY32F0xx/Source/gcc/startup_py32f030.s
)

set(PROJECT_FILES
    ${SRC_HEADER}
    ${SRC_SOURCE}
    ${DRIVERS_HEADER}
    ${DRIVERS_SOURCE}
)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/generated)
include_directories(${PY32_CMSIS_PATH}/Include)
include_directories(${PY32_CMSIS_PATH}/Device/PY32F0xx/Include)
include_directories(${PY32_HAL_PATH}/Inc)
include_directories(${PY32_HAL_PATH}/Inc/Legacy)

set(MCPU "-mcpu=cortex-m0plus")
set(MFPU "")
set(MFLOAT_ABI "-mfloat-abi=soft")
set(LINKER_SCRIPT ${PY32_LDS_PATH}/py32f030x6.ld)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} -mthumb")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} -Wl,-Map=test.map --specs=nosys.specs")
add_executable(${PROJECT_NAME} ${PROJECT_FILES})
