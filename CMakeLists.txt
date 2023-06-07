cmake_minimum_required(VERSION 3.25)
project(OpenGL)
set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(GLAD_SRC ${PROJECT_SOURCE_DIR}/src/glad.c)


file(GLOB CHR1 ${PROJECT_SOURCE_DIR}/src/01_Start/*.cpp)
foreach (file ${CHR1})
    string(REGEX REPLACE ".*/(.+)\\.cpp" "\\1" exe ${file})
    message(exe: ${exe})
    add_executable(${exe} ${file} ${GLAD_SRC})
    target_link_libraries(${exe} glfw3)
endforeach ()


