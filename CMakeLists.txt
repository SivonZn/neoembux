cmake_minimum_required(VERSION 3.10)
project(neoembux)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找gpiod库
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)

# 添加动态库目标 (关键修改)
add_library(neoembux SHARED 
    src/ux_gpio.cpp
)

add_executable(embux src/embux.cpp)

# 链接库和头文件
target_link_libraries(embux PRIVATE
    neoembux       # 链接您的动态库
    ${GPIOD_LIBRARIES}
)

target_include_directories(embux PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${GPIOD_INCLUDE_DIRS}
)

# 设置库的版本号 (可选)
set_target_properties(neoembux PROPERTIES VERSION 1.0.0 SOVERSION 1)

# 包含目录
target_include_directories(neoembux PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${GPIOD_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(neoembux PRIVATE ${GPIOD_LIBRARIES})

# 安装配置 (关键添加)
install(TARGETS neoembux
    LIBRARY DESTINATION lib          # 安装到系统库目录
    ARCHIVE DESTINATION lib          # 静态库位置
    PUBLIC_HEADER DESTINATION include # 头文件安装位置
)

install(TARGETS embux
    RUNTIME DESTINATION bin  # 关键：安装到系统bin目录
)

# 安装头文件 (假设公共头文件在include目录)
install(DIRECTORY include/ DESTINATION include)

# 保留测试程序（可选）
add_executable(neoembux_test test/test.cpp)
target_link_libraries(neoembux_test PRIVATE neoembux)