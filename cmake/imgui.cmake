cmake_minimum_required(VERSION 3.10)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui)
message(Using ImGui from: ${IMGUI_DIR})

# ImGui 核心文件
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

set(IMGUI_HEADERS
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h
)

# 创建 ImGui 核心库
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# 设置包含目录
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
)

# 编译选项
target_compile_features(imgui PUBLIC cxx_std_11)

# 如果是 Windows，添加一些定义
if(WIN32)
    target_compile_definitions(imgui PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_DEPRECATE
    )
endif()

# 后端选项
option(IMGUI_BUILD_GLFW_BACKEND "Build GLFW backend" ON)
option(IMGUI_BUILD_OPENGL3_BACKEND "Build OpenGL3 backend" ON)
option(IMGUI_BUILD_WIN32_BACKEND "Build Win32 backend" OFF)
option(IMGUI_BUILD_DX11_BACKEND "Build DirectX11 backend" OFF)

# GLFW + OpenGL3 后端
if(IMGUI_BUILD_GLFW_BACKEND AND IMGUI_BUILD_OPENGL3_BACKEND)
    set(IMGUI_BACKEND_SOURCES
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
    
    set(IMGUI_BACKEND_HEADERS
        ${IMGUI_DIR}/backends/imgui_impl_glfw.h
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
    )
    
    add_library(imgui_glfw_opengl3 STATIC ${IMGUI_BACKEND_SOURCES} ${IMGUI_BACKEND_HEADERS})
    target_link_libraries(imgui_glfw_opengl3 PUBLIC imgui)
    target_include_directories(imgui_glfw_opengl3 PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
    )
endif()

# Win32 + OpenGL3 后端
if(IMGUI_BUILD_WIN32_BACKEND AND IMGUI_BUILD_OPENGL3_BACKEND)
    set(IMGUI_WIN32_BACKEND_SOURCES
        backends/imgui_impl_win32.cpp
        backends/imgui_impl_opengl3.cpp
    )
    
    set(IMGUI_WIN32_BACKEND_HEADERS
        backends/imgui_impl_win32.h
        backends/imgui_impl_opengl3.h
    )
    
    add_library(imgui_win32_opengl3 STATIC ${IMGUI_WIN32_BACKEND_SOURCES} ${IMGUI_WIN32_BACKEND_HEADERS})
    target_link_libraries(imgui_win32_opengl3 PUBLIC imgui)
    target_include_directories(imgui_win32_opengl3 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
    )
endif()

# DirectX11 后端
if(IMGUI_BUILD_DX11_BACKEND)
    set(IMGUI_DX11_BACKEND_SOURCES
        backends/imgui_impl_dx11.cpp
        backends/imgui_impl_win32.cpp
    )
    
    set(IMGUI_DX11_BACKEND_HEADERS
        backends/imgui_impl_dx11.h
        backends/imgui_impl_win32.h
    )
    
    add_library(imgui_dx11 STATIC ${IMGUI_DX11_BACKEND_SOURCES} ${IMGUI_DX11_BACKEND_HEADERS})
    target_link_libraries(imgui_dx11 PUBLIC imgui)
    target_include_directories(imgui_dx11 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
    )
endif()