
set(target basic-painters)
message(STATUS "Example ${target}")


# External libraries

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)


# Includes

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDES}
    ${GLOBJECTS_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/gloperate/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Libraries

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    gloperate
)


# Compiler definitions

if (OPTION_BUILD_STATIC)
    add_definitions("-DBASIC_PAINTERS_STATIC")
else()
    add_definitions("-DBASIC_PAINTERS_EXPORTS")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")


# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
    ${source_path}/textured-quad/TexturedQuad.cpp
    ${source_path}/rotating-quad/RotatingQuad.cpp
    ${source_path}/cubescape/CubeScape.cpp
    ${source_path}/plugin.cpp
)

set(headers
    ${include_path}/textured-quad/TexturedQuad.h
    ${include_path}/rotating-quad/RotatingQuad.h
    ${include_path}/cubescape/CubeScape.h
    ${include_path}/basic_painters_api.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# Build library

add_library(${target} ${headers} ${sources})

target_link_libraries(${target} ${libs})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH                ${include_path})


# Deployment

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_PLUGINS} CONFIGURATIONS Release
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Debug runtime library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_PLUGINS_DEBUG} CONFIGURATIONS Debug
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
