## LearnOpenGL
### OpenGL介绍
### Create Window
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


### Camera
#### OribitCamera
- 极坐标
> 就像(x,y)一样，可以表示整个平面的点
```
[radius, angle]
旋转angle，向前radius距离
```
- 球坐标
> 极坐标的三维表示
```
需要两个角度: azimuth(方位角) angle and polar angle
azimuth angle: rotation angle around Y axis; meaning left or right
polar angle: rotation angle around Z axis; meaning top or down
```
