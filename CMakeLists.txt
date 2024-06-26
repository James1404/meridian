cmake_minimum_required(VERSION 3.27.7)

project(meridian VERSION 0.01)

find_package(LLVM REQUIRED CONFIG)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(meridian)

llvm_map_components_to_libnames(llvm_libs support core irreader)

if(MSVC)
  target_compile_options(meridian PRIVATE /W4)
else()
  target_compile_options(meridian PRIVATE -Wall -Wpedantic -Werror)
endif()

set(meridian_CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB meridian_src
	${meridian_CURRENT_DIR}/src/*.cpp
	${meridian_CURRENT_DIR}/src/*.c
)

target_link_libraries(meridian ${llvm_libs})

target_sources(meridian PRIVATE ${meridian_src})

target_include_directories(meridian PUBLIC
    ${meridian_CURRENT_DIR}/include
)

add_custom_command(
    TARGET meridian POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${meridian_CURRENT_DIR}/test
            ${CMAKE_CURRENT_BINARY_DIR}/test)

add_custom_command(
    TARGET meridian POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${meridian_CURRENT_DIR}/lib
            ${CMAKE_CURRENT_BINARY_DIR}/lib)
