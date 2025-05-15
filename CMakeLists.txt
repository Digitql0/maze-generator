cmake_minimum_required(VERSION 3.16)

# —————————————————————————————————————————————
# Auto‑derive project name from the root directory
# —————————————————————————————————————————————
get_filename_component(PROJECT_NAME "${CMAKE_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} LANGUAGES CXX)

# —————————————————————————————————————————————
# Project‑wide settings
# —————————————————————————————————————————————
# Require C11
set(CMAKE_CXX_STANDARD      11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS     OFF)

# Place all executables in <build>/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin")

# Re‑run CMake when you add/remove source files
cmake_policy(SET CMP0091 NEW)

# —————————————————————————————————————————————
# Dependencies
# —————————————————————————————————————————————
find_package(raylib REQUIRED)

# —————————————————————————————————————————————
# Gather sources
# —————————————————————————————————————————————
file(GLOB_RECURSE PROJECT_SOURCES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# —————————————————————————————————————————————
# Main executable
# —————————————————————————————————————————————
add_executable(${PROJECT_NAME}
  ${PROJECT_SOURCES}
)

# Make include/ “public” so other targets could consume your headers
target_include_directories(${PROJECT_NAME}
    PUBLIC
      "${PROJECT_SOURCE_DIR}/include"
)

# Link against raylib (and any system libs it needs)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
      raylib
      m         # math
      pthread   # threads
      dl        # dynamic loading
      GL        # OpenGL
)

# —————————————————————————————————————————————
# (Optional) Install rules
# —————————————————————————————————————————————
# install(TARGETS ${PROJECT_NAME}
#     RUNTIME DESTINATION bin
# )

