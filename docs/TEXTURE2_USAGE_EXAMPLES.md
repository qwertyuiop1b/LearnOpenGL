# Texture2 类使用示例

## 目录

1. [基础使用](#基础使用)
2. [移动语义示例](#移动语义示例)
3. [容器中使用](#容器中使用)
4. [错误处理](#错误处理)
5. [多纹理](#多纹理)
6. [程序化纹理](#程序化纹理)
7. [与 Texture 类的对比](#与-texture-类的对比)

---

## 基础使用

### 示例 1：从文件加载纹理

```cpp
#include "utils/Texture2.h"
#include "utils/Shader.h"

void basicExample() {
    // 方式 1：构造时加载
    Texture2 tex("textures/wall.jpg");
    
    // 方式 2：先创建，后加载
    Texture2 tex2;
    tex2.loadFromFile("textures/floor.png");
    
    // 使用纹理
    Shader shader("shader.vert", "shader.frag");
    shader.use();
    
    tex.bind(0);  // 绑定到纹理单元 0
    shader.setInt("uTexture", 0);
    
    // 渲染...
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
```

### 示例 2：检查加载是否成功

```cpp
void checkLoadingExample() {
    Texture2 tex("textures/wall.jpg");
    
    if (!tex.isValid()) {
        std::cerr << "纹理加载失败！使用默认纹理..." << std::endl;
        
        // 创建默认的单色纹理
        unsigned char pink[4] = {255, 192, 203, 255};
        tex.createFromData(pink, 1, 1, 4);
    }
    
    // 现在 tex 一定是有效的
    tex.bind(0);
}
```

### 示例 3：设置纹理参数

```cpp
void textureParametersExample() {
    Texture2 tex("textures/sprite.png");
    
    // 像素艺术风格（最近邻过滤）
    tex.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    tex.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // 边缘拉伸（不重复）
    tex.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    tex.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // 设置类型标签
    tex.setType(TextureType2::DIFFUSE);
}
```

---

## 移动语义示例

### 示例 4：函数返回纹理

```cpp
// ✅ 正确：可以从函数返回 Texture2
Texture2 loadWallTexture() {
    Texture2 tex("textures/wall.jpg");
    tex.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    return tex;  // 自动移动，不是拷贝！
}

void useExample() {
    Texture2 wall = loadWallTexture();  // 移动构造
    wall.bind(0);
}
```

### 示例 5：转移所有权

```cpp
void transferOwnershipExample() {
    Texture2 tex1("textures/wood.jpg");
    
    // 显式转移所有权
    Texture2 tex2 = std::move(tex1);
    
    // 现在：
    // - tex1.isValid() == false  （失效）
    // - tex2.isValid() == true   （拥有纹理）
    
    tex2.bind(0);  // ✅ 正确
    // tex1.bind(0);  // ❌ 错误！tex1 已失效
}
```

### 示例 6：移动赋值

```cpp
void moveAssignmentExample() {
    Texture2 tex1("textures/wall.jpg");   // 纹理 A
    Texture2 tex2("textures/floor.png");  // 纹理 B
    
    // 移动赋值
    tex1 = std::move(tex2);
    
    // 结果：
    // - 纹理 A 被删除（不再需要）
    // - tex1 现在拥有纹理 B
    // - tex2 失效（可以安全析构）
    
    tex1.bind(0);  // 绑定的是原来的 floor.png
}
```

### 示例 7：不能拷贝（编译错误）

```cpp
void copyErrorExample() {
    Texture2 tex1("textures/wall.jpg");
    
    // ❌ 拷贝构造 - 编译错误！
    // Texture2 tex2 = tex1;  
    
    // ❌ 拷贝赋值 - 编译错误！
    // Texture2 tex3;
    // tex3 = tex1;
    
    // ✅ 正确：使用移动
    Texture2 tex4 = std::move(tex1);
    
    // ✅ 正确：传递引用
    auto useTexture = [](const Texture2& t) {
        t.bind(0);
    };
    useTexture(tex4);
}
```

---

## 容器中使用

### 示例 8：vector 中存储纹理

```cpp
#include <vector>

void vectorExample() {
    std::vector<Texture2> textures;
    
    // 方式 1：push_back + 移动
    Texture2 tex1("wall.jpg");
    textures.push_back(std::move(tex1));
    
    // 方式 2：emplace_back（原地构造，推荐）
    textures.emplace_back("floor.png");
    textures.emplace_back("ceiling.png");
    
    // 使用纹理
    for (size_t i = 0; i < textures.size(); ++i) {
        textures[i].bind(0);
        // 渲染...
    }
    
    // ❌ 错误：不能拷贝
    // std::vector<Texture2> copy = textures;
}
```

### 示例 9：map 中存储纹理

```cpp
#include <map>
#include <string>

class TextureManager {
private:
    std::map<std::string, Texture2> textures;
    
public:
    void loadTexture(const std::string& name, const std::string& filepath) {
        // emplace 原地构造
        textures.emplace(name, Texture2(filepath));
        
        // 或使用 insert + 移动
        // Texture2 tex(filepath);
        // textures.insert({name, std::move(tex)});
    }
    
    Texture2* getTexture(const std::string& name) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            return &it->second;
        }
        return nullptr;
    }
    
    void useTexture(const std::string& name, GLuint unit) {
        Texture2* tex = getTexture(name);
        if (tex && tex->isValid()) {
            tex->bind(unit);
        }
    }
};

// 使用示例
void textureManagerExample() {
    TextureManager manager;
    manager.loadTexture("wall", "textures/wall.jpg");
    manager.loadTexture("floor", "textures/floor.png");
    
    manager.useTexture("wall", 0);
    // 渲染...
}
```

---

## 错误处理

### 示例 10：健壮的加载

```cpp
Texture2 loadTextureWithFallback(const std::string& filepath) {
    Texture2 tex(filepath);
    
    if (!tex.isValid()) {
        std::cerr << "加载 " << filepath << " 失败，使用默认纹理" << std::endl;
        
        // 创建默认棋盘格纹理
        const int size = 64;
        unsigned char checkerboard[size * size * 3];
        
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                bool isWhite = ((x / 8) % 2) ^ ((y / 8) % 2);
                int index = (y * size + x) * 3;
                unsigned char color = isWhite ? 255 : 128;
                checkerboard[index + 0] = color;
                checkerboard[index + 1] = 0;
                checkerboard[index + 2] = color;
            }
        }
        
        tex.createFromData(checkerboard, size, size, 3);
    }
    
    return tex;
}
```

---

## 多纹理

### 示例 11：使用多个纹理

```cpp
void multiTextureExample() {
    // 加载多个纹理
    Texture2 diffuseMap("textures/wood_diffuse.jpg");
    Texture2 specularMap("textures/wood_specular.jpg");
    Texture2 normalMap("textures/wood_normal.jpg");
    
    // 设置类型标签
    diffuseMap.setType(TextureType2::DIFFUSE);
    specularMap.setType(TextureType2::SPECULAR);
    normalMap.setType(TextureType2::NORMAL);
    
    // 绑定到不同的纹理单元
    Shader shader("pbr.vert", "pbr.frag");
    shader.use();
    
    diffuseMap.bind(0);
    specularMap.bind(1);
    normalMap.bind(2);
    
    shader.setInt("uDiffuseMap", 0);
    shader.setInt("uSpecularMap", 1);
    shader.setInt("uNormalMap", 2);
    
    // 渲染...
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
```

### 示例 12：材质系统

```cpp
struct Material {
    Texture2 diffuse;
    Texture2 specular;
    float shininess;
    
    // 使用移动构造
    Material(Texture2&& diff, Texture2&& spec, float shin)
        : diffuse(std::move(diff))
        , specular(std::move(spec))
        , shininess(shin)
    {}
    
    void bind(Shader& shader) {
        diffuse.bind(0);
        specular.bind(1);
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", shininess);
    }
};

void materialExample() {
    // 创建材质（纹理移动到材质中）
    Material woodMaterial(
        Texture2("textures/wood_diffuse.jpg"),
        Texture2("textures/wood_specular.jpg"),
        32.0f
    );
    
    Shader shader("lighting.vert", "lighting.frag");
    shader.use();
    woodMaterial.bind(shader);
    
    // 渲染...
}
```

---

## 程序化纹理

### 示例 13：创建渐变纹理

```cpp
Texture2 createGradientTexture(int width, int height) {
    std::vector<unsigned char> pixels(width * height * 3);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            float t = static_cast<float>(x) / width;  // 0.0 到 1.0
            
            pixels[index + 0] = static_cast<unsigned char>(255 * (1 - t));  // R
            pixels[index + 1] = static_cast<unsigned char>(255 * t);        // G
            pixels[index + 2] = 0;                                           // B
        }
    }
    
    Texture2 tex;
    tex.createFromData(pixels.data(), width, height, 3);
    return tex;
}
```

### 示例 14：创建噪声纹理

```cpp
#include <random>

Texture2 createNoiseTexture(int size) {
    std::vector<unsigned char> pixels(size * size);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (int i = 0; i < size * size; ++i) {
        pixels[i] = static_cast<unsigned char>(dis(gen));
    }
    
    Texture2 tex;
    tex.createFromData(pixels.data(), size, size, 1);  // 单通道
    return tex;
}
```

### 示例 15：创建棋盘格纹理

```cpp
Texture2 createCheckerboardTexture(int size, int tileSize) {
    std::vector<unsigned char> pixels(size * size * 3);
    
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            bool isWhite = ((x / tileSize) % 2) ^ ((y / tileSize) % 2);
            int index = (y * size + x) * 3;
            unsigned char color = isWhite ? 255 : 0;
            
            pixels[index + 0] = color;
            pixels[index + 1] = color;
            pixels[index + 2] = color;
        }
    }
    
    Texture2 tex;
    tex.createFromData(pixels.data(), size, size, 3);
    
    // 像素风格
    tex.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    tex.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    return tex;
}
```

---

## 与 Texture 类的对比

### Texture（原版）

```cpp
// ⚠️ 问题：可能有双重释放风险
class Texture {
    // 拷贝构造和拷贝赋值被注释掉
    // 移动构造和移动赋值也被注释掉
    // 使用时可能有问题
};

// 使用（可能有问题）
Texture tex1("image.png");
// Texture tex2 = tex1;  // 如果启用，会双重释放
```

### Texture2（改进版）

```cpp
// ✅ 安全：明确禁用拷贝，支持移动
class Texture2 {
    Texture2(const Texture2&) = delete;              // 禁用拷贝
    Texture2& operator=(const Texture2&) = delete;   // 禁用拷贝赋值
    Texture2(Texture2&&) noexcept;                   // 支持移动
    Texture2& operator=(Texture2&&) noexcept;        // 支持移动赋值
};

// 使用（安全）
Texture2 tex1("image.png");
// Texture2 tex2 = tex1;             // ❌ 编译错误（好！）
Texture2 tex3 = std::move(tex1);     // ✅ 正确（移动）
```

### 对比总结

| 特性 | Texture | Texture2 |
|------|---------|----------|
| **拷贝安全** | ⚠️ 不确定 | ✅ 明确禁用 |
| **移动支持** | ⚠️ 被注释 | ✅ 完整实现 |
| **容器支持** | ❌ 困难 | ✅ 完全支持 |
| **函数返回** | ⚠️ 可能有问题 | ✅ 安全 |
| **所有权语义** | ⚠️ 模糊 | ✅ 清晰 |
| **注释文档** | ⚠️ 较少 | ✅ 详细 |

---

## 最佳实践

### ✅ 推荐做法

```cpp
// 1. 使用 emplace_back 原地构造
std::vector<Texture2> textures;
textures.emplace_back("wall.jpg");

// 2. 从函数返回（自动移动）
Texture2 loadTexture() {
    Texture2 tex("image.png");
    return tex;  // 不需要 std::move
}

// 3. 传递引用（避免移动）
void useTexture(const Texture2& tex) {
    tex.bind(0);
}

// 4. 检查有效性
Texture2 tex("image.png");
if (tex.isValid()) {
    tex.bind(0);
}
```

### ❌ 避免做法

```cpp
// 1. 移动后继续使用
Texture2 tex1("image.png");
Texture2 tex2 = std::move(tex1);
tex1.bind(0);  // ❌ tex1 已失效！

// 2. 不必要的 std::move
Texture2 loadTexture() {
    Texture2 tex("image.png");
    return std::move(tex);  // ❌ 多余，反而可能阻止优化
}

// 3. 尝试拷贝
// Texture2 tex2 = tex1;  // ❌ 编译错误

// 4. 不检查有效性
Texture2 tex("nonexistent.png");
tex.bind(0);  // ⚠️ 如果加载失败，绑定无效纹理
```

---

## 完整示例程序

```cpp
#include <iostream>
#include <vector>
#include "utils/Texture2.h"
#include "utils/Shader.h"
#include "utils/Window.h"

class TextureDemo {
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::vector<Texture2> textures;
    
public:
    TextureDemo() {
        window = std::make_unique<Window>(800, 600, "Texture2 Demo");
        shader = std::make_unique<Shader>("shader.vert", "shader.frag");
        
        // 加载纹理
        textures.emplace_back("textures/wall.jpg");
        textures.emplace_back("textures/floor.png");
        textures.emplace_back("textures/ceiling.png");
        
        // 检查加载
        for (size_t i = 0; i < textures.size(); ++i) {
            if (!textures[i].isValid()) {
                std::cerr << "纹理 " << i << " 加载失败！" << std::endl;
            }
        }
    }
    
    void run() {
        while (!window->shouldClose()) {
            window->pollEvents();
            
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            shader->use();
            textures[0].bind(0);
            shader->setInt("uTexture", 0);
            
            // 渲染...
            
            window->swapBuffer();
        }
    }
};

int main() {
    try {
        TextureDemo demo;
        demo.run();
    } catch (const std::exception& e) {
        std::cerr << "错误：" << e.what() << std::endl;
        return -1;
    }
    return 0;
}
```

---

## 总结

`Texture2` 类的关键特性：

1. **安全的资源管理**：RAII + 移动语义
2. **禁用拷贝**：防止双重释放
3. **支持移动**：可以放入容器、从函数返回
4. **清晰的所有权语义**：谁拥有纹理一目了然
5. **详细的文档**：每个函数都有注释说明

使用 `Texture2` 时，记住：
- ✅ 移动可以，拷贝不行
- ✅ 移动后源对象失效
- ✅ 可以从函数返回
- ✅ 可以放入容器

