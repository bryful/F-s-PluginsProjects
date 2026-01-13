#include "FsSettings.h"
// プラグイン設定ファイル用ディレクトリを返すユーティリティ
// 失敗した場合は空文字列を返す
static bool EnsureDirectoryExists(const std::string& dir)
{
    if (dir.empty()) return false;
    std::string path = dir;
    std::replace(path.begin(), path.end(), '/', '\\');

    std::string cur;
    size_t pos = 0;
    // ドライブ指定 (C:\...) の場合は先頭を初期化
    if (path.size() >= 2 && path[1] == ':') {
        cur = path.substr(0, 3); // "C:\"
        pos = 3;
    }
    // UNC パスなど先頭が "\\" の場合はそのまま扱う
    else if (path.size() >= 2 && path[0] == '\\' && path[1] == '\\') {
        cur = "\\\\";
        pos = 2;
    }

    while (pos < path.size()) {
        size_t next = path.find('\\', pos);
        std::string part;
        if (next == std::string::npos) {
            part = path.substr(pos);
            pos = path.size();
        }
        else {
            part = path.substr(pos, next - pos);
            pos = next + 1;
        }
        if (part.empty()) continue;

        if (!cur.empty() && cur.back() != '\\') cur += '\\';
        cur += part;

        DWORD fa = ::GetFileAttributesA(cur.c_str());
        if (fa == INVALID_FILE_ATTRIBUTES) {
            if (!::CreateDirectoryA(cur.c_str(), NULL)) {
                DWORD e = ::GetLastError();
                if (e != ERROR_ALREADY_EXISTS) return false;
            }
        }
        else {
            if (!(fa & FILE_ATTRIBUTE_DIRECTORY)) return false;
        }
    }
    return true;
}

// ユーザー単位の設定保存先ディレクトリを返す。
// 例: "%LOCALAPPDATA%\F_sPlugins\blue_Text"
// 作成できなければ空文字列を返す。
static std::string GetPluginConfigDir()
{
    const char* local = std::getenv("LOCALAPPDATA");
    const char* roam = std::getenv("APPDATA");
    std::string base;

    if (local && local[0] != '\0') base = local;
    else if (roam && roam[0] != '\0') base = roam;
    else return std::string();

    // 企業名/プラグイン名は必要に応じて変更してください
    std::string dir = base + "\\FsPlugins";

    if (!EnsureDirectoryExists(dir)) return std::string();
    return dir;
}

// 指定したファイル名をプラグイン設定ディレクトリ内のフルパスで返す（存在ディレクトリを作成する）
// 失敗時は空文字列
static std::string GetPluginConfigFilePathL(const std::string& filename)
{
    if (filename.empty()) return std::string();
    std::string dir = GetPluginConfigDir();
    if (dir.empty()) return std::string();
    if (dir.back() != '\\') dir += '\\';
    return dir + filename;
}
std::string GetPluginConfigFilePath(const std::string& filename)
{
    return GetPluginConfigFilePathL(filename);
}