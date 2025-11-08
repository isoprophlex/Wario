cmake_minimum_required(VERSION 3.16)
project(Shooter25D_Qt_SDL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt6
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# SDL2 + SDL2_ttf
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf)

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIRS}
)

link_directories(
        ${SDL2_LIBRARY_DIRS}
        ${SDL2TTF_LIBRARY_DIRS}
)

# Buscar recursivamente todos los .cpp en src/
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Crear ejecutable
add_executable(Shooter25D_Qt_SDL ${SOURCES})

# Incluir directorios raíz y subcarpetas
target_include_directories(Shooter25D_Qt_SDL PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/qt_ui
        ${CMAKE_SOURCE_DIR}/src/engines
        ${CMAKE_SOURCE_DIR}/src/render
        ${CMAKE_SOURCE_DIR}/src/entities
        ${CMAKE_SOURCE_DIR}/src/threads
)

# Asegurar que Player.cpp existe
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src/entities/Player.cpp")
    message(FATAL_ERROR "Player.cpp no encontrado en src/entities/")
endif()

target_sources(Shooter25D_Qt_SDL PRIVATE
        ${CMAKE_SOURCE_DIR}/src/entities/Player.cpp
)

# Vincular librerías
target_link_libraries(Shooter25D_Qt_SDL PRIVATE
        Qt6::Widgets
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARIES}
)

# Para macOS (Qt instalado vía Homebrew)
if(APPLE)
    message(STATUS "If CMake fails to find Qt, rerun with:")
    message(STATUS "cmake -DCMAKE_PREFIX_PATH=$(brew --prefix qt) ..")
endif()
