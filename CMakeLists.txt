# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.9)
project(py32f0xx VERSION "1.0" LANGUAGES ASM C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(PY32_HEADER_PATH ${CMAKE_SOURCE_DIR}/include)

configure_file(
    ${CMAKE_MODULE_PATH}/config.h.in
    ${CMAKE_BINARY_DIR}/generated/config.h
)

add_definitions(
    "-fno-builtin"
    "-ffreestanding"
    "-ffunction-sections"
    "-fdata-sections"
    "-D PY32F030x6"
)

option(ENABLE_EXAMPLES "Build examples" OFF)
set(PY32_CMSIS_PATH ${CMAKE_SOURCE_DIR}/drivers/CMSIS)
set(PY32_HAL_PATH ${CMAKE_SOURCE_DIR}/drivers/PY32F0xx_HAL_Driver)
set(PY32_LDS_PATH ${CMAKE_SOURCE_DIR}/ldscripts)

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

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/generated)
include_directories(${CMAKE_SOURCE_DIR}/bfdev/include)
include_directories(${CMAKE_BINARY_DIR}/bfdev/generated)

include_directories(${PY32_CMSIS_PATH}/Include)
include_directories(${PY32_CMSIS_PATH}/Device/PY32F0xx/Include)
include_directories(${PY32_HAL_PATH}/Inc)
include_directories(${PY32_HAL_PATH}/Inc/Legacy)

set(MCPU "-mcpu=cortex-m0plus -mthumb")
set(MFLOAT_ABI "-mfloat-abi=soft")
set(LINKER_SCRIPT ${PY32_LDS_PATH}/py32f030x6.ld)

set(CMAKE_C_FLAGS "${MCPU} ${MFLOAT_ABI} -Os -std=gnu11 --specs=nano.specs")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

set(CMAKE_EXE_LINKER_FLAGS
    "-T ${LINKER_SCRIPT} \
     --specs=nosys.specs \
     -Wl,--gc-sections \
     -Wl,--print-memory-usage \
     -Wl,-Map=${CMAKE_PROJECT_NAME}.map"
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(BFDEV_CRC_EXTEND OFF)
add_subdirectory(bfdev)
add_library(${CMAKE_PROJECT_NAME} OBJECT ${PROJECT_FILES})

if(ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()
