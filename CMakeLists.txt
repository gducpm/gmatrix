cmake_minimum_required(VERSION 3.10)
project(gmatrix VERSION 1.0.0 LANGUAGES C)

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/generated/version.h")

add_executable(gmatrix src/main.c src/opts.c src/program_info.c src/buffer_ops.c src/matrix_frame.c src/def.h)

set(CPACK_PACKAGE_NAME "GMatrix")
set(CPACK_PACKAGE_VENDOR "GDucpm")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A fast and lightweight terminal entertainment program for Matrix rain")

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

# Only for MacOS
if(WIN32)
	set(CPACK_GENERATOR "NSIS64;ZIP")
elseif(APPLE)
	set(CPACK_GENERATOR "pkgbuild;DragNDrop")
else()
	set(CPACK_GENERATOR "DEB;RPM;TGZ;TXZ;TZST")
endif()

include(CPack)

install(TARGETS gmatrix DESTINATION bin)
