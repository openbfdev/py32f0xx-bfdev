# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(uart main.c)
target_link_libraries(uart ${CMAKE_PROJECT_NAME})

add_custom_command(
    TARGET uart
    POST_BUILD COMMAND
    ${CMAKE_OBJCOPY} -O binary
    ${CMAKE_CURRENT_BINARY_DIR}/uart
    ${CMAKE_CURRENT_BINARY_DIR}/uart.bin
    POST_BUILD COMMAND
    ${CMAKE_OBJCOPY} -O ihex
    ${CMAKE_CURRENT_BINARY_DIR}/uart
    ${CMAKE_CURRENT_BINARY_DIR}/uart.hex
)
