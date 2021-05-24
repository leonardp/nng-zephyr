# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/nng)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nng_zephyr)

target_sources(app PRIVATE src/main.c)

target_link_libraries(app PUBLIC nng)
