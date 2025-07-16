cmake_minimum_required(VERSION 3.25)
project(OpenGL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include directories
include_directories(
    3rdparty/glfw/include
    3rdparty/glad/include
    3rdparty/glm
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
add_subdirectory(3rdparty/imgui)

#glad soruce code
set(GLAD_SRC 3rdparty/glad/src/glad.c)



if (WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/include)
    include_directories(${PROJECT_SOURCE_DIR}/src)
elseif(APPLE)
    include_directories(/opt/homebrew/include)
    include_directories(${PROJECT_SOURCE_DIR}/include)
    include_directories(${PROJECT_SOURCE_DIR}/src)
    link_directories(/opt/homebrew/lib)
endif()


# source code
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils utils)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/imgui imgui)


file(GLOB CHR1 ${PROJECT_SOURCE_DIR}/src/01_Start/*.cpp)
foreach (file ${CHR1})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe ${file})
    message(exe: ${exe})
    add_executable(${exe} ${file} ${utils} ${SRC} ${imgui} ${IMGUI_OPENGL} ${GLAD_SRC})

    if (WIN32)
        target_link_libraries(${exe} glfw glm)
    elseif(APPLE)
        target_link_libraries(${exe} glfw 
            "-framework Cocoa"
            "-framework CoreFoundation"
            "-framework IOKit"
            "-framework CoreVideo"
        )
    endif()
endforeach ()


file(GLOB CHR2 ${PROJECT_SOURCE_DIR}/src/02_Light/*.cpp)
foreach (file2 ${CHR2})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe2 ${file2})
    message(exe: ${exe2})
    add_executable(${exe2} ${file2} ${GLAD_SRC} ${utils} ${imgui})

    if (WIN32)
        target_link_libraries(${exe2} glfw glm)
    elseif(APPLE)
        target_link_libraries(${exe2} glfw 
            "-framework Cocoa"
            "-framework CoreFoundation"
            "-framework IOKit"
            "-framework CoreVideo"
        )
    endif()
endforeach ()


