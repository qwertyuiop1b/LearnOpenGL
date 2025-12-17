# OpenGL Texture 类设计指南

## C++ 资源管理：Rule of Five

当类管理资源（内存、文件句柄、OpenGL 对象等）时，需要考虑 **Rule of Five**：

### Rule of Five 是什么？

如果你的类需要自定义以下 5 个函数中的任何一个，通常需要自定义所有 5 个：

1. **析构函数** `~T()`
2. **拷贝构造函数** `T(const T&)`
3. **拷贝赋值运算符** `T& operator=(const T&)`
4. **移动构造函数** `T(T&&) noexcept`
5. **移动赋值运算符** `T& operator=(T&&) noexcept`

### 为什么 Texture 需要考虑？

```cpp
class Texture {
    GLuint textureId;  // ← 这是资源句柄！
    
    ~Texture() {
        glDeleteTextures(1, &textureId);  // ← 需要释放资源
    }
};
```

一旦定义了析构函数，就必须考虑拷贝和移动！

## 三种设计方案

### 方案 1：禁用拷贝，允许移动（推荐）

**适用场景：**
- 纹理是独占资源
- 不希望意外拷贝
- 允许转移所有权

**实现：**
```cpp
class Texture {
public:
    // 禁用拷贝
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    
    // 允许移动
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
};
```

**优点：**
- ✅ 防止意外拷贝
- ✅ 清晰的所有权语义
- ✅ 零成本转移所有权
- ✅ 可以放入 `std::vector`（需要移动）

**缺点：**
- ❌ 不能直接拷贝（但这通常是好事）

### 方案 2：深拷贝（不推荐）

**实现：**
```cpp
Texture::Texture(const Texture& other) {
    // 重新加载纹理数据
    if (other.path.empty()) {
        // 无法拷贝
    } else {
        loadFromFile(other.path);
    }
}
```

**问题：**
- ❌ 拷贝代价高（重新加载文件）
- ❌ 浪费 GPU 显存（两份相同数据）
- ❌ 如果纹理是动态生成的，无法拷贝

### 方案 3：共享所有权（复杂）

**实现：**
```cpp
class Texture {
    std::shared_ptr<TextureData> data;  // 引用计数
};
```

**问题：**
- ❌ 增加复杂度
- ❌ 运行时开销（引用计数）
- ❌ OpenGL 纹理通常不需要共享

## 推荐实现：禁用拷贝 + 移动语义

### 核心概念

**拷贝 vs 移动：**

```cpp
// 拷贝：复制数据（代价高）
Texture tex1("image.png");
Texture tex2 = tex1;  // ❌ 禁用

// 移动：转移所有权（零成本）
Texture tex1("image.png");
Texture tex2 = std::move(tex1);  // ✅ tex1 失效，tex2 接管
```

### 移动的作用

1. **函数返回时避免拷贝**
```cpp
Texture loadTexture() {
    Texture tex("image.png");
    return tex;  // 自动移动，不是拷贝
}
```

2. **容器中存储**
```cpp
std::vector<Texture> textures;
textures.push_back(Texture("tex1.png"));  // 移动
textures.emplace_back("tex2.png");        // 原地构造
```

3. **转移所有权**
```cpp
Texture tex1("image.png");
Texture tex2 = std::move(tex1);  // tex1 不再拥有纹理
```

## 移动语义实现详解

### 移动构造函数

```cpp
Texture::Texture(Texture&& other) noexcept 
    : textureId(other.textureId)
    , width(other.width)
    , height(other.height)
    , channels(other.channels)
    , format(other.format)
    , type(other.type)
    , path(std::move(other.path))  // 移动字符串
{
    // 关键：让 other 失效，防止双重删除
    other.textureId = 0;
}
```

**步骤：**
1. 窃取 `other` 的资源（textureId）
2. 将 `other.textureId` 设为 0（无效状态）
3. 当 `other` 析构时，不会删除纹理（因为 ID 是 0）

### 移动赋值运算符

```cpp
Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {  // 防止自赋值
        // 1. 释放当前持有的资源
        if (textureId != 0) {
            glDeleteTextures(1, &textureId);
        }
        
        // 2. 窃取 other 的资源
        textureId = other.textureId;
        width = other.width;
        height = other.height;
        channels = other.channels;
        format = other.format;
        type = other.type;
        path = std::move(other.path);
        
        // 3. 让 other 失效
        other.textureId = 0;
    }
    return *this;
}
```

## 常见陷阱

### 陷阱 1：忘记让源对象失效

```cpp
// ❌ 错误
Texture::Texture(Texture&& other) noexcept 
    : textureId(other.textureId) 
{
    // 忘记设置 other.textureId = 0
}

// 结果：两个对象都有相同的 ID，双重删除！
```

### 陷阱 2：移动后继续使用

```cpp
Texture tex1("image.png");
Texture tex2 = std::move(tex1);
tex1.bind();  // ❌ tex1 已失效！
```

### 陷阱 3：忘记 noexcept

```cpp
// ❌ 缺少 noexcept
Texture(Texture&& other) { ... }

// 问题：std::vector 在重新分配时不会使用移动，而是拷贝
// 解决：添加 noexcept
Texture(Texture&& other) noexcept { ... }
```

## 使用示例

### 创建和移动

```cpp
// 创建纹理
Texture tex1("container.jpg");

// 移动到另一个对象
Texture tex2 = std::move(tex1);  // tex1 失效

// 移动到容器
std::vector<Texture> textures;
textures.push_back(std::move(tex2));

// 从函数返回（自动移动）
Texture loadMyTexture() {
    Texture temp("wall.jpg");
    return temp;  // 自动移动，不需要 std::move
}
```

### 错误示例

```cpp
// ❌ 拷贝被禁用
Texture tex1("image.png");
Texture tex2 = tex1;  // 编译错误！

// ✅ 正确：使用移动
Texture tex2 = std::move(tex1);
```

## 性能对比

| 操作 | 拷贝 | 移动 |
|------|------|------|
| **时间** | O(n) - 重新加载文件 | O(1) - 只是指针赋值 |
| **内存** | 2倍 | 1倍 |
| **GPU 显存** | 2倍 | 1倍 |

## 总结

### 对于 Texture 类

✅ **应该做：**
- 禁用拷贝构造和拷贝赋值
- 实现移动构造和移动赋值
- 添加 `noexcept` 到移动函数
- 在移动后让源对象失效

❌ **不应该做：**
- 实现深拷贝（代价太高）
- 默认拷贝（导致双重删除）
- 忘记 `noexcept`（影响容器性能）

### 记忆口诀

```
资源类要慎重，
析构释放记心中。
拷贝移动要分清，
Rule of Five 不能松。

OpenGL 是句柄，
拷贝容易出大问题。
禁用拷贝用移动，
所有权转移最轻松。
```

