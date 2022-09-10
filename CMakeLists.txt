cmake_minimum_required(VERSION 3.21)
project(Journey_Detour)

set(CMAKE_CXX_STANDARD 17)

link_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(Journey_Detour SHARED main.cpp menu.cpp menu.h sharedlibrary.h lua.cpp lua.h hookutils.cpp hookutils.h game.cpp game.h console.cpp console.h utils.cpp utils.h render.cpp render.h)
# add_executable(Journey_Detour main.cpp menu.cpp menu.h sharedlibrary.h lua.cpp lua.h hookutils.cpp hookutils.h game.cpp game.h console.cpp console.h utils.cpp utils.h)
