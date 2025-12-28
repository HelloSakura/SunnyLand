# CMake 脚本说明

## cleanup.in / cleanup.bat.in

这些是清理脚本的模板文件，用于生成跨平台的清理脚本。

### 工作原理

1. **configure_file** 命令会读取模板文件（`.in` 后缀）
2. 将模板中的变量（如 `@CMAKE_BINARY_DIR@`）替换为实际值
3. 生成最终的脚本文件到构建目录

### 使用方法

#### 方法 1: 使用 CMake 目标
```bash
cmake --build build --target generate-clean-script
```

#### 方法 2: 直接运行生成的脚本
- **Windows**: `build\cleanup.bat`
- **Linux/Mac**: `./build/cleanup.sh`

### 变量说明

- `@CMAKE_BINARY_DIR@`: 构建目录的完整路径
- `@CMAKE_SOURCE_DIR@`: 源代码目录的完整路径
- `@CMAKE_CURRENT_SOURCE_DIR@`: 当前 CMakeLists.txt 所在的目录

### 注意事项

- 清理脚本会**删除整个构建目录**，请谨慎使用
- 建议在提交代码前先备份重要文件

