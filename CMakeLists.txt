cmake_minimum_required(VERSION 3.10)
project(neoembux)

set(BOARD "None" CACHE STRING "Select board type")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)

if(${BOARD} STREQUAL "FORLINX_ELF_2")
    message("Selected board: Forlinx ELF 2")
    add_definitions(-DEMBUX_BOARD_FORLINX_ELF_2)
elseif(${BOARD} STREQUAL "ORANGEPI_5_PLUS")
    message("Selected board: Orange Pi 5 Plus")
    add_definitions(-DEMBUX_BOARD_ORANGEPI_5_PLUS)
else()
    message(FATAL_ERROR "No board has been found in list! BOARD=${BOARD}")
endif()

add_library(neoembux SHARED 
    src/lib/ux_gpio.cpp
    src/lib/ux_softirq.cpp
)

add_executable(embux src/app/embux.cpp)

target_link_libraries(embux PRIVATE
    neoembux
    ${GPIOD_LIBRARIES}
)

target_include_directories(embux PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${GPIOD_INCLUDE_DIRS}
)

set_target_properties(neoembux PROPERTIES VERSION 1.0.0 SOVERSION 1)

target_include_directories(neoembux PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${GPIOD_INCLUDE_DIRS}
)

target_link_libraries(neoembux PRIVATE ${GPIOD_LIBRARIES})

# 安装部分兼容 buildroot external
include(GNUInstallDirs)

if(NOT CMAKE_INSTALL_PREFIX)
    message(FATAL_ERROR "ERROR: CMAKE_INSTALL_PREFIX is not defined.")
endif()

# Buildroot external 推荐用 CMAKE_INSTALL_FULL_LIBDIR 判断
if(DEFINED CMAKE_INSTALL_FULL_LIBDIR)
    install(TARGETS neoembux
        LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
    )
    install(TARGETS embux
        RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
    )
else()
    install(TARGETS neoembux
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    install(TARGETS embux
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# 测试程序
# add_executable(neoembux_test test/test.cpp)
# target_link_libraries(neoembux_test PRIVATE neoembux)