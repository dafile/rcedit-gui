# RceditGUI

[English](#english) | [中文](#中文)

---

## 中文

基于 [rcedit](https://github.com/electron/rcedit) 的图形化 Windows 资源编辑器，用于修改 EXE/DLL 文件的版本信息、图标、执行权限等。

### 功能
<img width="1264" height="961" alt="2026-06-14_19-46-15" src="https://github.com/user-attachments/assets/14330cbe-3eb9-429f-ac5a-f6a30b195231" />

- 编辑 12 个版本字符串字段（公司名称、文件描述、版权信息、原始文件名等）
- 修改文件版本号和产品版本号
- 替换程序图标（.ico）
- 设置执行权限（普通/最高可用/管理员）
- 加载自定义应用清单（manifest）
- 三种保存方式：覆盖原文件、备份+覆盖、另存为新文件
- 拖拽文件打开
- 简体中文 / English 双语界面

### 使用方法

1. 双击运行 `RceditGUI.exe`
2. 拖入 EXE/DLL 文件，或点击"点击打开文件"
3. 修改需要的字段
4. 选择保存方式，点击菜单 文件 > 保存

### 版本字符串字段说明

| 字段名 | 含义 |
|--------|------|
| CompanyName | 发布此文件的公司或组织名称 |
| FileDescription | 向用户显示的文件说明 |
| FileVersion (字符串) | 文件版本的文字描述 |
| InternalName | 文件的内部名称，不含扩展名 |
| LegalCopyright | 版权声明 |
| LegalTrademarks | 商标和注册商标信息 |
| OriginalFilename | 原始文件名（不含路径），用于判断文件是否被重命名 |
| PrivateBuild | 私有构建信息 |
| ProductName | 此文件所属产品的名称 |
| ProductVersion (字符串) | 产品版本的文字描述 |
| SpecialBuild | 此版本与标准版本的区别说明 |
| 备注 | 附加的注释或说明信息 |

### 快捷键

| 快捷键 | 功能 |
|--------|------|
| Ctrl+O | 打开文件 |
| Ctrl+S | 保存 |
| Ctrl+Shift+S | 另存为 |

### 构建环境

- Clang 20 (MSVC target)
- Windows SDK 10.0.26100
- Dear ImGui 1.91.8
- DirectX 11

### 编译

```bat
cd rcedit-gui
build.bat
```

---

## English

A graphical Windows resource editor based on [rcedit](https://github.com/electron/rcedit), used to modify version information, icons, execution level and more in EXE/DLL files.
<img width="1264" height="961" alt="2026-06-14_19-46-34" src="https://github.com/user-attachments/assets/e15214d9-1145-4179-af84-f6cdc2b96746" />

### Features

- Edit 12 version string fields (CompanyName, FileDescription, LegalCopyright, OriginalFilename, etc.)
- Modify file version and product version numbers
- Replace application icon (.ico)
- Set execution level (normal / highest available / administrator)
- Load custom application manifest
- Three save modes: overwrite original, backup + overwrite, save as new file
- Drag & drop file opening
- Simplified Chinese / English bilingual interface

### Usage

1. Double-click `RceditGUI.exe`
2. Drag & drop an EXE/DLL file, or click "Click to open file"
3. Edit the desired fields
4. Choose save mode, then File > Save

### Version String Fields

| Field | Description |
|-------|-------------|
| CompanyName | Company or organization that produced this file |
| FileDescription | Description shown to users |
| FileVersion (string) | Textual file version |
| InternalName | Internal name of the file, without extension |
| LegalCopyright | Copyright notice |
| LegalTrademarks | Trademark and registered trademark info |
| OriginalFilename | Original filename (no path), used to detect if file was renamed |
| PrivateBuild | Info about private build |
| ProductName | Name of the product this file belongs to |
| ProductVersion (string) | Textual product version |
| SpecialBuild | How this version differs from the standard release |
| Comments | Additional comments or notes |

### Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| Ctrl+O | Open file |
| Ctrl+S | Save |
| Ctrl+Shift+S | Save As |

### Build Environment

- Clang 20 (MSVC target)
- Windows SDK 10.0.26100
- Dear ImGui 1.91.8
- DirectX 11

### Build

```bat
cd rcedit-gui
build.bat
```

---

## License

rcedit core code: MIT License (Electron team)
Dear ImGui: MIT License (Omar Cornut)
