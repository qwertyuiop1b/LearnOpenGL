cmake_minimum_required(VERSION 3.25)
project(OpenGL)
set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
link_directories(${PROJECT_SOURCE_DIR}/lib)


set(GLAD_SRC ${PROJECT_SOURCE_DIR}/src/glad.c)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils utils)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/imgui imgui)




file(GLOB CHR1 ${PROJECT_SOURCE_DIR}/src/01_Start/*.cpp)
foreach (file ${CHR1})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe ${file})
    message(exe: ${exe})
    add_executable(${exe} ${file} ${GLAD_SRC} ${utils} ${SRC} ${imgui})
    target_link_libraries(${exe} glfw3)
endforeach ()



file(GLOB CHR2 ${PROJECT_SOURCE_DIR}/src/02_Light/*.cpp)
foreach (file2 ${CHR2})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe2 ${file2})
    message(exe: ${exe2})
    add_executable(${exe2} ${file2} ${GLAD_SRC} ${utils} ${imgui})
    target_link_libraries(${exe2} glfw3)
endforeach ()


