cmake_minimum_required(VERSION 3.10)
project(gmatrix VERSION 1.0.0 LANGUAGES C)

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/generated/version.h")

add_executable(gmatrix src/main.c src/opts.c src/program_info.c src/buffer_ops.c src/matrix_frame.c src/def.h)

if(WIN32)
    find_package(unofficial-pdcurses CONFIG REQUIRED)
    find_package(getopt CONFIG REQUIRED)
    target_link_libraries(gmatrix PRIVATE unofficial::pdcurses::pdcurses $<IF:$<TARGET_EXISTS:getopt::getopt_shared>,getopt::getopt_shared,getopt::getopt_static>)
    target_include_directories(gmatrix PRIVATE "${PROJECT_BINARY_DIR}/generated")
    target_compile_options(gmatrix PRIVATE /W4 /WX /O2 /wd4996)
else()
    find_package(Curses REQUIRED)
    target_link_libraries(gmatrix PRIVATE ${CURSES_LIBRARIES})
    target_include_directories(gmatrix PRIVATE "${PROJECT_BINARY_DIR}/generated" "${CURSES_INCLUDE_DIR}")
    
    target_compile_options(gmatrix PRIVATE -O3 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror)
endif()

install(TARGETS gmatrix DESTINATION bin)
