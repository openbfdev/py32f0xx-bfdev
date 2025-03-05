# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(beep main.c beep.c)
target_link_libraries(beep ${CMAKE_PROJECT_NAME})

add_custom_command(
    TARGET beep
    POST_BUILD COMMAND
    ${CMAKE_OBJCOPY} -O binary
    ${CMAKE_CURRENT_BINARY_DIR}/beep
    ${CMAKE_CURRENT_BINARY_DIR}/beep.bin
    POST_BUILD COMMAND
    ${CMAKE_OBJCOPY} -O ihex
    ${CMAKE_CURRENT_BINARY_DIR}/beep
    ${CMAKE_CURRENT_BINARY_DIR}/beep.hex
)
