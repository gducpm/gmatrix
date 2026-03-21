cmake_minimum_required(VERSION 3.10)

project(gmatrix VERSION 1.0.0 LANGUAGES C)

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/generated/version.h")

set(CURSES_NEED_NCURSES true)
find_package(Curses REQUIRED)
include_directories("${CURSES_INCLUDE_DIR}" "${PROJECT_BINARY_DIR}/generated")

add_executable(gmatrix src/main.c src/opts.c src/program_info.c src/buffer_ops.c src/matrix_frame.c src/def.h)

target_link_libraries(gmatrix ${CURSES_LIBRARIES})

target_compile_options(gmatrix PRIVATE -O3 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror)

install(TARGETS gmatrix DESTINATION bin)

