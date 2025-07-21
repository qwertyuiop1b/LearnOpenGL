## LearnOpenGL
### OpenGL介绍
### 创建窗口
#### GLFW 
- A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input
```cmake
// 更新子模块
git submodule update --init --recursive
// sh 使用git submodule添加到项目
git submodule add git@github.com:glfw/glfw.git 3rdparty/glfw

// CMakeLists
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdparty/glfw)

```
#### GLAD
- 

### Hello Window

