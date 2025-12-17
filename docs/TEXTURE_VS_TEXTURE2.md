# Texture vs Texture2 对比

## 快速对比

| 方面 | Texture（原版） | Texture2（新版） |
|------|----------------|-----------------|
| **拷贝构造** | 注释掉（不确定） | `= delete` 明确禁用 |
| **拷贝赋值** | 注释掉（不确定） | `= delete` 明确禁用 |
| **移动构造** | 注释掉（不可用） | 完整实现 + `noexcept` |
| **移动赋值** | 注释掉（不可用） | 完整实现 + `noexcept` |
| **放入容器** | ❌ 困难 | ✅ 完全支持 |
| **函数返回** | ⚠️ 可能有问题 | ✅ 安全且高效 |
| **注释文档** | 基础注释 | 详细的中文注释 |
| **安全性** | ⚠️ 需要小心 | ✅ 编译时保证 |
| **学习价值** | 基础实现 | 展示完整 Rule of Five |

## 代码对比

### 1. 类声明对比

#### Texture（原版）

```cpp:src/utils/Texture.h
class Texture {
private:
    GLuint textureId;
    // ...

public:
    Texture();
    Texture(const std::string& filepath);
    ~Texture();

    // ⚠️ 被注释掉，不确定行为
    // Texture(const Texture&) = default;
    // Texture& operator=(const Texture&) = default;
    // Texture(Texture&& other) noexcept;
    // Texture& operator=(Texture&& other) noexcept;
    
    // ...
};
```

**问题：**
- 拷贝构造/赋值被注释，但编译器可能自动生成
- 移动构造/赋值被注释，无法使用
- 不能放入 `std::vector` 等容器

#### Texture2（新版）

```cpp:src/utils/Texture2.h
class Texture2 {
private:
    GLuint textureId;
    // ...

public:
    Texture2();
    explicit Texture2(const std::string& filepath);
    ~Texture2();

    // ✅ 明确禁用拷贝
    Texture2(const Texture2&) = delete;
    Texture2& operator=(const Texture2&) = delete;

    // ✅ 完整实现移动
    Texture2(Texture2&& other) noexcept;
    Texture2& operator=(Texture2&& other) noexcept;
    
    // ...
};
```

**优点：**
- 明确禁用拷贝，编译时保证安全
- 完整实现移动语义
- 可以放入容器，从函数返回
- 语义清晰，易于理解

### 2. 实现对比

#### Texture（原版）

```cpp:src/utils/Texture.cpp
// 移动语义被注释掉
// Texture::Texture(Texture&& other) noexcept 
//     : textureId(other.textureId)
//     , width(other.width)
//     , height(other.height)
//     , channels(other.channels)
//     , format(other.format)
// {
//     other.textureId = 0;
// }
```

**问题：**
- 代码被注释，无法使用
- 缺少详细说明

#### Texture2（新版）

```cpp:src/utils/Texture2.cpp
/**
 * 移动构造函数
 * 
 * 核心思想：窃取资源 + 让源对象失效
 * 
 * 步骤分解：
 * 1. 初始化列表拷贝所有成员
 * 2. 特殊处理 path（std::string）
 * 3. 让 other 失效（关键！）
 * 
 * 内存布局示例：[详细说明...]
 */
Texture2::Texture2(Texture2&& other) noexcept
    : textureId(other.textureId)
    , width(other.width)
    , height(other.height)
    , channels(other.channels)
    , format(other.format)
    , type(other.type)
    , path(std::move(other.path))
{
    other.textureId = 0;
    other.width = 0;
    other.height = 0;
    other.channels = 0;
}
```

**优点：**
- 完整实现，可以使用
- 详细的中文注释说明每个步骤
- 教学价值高，便于理解

## 使用场景对比

### 场景 1：容器中存储纹理

#### ❌ Texture（不推荐）

```cpp
std::vector<Texture> textures;

// ❌ 问题：没有移动语义，无法编译
// textures.emplace_back("wall.jpg");  // 编译错误！

// 只能使用指针（麻烦）
std::vector<std::unique_ptr<Texture>> textures;
textures.push_back(std::make_unique<Texture>("wall.jpg"));
```

#### ✅ Texture2（推荐）

```cpp
std::vector<Texture2> textures;

// ✅ 直接使用，简单清晰
textures.emplace_back("wall.jpg");
textures.emplace_back("floor.png");
```

### 场景 2：函数返回纹理

#### ⚠️ Texture（可能有问题）

```cpp
// 可能可以（取决于编译器优化）
Texture loadTexture() {
    Texture tex("image.png");
    return tex;  // 依赖 RVO/NRVO
}

// 不能显式移动（移动被注释了）
// return std::move(tex);  // 无法编译
```

#### ✅ Texture2（安全）

```cpp
// ✅ 安全且高效
Texture2 loadTexture() {
    Texture2 tex("image.png");
    return tex;  // 自动移动
}

// 也可以显式移动
// return std::move(tex);  // 可以，但不必要
```

### 场景 3：所有权转移

#### ❌ Texture（无法实现）

```cpp
Texture tex1("image.png");

// ❌ 无法转移所有权（移动被禁用）
// Texture tex2 = std::move(tex1);  // 编译错误

// 只能用指针
std::unique_ptr<Texture> ptr1 = std::make_unique<Texture>("image.png");
std::unique_ptr<Texture> ptr2 = std::move(ptr1);  // 转移指针，不是纹理本身
```

#### ✅ Texture2（简单直接）

```cpp
Texture2 tex1("image.png");

// ✅ 直接转移所有权
Texture2 tex2 = std::move(tex1);

// tex1 失效，tex2 接管纹理
```

## 安全性对比

### Texture 的潜在问题

```cpp
// 问题 1：编译器可能自动生成拷贝构造
Texture tex1("image.png");
Texture tex2 = tex1;  // 如果编译通过，会双重释放！

// 问题 2：无法放入容器
std::vector<Texture> textures;
// textures.emplace_back("wall.jpg");  // 编译错误

// 问题 3：函数返回不确定
Texture loadTexture() {
    Texture tex("image.png");
    return tex;  // 可能可以，取决于编译器
}
```

### Texture2 的安全保证

```cpp
// ✅ 拷贝被明确禁用（编译时错误）
Texture2 tex1("image.png");
// Texture2 tex2 = tex1;  // 编译错误！好！

// ✅ 可以放入容器
std::vector<Texture2> textures;
textures.emplace_back("wall.jpg");  // 完美工作

// ✅ 函数返回安全
Texture2 loadTexture() {
    Texture2 tex("image.png");
    return tex;  // 保证移动，不会拷贝
}

// ✅ 移动后源对象失效
Texture2 tex1("image.png");
Texture2 tex2 = std::move(tex1);
// tex1.isValid() == false  // 可以检查
```

## 教学价值对比

### Texture

- ✅ 展示基础 OpenGL 纹理操作
- ⚠️ 资源管理不完整
- ❌ 缺少移动语义示例
- ⚠️ 注释较少，学习难度高

### Texture2

- ✅ 展示完整的 Rule of Five
- ✅ 详细的中文注释
- ✅ 移动语义完整示例
- ✅ 包含使用示例文档
- ✅ 展示现代 C++ 最佳实践

## 性能对比

| 操作 | Texture | Texture2 |
|------|---------|----------|
| **构造** | 相同 | 相同 |
| **析构** | 相同 | 相同 |
| **拷贝** | ⚠️ 未定义 | ❌ 禁用（正确） |
| **移动** | ❌ 不可用 | ✅ O(1) 零成本 |
| **容器存储** | 需要指针（额外开销） | 直接存储（无开销） |

## 迁移指南

如果你已经在使用 `Texture`，想迁移到 `Texture2`：

### 步骤 1：简单替换

```cpp
// 之前
Texture tex("image.png");
tex.bind(0);

// 之后（完全兼容）
Texture2 tex("image.png");
tex.bind(0);
```

### 步骤 2：容器使用

```cpp
// 之前（使用指针）
std::vector<std::unique_ptr<Texture>> textures;
textures.push_back(std::make_unique<Texture>("wall.jpg"));
textures[0]->bind(0);  // 需要 ->

// 之后（直接使用）
std::vector<Texture2> textures;
textures.emplace_back("wall.jpg");
textures[0].bind(0);  // 使用 .
```

### 步骤 3：函数返回

```cpp
// 之前（返回指针或引用）
std::unique_ptr<Texture> loadTexture() {
    return std::make_unique<Texture>("image.png");
}
auto tex = loadTexture();
tex->bind(0);

// 之后（直接返回）
Texture2 loadTexture() {
    return Texture2("image.png");
}
Texture2 tex = loadTexture();
tex.bind(0);
```

## 何时使用哪个？

### 使用 Texture

- 你只需要基础纹理功能
- 不需要放入容器
- 不需要从函数返回
- 代码很简单，不涉及所有权转移

### 使用 Texture2（推荐）

- 需要放入 `std::vector` 等容器
- 需要从函数返回纹理对象
- 需要转移所有权
- 想要编译时安全保证
- 学习现代 C++ 资源管理
- 需要详细的文档和注释

## 总结

**Texture（原版）**
- ✅ 简单实现
- ✅ 满足基本需求
- ⚠️ 资源管理不完整
- ⚠️ 使用场景受限
- ⚠️ 教学价值有限

**Texture2（新版）**
- ✅ 完整的 Rule of Five
- ✅ 编译时安全保证
- ✅ 支持容器和函数返回
- ✅ 详细的中文注释
- ✅ 展示现代 C++ 最佳实践
- ✅ 高教学价值

**推荐：**
- 学习项目：使用 `Texture2`，理解完整的资源管理
- 生产项目：使用 `Texture2`，更安全、更灵活
- 简单演示：`Texture` 也可以，但 `Texture2` 更好

**关键差异：**
```cpp
// Texture：可能有问题
Texture tex1("img.png");
// Texture tex2 = tex1;  // 危险！

// Texture2：安全且清晰
Texture2 tex1("img.png");
// Texture2 tex2 = tex1;     // ❌ 编译错误（好！）
Texture2 tex2 = std::move(tex1);  // ✅ 明确的移动
```

