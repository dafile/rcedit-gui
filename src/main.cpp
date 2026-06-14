// RceditGUI - GUI for rcedit
// Built with Dear ImGui + Win32 + DirectX 11

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "rescle.h"

// ============================================================
// Language System
// ============================================================

enum class Lang { ZH_CN, EN };

static Lang g_lang = Lang::ZH_CN;

struct I18n {
    // Menu
    const char* menuFile;
    const char* menuOpen;
    const char* menuSave;
    const char* menuSaveAs;
    const char* menuExit;
    const char* menuHelp;
    const char* menuAbout;
    const char* menuLanguage;

    // File info
    const char* fileInfo;
    const char* fileName;
    const char* fileSize;
    const char* dropHint;
    const char* clickToOpen;
    const char* openFilter;

    // Version numbers
    const char* versionNumbers;
    const char* fileVersion;
    const char* productVersion;
    const char* fileVersionTip;
    const char* productVersionTip;

    // Save options
    const char* saveOptions;
    const char* saveOverwrite;
    const char* saveBackup;
    const char* saveAsNew;

    // Icon / Execution / Manifest
    const char* icon;
    const char* iconTip;
    const char* execLevel;
    const char* execDefault;
    const char* execInvoker;
    const char* execHighest;
    const char* execAdmin;
    const char* manifest;
    const char* manifestTip;

    // Version strings
    const char* versionStrings;
    const char* btnClearAll;
    const char* btnResetAll;
    const char* btnReset;
    const char* btnBrowse;

    // Status
    const char* statusReady;
    const char* statusNoFile;
    const char* statusLoaded;
    const char* statusSaved;
    const char* statusSavedBackup;
    const char* statusSavedAs;
    const char* statusErrLoad;
    const char* statusErrBackup;
    const char* statusErrSaveAs;
    const char* statusErrSave;
    const char* statusErrCommit;
    const char* statusErrNoPath;

    // About
    const char* aboutTitle;

    // Field labels & tooltips
    struct FieldInfo { const char* label; const char* tip; };
    FieldInfo companyName;
    FieldInfo fileDescription;
    FieldInfo fvString;
    FieldInfo internalName;
    FieldInfo legalCopyright;
    FieldInfo legalTrademarks;
    FieldInfo originalFilename;
    FieldInfo privateBuild;
    FieldInfo productName;
    FieldInfo pvString;
    FieldInfo specialBuild;
    FieldInfo comments;
};

static const I18n g_zh = {
    /* menuFile */    "文件",
    /* menuOpen */    "打开...",
    /* menuSave */    "保存",
    /* menuSaveAs */  "另存为...",
    /* menuExit */    "退出",
    /* menuHelp */    "帮助",
    /* menuAbout */   "关于",
    /* menuLanguage */"语言",

    /* fileInfo */     "文件信息",
    /* fileName */     "文件",
    /* fileSize */     "大小",
    /* dropHint */     "拖拽 EXE/DLL 文件到此处",
    /* clickToOpen */  "点击打开文件",
    /* openFilter */   nullptr,

    /* versionNumbers */  "版本号",
    /* fileVersion */     "文件版本",
    /* productVersion */  "产品版本",
    /* fileVersionTip */    "文件版本号，如 1.0.0.0\n修改文件的二进制版本信息",
    /* productVersionTip */ "产品版本号，如 2.0.0\n修改产品的二进制版本信息",

    /* saveOptions */  "保存方式",
    /* saveOverwrite */"直接覆盖原文件",
    /* saveBackup */   "备份原文件(.bak)后覆盖",
    /* saveAsNew */    "另存为新文件",

    /* icon */         "图标",
    /* iconTip */      "替换程序图标（.ico 格式）",
    /* execLevel */    "执行权限",
    /* execDefault */  "不修改",
    /* execInvoker */  "普通权限（asInvoker）",
    /* execHighest */  "最高可用权限（highestAvailable）",
    /* execAdmin */    "管理员权限（requireAdministrator）",
    /* manifest */     "应用清单",
    /* manifestTip */  "自定义应用程序清单文件（.xml/.manifest）",

    /* versionStrings */ "版本字符串",
    /* btnClearAll */    "清空全部",
    /* btnResetAll */    "重置全部",
    /* btnReset */       "重置",
    /* btnBrowse */      "浏览...",

    /* statusReady */      "就绪",
    /* statusNoFile */     "未加载文件",
    /* statusLoaded */     "文件加载成功",
    /* statusSaved */      "保存成功",
    /* statusSavedBackup */"保存成功（已创建备份）",
    /* statusSavedAs */    "另存成功",
    /* statusErrLoad */    "加载文件失败",
    /* statusErrBackup */  "创建备份失败",
    /* statusErrSaveAs */  "另存失败",
    /* statusErrSave */    "保存失败",
    /* statusErrCommit */  "提交更改失败",
    /* statusErrNoPath */  "请指定保存路径",

    /* aboutTitle */   "关于 RceditGUI",

    /* companyName */       {"公司名称",        "发布此文件的公司或组织名称"},
    /* fileDescription */   {"文件描述",        "向用户显示的文件说明，安装时会显示在列表中"},
    /* fvString */          {"文件版本(字符串)", "文件版本的文字描述，如 '1.0.0 Release'"},
    /* internalName */      {"内部名称",        "文件的内部名称，不含扩展名"},
    /* legalCopyright */    {"版权声明",        "法律版权信息，如 'Copyright (c) 2026 MyCompany'"},
    /* legalTrademarks */   {"商标声明",        "商标和注册商标信息"},
    /* originalFilename */  {"原始文件名",      "文件的原始名称（不含路径），用于判断文件是否被重命名"},
    /* privateBuild */      {"私有构建信息",    "私有版本的构建信息，如 'Built by TESTER1'"},
    /* productName */       {"产品名称",        "此文件所属产品的名称"},
    /* pvString */          {"产品版本(字符串)", "产品版本的文字描述"},
    /* specialBuild */      {"特殊构建信息",    "此版本与标准版本的区别说明"},
    /* comments */          {"备注",            "附加的注释或说明信息"},
};

static const I18n g_en = {
    /* menuFile */    "File",
    /* menuOpen */    "Open...",
    /* menuSave */    "Save",
    /* menuSaveAs */  "Save As...",
    /* menuExit */    "Exit",
    /* menuHelp */    "Help",
    /* menuAbout */   "About",
    /* menuLanguage */"Language",

    /* fileInfo */     "File Information",
    /* fileName */     "File",
    /* fileSize */     "Size",
    /* dropHint */     "Drop an EXE/DLL file here",
    /* clickToOpen */  "Click to open file",
    /* openFilter */   nullptr,

    /* versionNumbers */  "Version Numbers",
    /* fileVersion */     "File Version",
    /* productVersion */  "Product Version",
    /* fileVersionTip */    "Binary file version, e.g. 1.0.0.0\nModifies the PE version resource",
    /* productVersionTip */ "Binary product version, e.g. 2.0.0\nModifies the PE version resource",

    /* saveOptions */  "Save Options",
    /* saveOverwrite */"Overwrite original file",
    /* saveBackup */   "Backup (.bak) then overwrite",
    /* saveAsNew */    "Save as new file",

    /* icon */         "Icon",
    /* iconTip */      "Replace application icon (.ico format)",
    /* execLevel */    "Execution Level",
    /* execDefault */  "No change",
    /* execInvoker */  "Normal (asInvoker)",
    /* execHighest */  "Highest available",
    /* execAdmin */    "Administrator (requireAdministrator)",
    /* manifest */     "Application Manifest",
    /* manifestTip */  "Custom application manifest file (.xml/.manifest)",

    /* versionStrings */ "Version Strings",
    /* btnClearAll */    "Clear All",
    /* btnResetAll */    "Reset All",
    /* btnReset */       "Reset",
    /* btnBrowse */      "Browse...",

    /* statusReady */      "Ready",
    /* statusNoFile */     "No file loaded",
    /* statusLoaded */     "File loaded successfully",
    /* statusSaved */      "Saved successfully",
    /* statusSavedBackup */"Saved (backup created)",
    /* statusSavedAs */    "Saved as new file",
    /* statusErrLoad */    "Failed to load file",
    /* statusErrBackup */  "Failed to create backup",
    /* statusErrSaveAs */  "Failed to save as new file",
    /* statusErrSave */    "Failed to save changes",
    /* statusErrCommit */  "Failed to commit changes",
    /* statusErrNoPath */  "Please specify save path",

    /* aboutTitle */   "About RceditGUI",

    /* companyName */       {"CompanyName",        "Company or organization that produced this file"},
    /* fileDescription */   {"FileDescription",    "Description shown to users, e.g. 'Keyboard Driver for AT-Style Keyboards'"},
    /* fvString */          {"FileVersion (str)",  "Textual file version, e.g. '1.0.0 Release'"},
    /* internalName */      {"InternalName",       "Internal name of the file, without extension"},
    /* legalCopyright */    {"LegalCopyright",     "Copyright notice, e.g. 'Copyright (c) 2026 MyCompany'"},
    /* legalTrademarks */   {"LegalTrademarks",    "Trademark and registered trademark info"},
    /* originalFilename */  {"OriginalFilename",   "Original filename (no path), used to detect if file was renamed"},
    /* privateBuild */      {"PrivateBuild",       "Info about private build, e.g. 'Built by TESTER1'"},
    /* productName */       {"ProductName",        "Name of the product this file belongs to"},
    /* pvString */          {"ProductVersion (str)","Textual product version"},
    /* specialBuild */      {"SpecialBuild",       "How this version differs from the standard release"},
    /* comments */          {"Comments",           "Additional comments or notes"},
};

static const I18n* T() { return g_lang == Lang::ZH_CN ? &g_zh : &g_en; }

// ============================================================
// D3D11 Globals
// ============================================================

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ============================================================
// Version Fields
// ============================================================

struct VersionField {
    const I18n::FieldInfo* info;
    const wchar_t* key;
    char value[512];
    char original[512];
};

static std::vector<VersionField> g_versionFields;

static void InitFields() {
    g_versionFields = {
        { &T()->companyName,       L"CompanyName",        "", "" },
        { &T()->fileDescription,   L"FileDescription",    "", "" },
        { &T()->fvString,          L"FileVersion",        "", "" },
        { &T()->internalName,      L"InternalName",       "", "" },
        { &T()->legalCopyright,    L"LegalCopyright",     "", "" },
        { &T()->legalTrademarks,   L"LegalTrademarks",    "", "" },
        { &T()->originalFilename,  L"OriginalFilename",   "", "" },
        { &T()->privateBuild,      L"PrivateBuild",       "", "" },
        { &T()->productName,       L"ProductName",        "", "" },
        { &T()->pvString,          L"ProductVersion",     "", "" },
        { &T()->specialBuild,      L"SpecialBuild",       "", "" },
        { &T()->comments,          L"Comments",           "", "" },
    };
}

// ============================================================
// Application State
// ============================================================

struct AppState {
    std::wstring filePath;
    std::string filePathUtf8;
    bool fileLoaded = false;

    char fileVersion[64] = "";
    char productVersion[64] = "";
    char originalFileVersion[64] = "";
    char originalProductVersion[64] = "";

    int executionLevel = 0;
    char manifestPath[1024] = "";
    char iconPath[1024] = "";

    int saveMode = 1;
    char saveAsPath[1024] = "";

    char statusMsg[512] = "";
    bool statusIsError = false;
    float statusTime = 0.0f;

    bool showAbout = false;
};

static AppState g_app;

// ============================================================
// Helpers
// ============================================================

static std::wstring Utf8ToWide(const char* str) {
    if (!str || !*str) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    std::wstring result(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, result.data(), len);
    return result;
}

static std::string WideToUtf8(const wchar_t* str) {
    if (!str || !*str) return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
    std::string result(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, result.data(), len, nullptr, nullptr);
    return result;
}

static std::string GetFileName(const std::string& path) {
    auto pos = path.find_last_of("\\/");
    return pos != std::string::npos ? path.substr(pos + 1) : path;
}

static void SetStatus(const char* msg, bool isError = false) {
    snprintf(g_app.statusMsg, sizeof(g_app.statusMsg), "%s", msg);
    g_app.statusIsError = isError;
    g_app.statusTime = 4.0f;
}

static bool ParseVersion(const char* str, unsigned short& v1, unsigned short& v2, unsigned short& v3, unsigned short& v4) {
    v1 = v2 = v3 = v4 = 0;
    return sscanf_s(str, "%hu.%hu.%hu.%hu", &v1, &v2, &v3, &v4) >= 1;
}

// ============================================================
// File Operations
// ============================================================

static bool LoadExeInfo(const std::wstring& path) {
    g_app.fileLoaded = false;
    g_app.executionLevel = 0;
    g_app.manifestPath[0] = 0;
    g_app.iconPath[0] = 0;
    for (auto& f : g_versionFields) { f.value[0] = 0; f.original[0] = 0; }
    g_app.fileVersion[0] = 0;
    g_app.productVersion[0] = 0;
    g_app.originalFileVersion[0] = 0;
    g_app.originalProductVersion[0] = 0;

    HMODULE hModule = LoadLibraryExW(path.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (!hModule) {
        SetStatus(T()->statusErrLoad, true);
        return false;
    }

    DWORD verHandle = 0;
    DWORD verSize = GetFileVersionInfoSizeW(path.c_str(), &verHandle);
    if (verSize > 0) {
        std::vector<BYTE> verData(verSize);
        if (GetFileVersionInfoW(path.c_str(), verHandle, verSize, verData.data())) {
            struct LangCodePage { WORD language; WORD codePage; };
            LangCodePage* translations = nullptr;
            UINT transLen = 0;
            if (VerQueryValueW(verData.data(), L"\\VarFileInfo\\Translation", (LPVOID*)&translations, &transLen)) {
                if (transLen >= sizeof(LangCodePage)) {
                    WORD lang = translations[0].language;
                    WORD codePage = translations[0].codePage;
                    for (auto& field : g_versionFields) {
                        wchar_t subBlock[256];
                        swprintf_s(subBlock, L"\\StringFileInfo\\%04x%04x\\%s", lang, codePage, field.key);
                        wchar_t* value = nullptr;
                        UINT valueLen = 0;
                        if (VerQueryValueW(verData.data(), subBlock, (LPVOID*)&value, &valueLen) && value) {
                            std::string utf8 = WideToUtf8(value);
                            snprintf(field.value, sizeof(field.value), "%s", utf8.c_str());
                            snprintf(field.original, sizeof(field.original), "%s", utf8.c_str());
                        }
                    }
                    VS_FIXEDFILEINFO* ffi = nullptr;
                    UINT ffiLen = 0;
                    if (VerQueryValueW(verData.data(), L"\\", (LPVOID*)&ffi, &ffiLen) && ffi) {
                        snprintf(g_app.fileVersion, sizeof(g_app.fileVersion), "%u.%u.%u.%u",
                            (unsigned)((ffi->dwFileVersionMS >> 16) & 0xffff),
                            (unsigned)((ffi->dwFileVersionMS >> 0) & 0xffff),
                            (unsigned)((ffi->dwFileVersionLS >> 16) & 0xffff),
                            (unsigned)((ffi->dwFileVersionLS >> 0) & 0xffff));
                        snprintf(g_app.originalFileVersion, sizeof(g_app.originalFileVersion), "%s", g_app.fileVersion);
                        snprintf(g_app.productVersion, sizeof(g_app.productVersion), "%u.%u.%u.%u",
                            (unsigned)((ffi->dwProductVersionMS >> 16) & 0xffff),
                            (unsigned)((ffi->dwProductVersionMS >> 0) & 0xffff),
                            (unsigned)((ffi->dwProductVersionLS >> 16) & 0xffff),
                            (unsigned)((ffi->dwProductVersionLS >> 0) & 0xffff));
                        snprintf(g_app.originalProductVersion, sizeof(g_app.originalProductVersion), "%s", g_app.productVersion);
                    }
                }
            }
        }
    }

    FreeLibrary(hModule);
    g_app.filePath = path;
    g_app.filePathUtf8 = WideToUtf8(path.c_str());
    g_app.fileLoaded = true;
    g_app.saveMode = 1;
    g_app.saveAsPath[0] = 0;
    SetStatus(T()->statusLoaded);
    return true;
}

static bool BackupFile(const std::wstring& path) {
    try {
        std::filesystem::copy_file(path, path + L".bak", std::filesystem::copy_options::overwrite_existing);
        return true;
    } catch (...) { return false; }
}

static bool SaveChanges() {
    if (!g_app.fileLoaded) return false;
    std::wstring targetPath;

    if (g_app.saveMode == 2) {
        if (g_app.saveAsPath[0] == 0) { SetStatus(T()->statusErrNoPath, true); return false; }
        targetPath = Utf8ToWide(g_app.saveAsPath);
        try { std::filesystem::copy_file(g_app.filePath, targetPath, std::filesystem::copy_options::overwrite_existing); }
        catch (...) { SetStatus(T()->statusErrSaveAs, true); return false; }
    } else {
        targetPath = g_app.filePath;
        if (g_app.saveMode == 1) {
            if (!BackupFile(g_app.filePath)) { SetStatus(T()->statusErrBackup, true); return false; }
        }
    }

    rescle::ResourceUpdater updater;
    if (!updater.Load(targetPath.c_str())) { SetStatus(T()->statusErrSave, true); return false; }

    for (const auto& field : g_versionFields) {
        updater.SetVersionString(field.key, Utf8ToWide(field.value).c_str());
    }

    unsigned short fv1, fv2, fv3, fv4;
    if (ParseVersion(g_app.fileVersion, fv1, fv2, fv3, fv4)) {
        updater.SetFileVersion(fv1, fv2, fv3, fv4);
        updater.SetVersionString(L"FileVersion", Utf8ToWide(g_app.fileVersion).c_str());
    }
    unsigned short pv1, pv2, pv3, pv4;
    if (ParseVersion(g_app.productVersion, pv1, pv2, pv3, pv4)) {
        updater.SetProductVersion(pv1, pv2, pv3, pv4);
        updater.SetVersionString(L"ProductVersion", Utf8ToWide(g_app.productVersion).c_str());
    }

    if (g_app.iconPath[0] != 0) {
        auto p = Utf8ToWide(g_app.iconPath);
        if (std::filesystem::exists(p)) updater.SetIcon(p.c_str());
    }
    if (g_app.executionLevel > 0) {
        const wchar_t* levels[] = { L"", L"asInvoker", L"highestAvailable", L"requireAdministrator" };
        updater.SetExecutionLevel(levels[g_app.executionLevel]);
    }
    if (g_app.manifestPath[0] != 0) {
        auto p = Utf8ToWide(g_app.manifestPath);
        if (std::filesystem::exists(p)) updater.SetApplicationManifest(p.c_str());
    }

    if (!updater.Commit()) { SetStatus(T()->statusErrCommit, true); return false; }

    for (auto& f : g_versionFields) snprintf(f.original, sizeof(f.original), "%s", f.value);
    snprintf(g_app.originalFileVersion, sizeof(g_app.originalFileVersion), "%s", g_app.fileVersion);
    snprintf(g_app.originalProductVersion, sizeof(g_app.originalProductVersion), "%s", g_app.productVersion);

    SetStatus(g_app.saveMode == 2 ? T()->statusSavedAs : g_app.saveMode == 1 ? T()->statusSavedBackup : T()->statusSaved);
    return true;
}

static std::wstring OpenFileDialog(const wchar_t* filter) {
    OPENFILENAMEW ofn = {};
    wchar_t szFile[MAX_PATH] = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameW(&ofn)) return szFile;
    return L"";
}

static std::wstring SaveFileDialog() {
    OPENFILENAMEW ofn = {};
    wchar_t szFile[MAX_PATH] = {};
    if (!g_app.filePathUtf8.empty()) {
        std::string name = GetFileName(g_app.filePathUtf8);
        MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, szFile, MAX_PATH);
    }
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Executable (*.exe)\0*.exe\0DLL (*.dll)\0*.dll\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    if (GetSaveFileNameW(&ofn)) return szFile;
    return L"";
}

static void HandleDrop(HDROP hDrop) {
    UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);
    if (fileCount > 0) {
        wchar_t filePath[MAX_PATH];
        DragQueryFileW(hDrop, 0, filePath, MAX_PATH);
        LoadExeInfo(filePath);
    }
    DragFinish(hDrop);
}

// ============================================================
// Tooltip Helper
// ============================================================

static void HelpMarker(const char* desc) {
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

// ============================================================
// GUI Rendering
// ============================================================

static void RenderGUI() {
    ImGuiIO& io = ImGui::GetIO();
    const I18n* t = T();

    if (g_app.statusTime > 0.0f)
        g_app.statusTime -= io.DeltaTime;

    // ---- Menu Bar ----
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(t->menuFile)) {
            if (ImGui::MenuItem(t->menuOpen, "Ctrl+O")) {
                auto path = OpenFileDialog(L"Executable (*.exe)\0*.exe\0DLL (*.dll)\0*.dll\0All Files (*.*)\0*.*\0");
                if (!path.empty()) LoadExeInfo(path);
            }
            ImGui::Separator();
            if (ImGui::MenuItem(t->menuSave, "Ctrl+S", false, g_app.fileLoaded))
                SaveChanges();
            if (ImGui::MenuItem(t->menuSaveAs, "Ctrl+Shift+S", false, g_app.fileLoaded)) {
                auto path = SaveFileDialog();
                if (!path.empty()) {
                    snprintf(g_app.saveAsPath, sizeof(g_app.saveAsPath), "%s", WideToUtf8(path.c_str()).c_str());
                    g_app.saveMode = 2;
                    SaveChanges();
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem(t->menuExit, "Alt+F4"))
                PostQuitMessage(0);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(t->menuLanguage)) {
            if (ImGui::MenuItem("简体中文", nullptr, g_lang == Lang::ZH_CN)) {
                g_lang = Lang::ZH_CN;
                InitFields();
                // Re-read labels from loaded file won't change values
            }
            if (ImGui::MenuItem("English", nullptr, g_lang == Lang::EN)) {
                g_lang = Lang::EN;
                InitFields();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(t->menuHelp)) {
            if (ImGui::MenuItem(t->menuAbout)) g_app.showAbout = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // ---- Keyboard Shortcuts ----
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O)) {
        auto path = OpenFileDialog(L"Executable (*.exe)\0*.exe\0DLL (*.dll)\0*.dll\0All Files (*.*)\0*.*\0");
        if (!path.empty()) LoadExeInfo(path);
    }
    if (io.KeyCtrl && !io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_S))
        SaveChanges();
    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_S)) {
        auto path = SaveFileDialog();
        if (!path.empty()) {
            snprintf(g_app.saveAsPath, sizeof(g_app.saveAsPath), "%s", WideToUtf8(path.c_str()).c_str());
            g_app.saveMode = 2;
            SaveChanges();
        }
    }

    // ---- Main Window ----
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - ImGui::GetFrameHeight()));
    ImGui::Begin("##Main", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    if (!g_app.fileLoaded) {
        // ---- Drop Zone ----
        float zoneW = 420, zoneH = 140;
        ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - zoneW) * 0.5f, (io.DisplaySize.y - zoneH) * 0.5f - 30));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
        ImGui::BeginChild("DropZone", ImVec2(zoneW, zoneH), true,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Dummy(ImVec2(0, 15));
        ImGui::SetCursorPosX((zoneW - ImGui::CalcTextSize(t->dropHint).x) * 0.5f);
        ImGui::Text("%s", t->dropHint);
        ImGui::Dummy(ImVec2(0, 10));
        float btnW = 180;
        ImGui::SetCursorPosX((zoneW - btnW) * 0.5f);
        if (ImGui::Button(t->clickToOpen, ImVec2(btnW, 36))) {
            auto path = OpenFileDialog(L"Executable (*.exe)\0*.exe\0DLL (*.dll)\0*.dll\0All Files (*.*)\0*.*\0");
            if (!path.empty()) LoadExeInfo(path);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    } else {
        // ---- Editor ----
        float leftW = io.DisplaySize.x * 0.38f;
        float rightW = io.DisplaySize.x - leftW - 30;

        // -- Left Panel --
        ImGui::BeginChild("Left", ImVec2(leftW, 0), true);

        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", t->fileInfo);
        ImGui::Separator();
        ImGui::TextWrapped("%s: %s", t->fileName, g_app.filePathUtf8.c_str());
        try {
            auto sz = std::filesystem::file_size(g_app.filePath);
            ImGui::Text("%s: %.2f %s", t->fileSize, sz > 1048576 ? sz / 1048576.0 : sz / 1024.0,
                        sz > 1048576 ? "MB" : "KB");
        } catch (...) {}

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", t->versionNumbers);
        ImGui::Separator();
        ImGui::SetNextItemWidth(200);
        ImGui::InputText(t->fileVersion, g_app.fileVersion, sizeof(g_app.fileVersion));
        HelpMarker(t->fileVersionTip);
        ImGui::SetNextItemWidth(200);
        ImGui::InputText(t->productVersion, g_app.productVersion, sizeof(g_app.productVersion));
        HelpMarker(t->productVersionTip);

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", t->saveOptions);
        ImGui::Separator();
        ImGui::RadioButton(t->saveOverwrite, &g_app.saveMode, 0);
        ImGui::RadioButton(t->saveBackup, &g_app.saveMode, 1);
        ImGui::RadioButton(t->saveAsNew, &g_app.saveMode, 2);
        if (g_app.saveMode == 2) {
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##SavePath", g_app.saveAsPath, sizeof(g_app.saveAsPath));
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", t->icon);
        HelpMarker(t->iconTip);
        ImGui::Separator();
        ImGui::SetNextItemWidth(-80);
        ImGui::InputText("##Icon", g_app.iconPath, sizeof(g_app.iconPath));
        ImGui::SameLine();
        if (ImGui::SmallButton(t->btnBrowse)) {
            auto path = OpenFileDialog(L"Icon (*.ico)\0*.ico\0All Files (*.*)\0*.*\0");
            if (!path.empty()) snprintf(g_app.iconPath, sizeof(g_app.iconPath), "%s", WideToUtf8(path.c_str()).c_str());
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", t->execLevel);
        ImGui::Separator();
        ImGui::RadioButton(t->execDefault,  &g_app.executionLevel, 0);
        ImGui::RadioButton(t->execInvoker,  &g_app.executionLevel, 1);
        ImGui::RadioButton(t->execHighest,  &g_app.executionLevel, 2);
        ImGui::RadioButton(t->execAdmin,    &g_app.executionLevel, 3);

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", t->manifest);
        HelpMarker(t->manifestTip);
        ImGui::Separator();
        ImGui::SetNextItemWidth(-80);
        ImGui::InputText("##Manifest", g_app.manifestPath, sizeof(g_app.manifestPath));
        ImGui::SameLine();
        if (ImGui::SmallButton(t->btnBrowse)) {
            auto path = OpenFileDialog(L"Manifest (*.xml;*.manifest)\0*.xml;*.manifest\0All Files (*.*)\0*.*\0");
            if (!path.empty()) snprintf(g_app.manifestPath, sizeof(g_app.manifestPath), "%s", WideToUtf8(path.c_str()).c_str());
        }

        ImGui::EndChild();
        ImGui::SameLine();

        // -- Right Panel: Version Strings --
        ImGui::BeginChild("Right", ImVec2(0, 0), true);
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", t->versionStrings);
        ImGui::Separator();

        for (size_t i = 0; i < g_versionFields.size(); i++) {
            auto& field = g_versionFields[i];
            ImGui::PushID((int)i);

            // Label + tooltip
            ImGui::Text("%s", field.info->label);
            HelpMarker(field.info->tip);

            // Input
            bool modified = strcmp(field.value, field.original) != 0;
            if (modified) ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##v", field.value, sizeof(field.value));
            if (modified) {
                ImGui::PopStyleColor();
                ImGui::SameLine();
                if (ImGui::SmallButton(t->btnReset))
                    snprintf(field.value, sizeof(field.value), "%s", field.original);
            }

            ImGui::PopID();
        }

        ImGui::Spacing();
        if (ImGui::Button(t->btnClearAll)) {
            for (auto& f : g_versionFields) f.value[0] = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button(t->btnResetAll)) {
            for (auto& f : g_versionFields) snprintf(f.value, sizeof(f.value), "%s", f.original);
            snprintf(g_app.fileVersion, sizeof(g_app.fileVersion), "%s", g_app.originalFileVersion);
            snprintf(g_app.productVersion, sizeof(g_app.productVersion), "%s", g_app.originalProductVersion);
            g_app.iconPath[0] = 0;
            g_app.executionLevel = 0;
            g_app.manifestPath[0] = 0;
        }

        ImGui::EndChild();
    }

    ImGui::End();

    // ---- Status Bar ----
    ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - 28));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 28));
    ImGui::Begin("##Status", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    if (g_app.statusTime > 0.0f) {
        ImVec4 col = g_app.statusIsError ? ImVec4(1, 0.3f, 0.3f, 1) : ImVec4(0.3f, 1, 0.3f, 1);
        ImGui::TextColored(col, "%s", g_app.statusMsg);
    } else {
        ImGui::Text("%s - %s", t->statusReady,
            g_app.fileLoaded ? GetFileName(g_app.filePathUtf8).c_str() : t->statusNoFile);
    }
    ImGui::End();

    // ---- About ----
    if (g_app.showAbout) { ImGui::OpenPopup(t->aboutTitle); g_app.showAbout = false; }
    if (ImGui::BeginPopupModal(t->aboutTitle, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("RceditGUI v1.1.0");
        ImGui::Separator();
        ImGui::Text("Based on rcedit by Electron team");
        ImGui::Text("Built with Dear ImGui + DirectX 11");
        ImGui::Spacing();
        if (ImGui::Button("OK", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

// ============================================================
// Win32 + D3D11
// ============================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    InitFields();

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0, 0,
        hInstance, nullptr, nullptr, nullptr, nullptr, L"RceditGUI", nullptr };
    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowW(wc.lpszClassName, L"RceditGUI",
        WS_OVERLAPPEDWINDOW, 100, 100, 1280, 860,
        nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd)) { CleanupDeviceD3D(); UnregisterClassW(wc.lpszClassName, wc.hInstance); return 1; }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);
    DragAcceptFiles(hwnd, TRUE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr; // No imgui.ini

    // Load Chinese font
    ImFontConfig fontCfg;
    fontCfg.OversampleH = 2;
    fontCfg.OversampleV = 2;
    // Try Microsoft YaHei (common on Chinese Windows)
    const char* fontPaths[] = {
        "C:/Windows/Fonts/msyh.ttc",
        "C:/Windows/Fonts/msyhbd.ttc",
        "C:/Windows/Fonts/simhei.ttf",
        "C:/Windows/Fonts/simsun.ttc",
        nullptr
    };
    bool fontLoaded = false;
    for (int i = 0; fontPaths[i]; i++) {
        if (std::filesystem::exists(fontPaths[i])) {
            io.Fonts->AddFontFromFileTTF(fontPaths[i], 18.0f, &fontCfg, io.Fonts->GetGlyphRangesChineseFull());
            fontLoaded = true;
            break;
        }
    }
    if (!fontLoaded) {
        io.Fonts->AddFontDefault();
    }

    // Style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.WindowPadding = ImVec2(14, 14);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(10, 8);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 14.0f;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool running = true;
    MSG msg;
    while (running) {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) running = false;
        }
        if (!running) break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        RenderGUI();
        ImGui::Render();

        const float bg[4] = {0.12f, 0.12f, 0.14f, 1.0f};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, bg);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    CoUninitialize();
    return 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    switch (msg) {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_DROPFILES:
        HandleDrop((HDROP)wParam);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL levels[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0};
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        levels, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;
    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
