cmake_minimum_required(VERSION 3.21)
project(OpenGL VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# include directories
include_directories(
    3rdparty/glfw/include
    3rdparty/glad/include
    3rdparty/glm
    3rdparty/stb
    3rdparty/imgui
    3rdparty/imgui/backends
    3rdparty/assimp/include
)


# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdparty/glfw)


#glm
option(GLM_ENABLE_CXX_17 ON)
add_subdirectory(3rdparty/glm)


#imgui
include(cmake/imgui.cmake)
set(IMGUI_LIB imgui imgui_glfw_opengl3) # cmake/imgui.cmake will create these targets


#assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(3rdparty/assimp)
# todo copy assimp generate header file to src/include


#glad soruce code
set(GLAD_SRC 3rdparty/glad/src/glad.c)



# source code include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/include
)


# source code
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils utils)



file(GLOB CHR1 ${PROJECT_SOURCE_DIR}/src/01_Start/*.cpp)
foreach (file ${CHR1})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe ${file})
    message(exe: ${exe})
    add_executable(${exe} ${file} ${utils} ${GLAD_SRC})

    if (APPLE)
        target_link_libraries(${exe} glfw glm assimp::assimp ${IMGUI_LIB}
            "-framework Cocoa"
            "-framework CoreFoundation"
            "-framework IOKit"
            "-framework CoreVideo"
        )
    elseif(WIN32 OR UNIX)
        target_link_libraries(${exe} glfw glm assimp::assimp ${IMGUI_LIB})
    endif()
endforeach ()



file(GLOB CHR2 ${PROJECT_SOURCE_DIR}/src/02_Light/*.cpp)
foreach (file2 ${CHR2})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe2 ${file2})
    message(exe: ${exe2})
    add_executable(${exe2} ${file2} ${utils} ${GLAD_SRC})

    if (APPLE)
        target_link_libraries(${exe2} glfw glm assimp::assimp ${IMGUI_LIB}
            "-framework Cocoa"
            "-framework CoreFoundation"
            "-framework IOKit"
            "-framework CoreVideo"
        )
    elseif(WIN32 OR UNIX)
        target_link_libraries(${exe2} glfw glm assimp::assimp ${IMGUI_LIB})
    endif()
endforeach ()


file(GLOB CHR3 ${PROJECT_SOURCE_DIR}/src/03_ModelLoading/*.cpp)
foreach (file2 ${CHR3})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe2 ${file2})
    message(exe: ${exe2})
    add_executable(${exe2} ${file2} ${utils} ${GLAD_SRC})

    if (APPLE)
        target_link_libraries(${exe2} glfw glm assimp::assimp ${IMGUI_LIB}
            "-framework Cocoa"
            "-framework CoreFoundation"
            "-framework IOKit"
            "-framework CoreVideo"
        )
    elseif(WIN32 OR UNIX)
        target_link_libraries(${exe2} glfw glm assimp::assimp ${IMGUI_LIB})
    endif()
endforeach ()


