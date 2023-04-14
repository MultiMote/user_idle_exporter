cmake_minimum_required(VERSION 3.0.0)
project(user_idle_exporter VERSION 0.1)

option(HIDE_CONSOLE "Hides console window" ON)
set(LISTEN_ADDR "0.0.0.0:9183" CACHE STRING "Listen address")

if(NOT WIN32)
message(FATAL_ERROR "Only Windows supported")
endif()

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

if(HIDE_CONSOLE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

# strip executable
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -s")

include_directories(mongoose)
add_executable(user_idle_exporter main.c mongoose/mongoose.c)
target_link_libraries(user_idle_exporter ws2_32)
target_compile_definitions(user_idle_exporter PRIVATE
    -DUSER_IDLE_EXPORTER_VERSION="${CMAKE_PROJECT_VERSION}"
    -DUSER_IDLE_EXPORTER_LISTEN_ADDR="${LISTEN_ADDR}"
)