cmake_minimum_required(VERSION 3.5)

project(femto LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(femto main.cpp
	terminal.h terminal.cpp
	editor.h editor.cpp
	utils.h
	textbuffer.h textbuffer.cpp
	formatstring.h formatstring.cpp)

include(GNUInstallDirs)
install(TARGETS femto
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
