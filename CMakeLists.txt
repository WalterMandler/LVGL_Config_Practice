cmake_minimum_required(VERSION 4.1.0)
project(LVGL_Simulator C)
set(CMAKE_C_STANDARD 99)

# Set the locations of the LVGL library and the configuration file.
set(MY_LIBRARIES_DIR "C:/MyCode/ExternalLibraries")
set(MY_LVGL_DIR "${MY_LIBRARIES_DIR}/lvgl")
set(LV_BUILD_CONF_PATH "${PROJECT_SOURCE_DIR}/lv_conf.h")

# Required source files.
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.c")

# Find the SDL2 library. SDL2 will create the variable SDL2_LIBRARIES.
find_package(SDL2 REQUIRED)
message(STATUS "SDL Libraries Found: ${SDL2_LIBRARIES}")

# Add LVGL as a subdirectory. LVGL will create a target library named "lvgl".
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CONFIG_LV_BUILD_DEMOS OFF)
set(CONFIG_LV_BUILD_EXAMPLES OFF)
set(CONFIG_LV_USE_THORVG_INTERNAL OFF)
add_subdirectory(${MY_LVGL_DIR} "${PROJECT_SOURCE_DIR}/build_lvgl")

# Create a target for the main executable.
add_executable(main ${SOURCE_FILES})
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Link the required libraries. The target "main" inherits the include
# directories from each linked library that defined them as INTERFACE or PUBLIC.
# Critically, this seems to include LVGL and SDL2. This means that
# any source files attached to "main" also have access to those include directories.
target_link_libraries(main PRIVATE 
	lvgl
	${SDL2_LIBRARIES}
)

# Local project include directories.
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)
