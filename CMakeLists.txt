cmake_minimum_required(VERSION 3.13)

# Set up environment variables for devkitPro
if (NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
endif ()

if (NOT DEFINED ENV{DEVKITARM})
    set(ENV{DEVKITARM} $ENV{DEVKITPRO}/devkitARM)
endif ()

# Set up the toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR armv6k)
set(CMAKE_C_COMPILER $ENV{DEVKITARM}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER $ENV{DEVKITARM}/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER $ENV{DEVKITARM}/bin/arm-none-eabi-gcc)
set(CMAKE_AR $ENV{DEVKITARM}/bin/arm-none-eabi-ar)
set(CMAKE_RANLIB $ENV{DEVKITARM}/bin/arm-none-eabi-ranlib)
set(CMAKE_STRIP $ENV{DEVKITARM}/bin/arm-none-eabi-strip)

# Prevent CMake from testing the compiler (which would fail for cross-compilation)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Project definition
project(HexGuess LANGUAGES C CXX ASM)
set(PROJECT_DESCRIPTION "Template project")
set(PROJECT_AUTHOR "LaDaniWapa")

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Architecture and system defines
add_definitions(-D__3DS__)

# Include directories
include_directories(
        $ENV{DEVKITPRO}/libctru/include
        $ENV{DEVKITPRO}/portlibs/3ds/include
)

# Library directories
link_directories(
        $ENV{DEVKITPRO}/libctru/lib
        $ENV{DEVKITPRO}/portlibs/3ds/lib
)

# Compiler flags for 3DS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft")

# Common flags for both C and C++
set(COMMON_FLAGS "-O2 -mword-relocations -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-specs=3dsx.specs -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -Wl,--gc-sections")

# Define your source files
set(SOURCES
        src/main.cpp
        src/Shake128.cpp
        src/Helper.cpp
        src/HexGuess.cpp
        src/StatusBar.cpp
)

# Create SMDH file from PNG icon
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/icon.smdh
        COMMAND $ENV{DEVKITPRO}/tools/bin/smdhtool --create "${PROJECT_NAME}" "${PROJECT_DESCRIPTION}" "${PROJECT_AUTHOR}" ${CMAKE_SOURCE_DIR}/assets/icon.png ${CMAKE_BINARY_DIR}/icon.smdh
        DEPENDS ${CMAKE_SOURCE_DIR}/assets/icon.png
        COMMENT "Creating icon.smdh"
)

# Create BNR file from PNG icon and WAV sound
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/banner.bnr
        COMMAND $ENV{DEVKITPRO}/tools/bin/bannertool makebanner -i ${CMAKE_SOURCE_DIR}/assets/banner.png -a ${CMAKE_SOURCE_DIR}/assets/sound.wav -o ${CMAKE_BINARY_DIR}/banner.bnr
        DEPENDS ${CMAKE_SOURCE_DIR}/assets/banner.png ${CMAKE_SOURCE_DIR}/assets/sound.wav
        COMMENT "Creating banner.bnr"
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries (order matters!)
target_link_libraries(${PROJECT_NAME}
        citro2d
        citro3d
        ctru
        m
)

# Custom target to create .3dsx file
add_custom_target(3dsx ALL
        COMMAND $ENV{DEVKITPRO}/tools/bin/3dsxtool ${PROJECT_NAME} ${PROJECT_NAME}.3dsx --smdh=${CMAKE_BINARY_DIR}/icon.smdh
        DEPENDS ${PROJECT_NAME} ${CMAKE_BINARY_DIR}/icon.smdh
        COMMENT "Creating .3dsx file"
)

# Optional: Create .cia file (if you have makerom)
add_custom_target(cia ALL
        COMMAND $ENV{DEVKITPRO}/tools/bin/makerom -f cia -o ${PROJECT_NAME}.cia -rsf ${CMAKE_SOURCE_DIR}/assets/app.rsf -icon ${CMAKE_BINARY_DIR}/icon.smdh -banner ${CMAKE_BINARY_DIR}/banner.bnr -target t -exefslogo -elf ${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME} ${CMAKE_BINARY_DIR}/icon.smdh ${CMAKE_BINARY_DIR}/banner.bnr
        COMMENT "Creating .cia file"
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# For CLion to recognize the headers
target_include_directories(${PROJECT_NAME} PRIVATE
        $ENV{DEVKITPRO}/libctru/include
        $ENV{DEVKITPRO}/portlibs/3ds/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)