#include "NF_Settings.h"
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
// 例: "%LOCALAPPDATA%\F_sPlugins\TargetGrad"
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
    std::string dir = base + "\\NF-Plugins";

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

std::string loadTextFile(std::string name)
{
    std::string ret;
    std::string p = GetPluginConfigFilePath(name);
    if (!p.empty())
    {
        std::ifstream f(p.c_str(), std::ios::binary);
        if (f.is_open() && f.good())
        {
            std::ifstream ifs(p);
            if (ifs.good())
            {
                ifs >> ret;
            }
        }
    }
    return ret;
}
void saveTextFile(std::string name, std::string txt)
{
    std::string ret;
    std::string p = GetPluginConfigFilePath(name);
    if (!p.empty())
    {
        std::ifstream f(p.c_str(), std::ios::binary);
        if (p.empty() == FALSE)
        {
            std::ofstream writing_file;
            writing_file.open(p, std::ios::out);
            writing_file << txt << std::endl;
            writing_file.close();
        }
    }
}
std::string SaveFileDialog(
    std::string title,
    std::string defp,
    std::vector<std::string> fltp,
    std::string des
)
{
    std::vector<const char*> filterPatterns;
    for (const auto& pattern : fltp)
    {
        filterPatterns.push_back(pattern.c_str());
    }
    const char* selectedFile = tinyfd_saveFileDialog(
        title.c_str(),                      // ダイアログのタイトル
        defp.c_str(),                       // 初期ディレクトリ
        static_cast<int>(filterPatterns.size()),              // フィルタパターンの数
        filterPatterns.data(),              // フィルタパターン
        des.c_str()                         // フィルタの説明
    );
    std::string ret;
    if (selectedFile)
    {
        ret = std::string(selectedFile);
    }

    return ret;
}
std::string OpenFileDialog(
    std::string title, 
    std::string defp, 
    std::vector<std::string> fltp,
    std::string des
)
{
    std::vector<const char*> filterPatterns;
    for (const auto& pattern : fltp)
    {
        filterPatterns.push_back(pattern.c_str());
    }
    
    const char* selectedFile = tinyfd_openFileDialog(
        title.c_str(),                      // ダイアログのタイトル
        defp.c_str(),                       // 初期ディレクトリ
        static_cast<int>(filterPatterns.size()),              // フィルタパターンの数
        filterPatterns.data(),              // フィルタパターン
        des.c_str(),                        // フィルタの説明
        0                                   // マルチセレクトの可否 (0 = No, 1 = Yes)
    );
    std::string ret;
    if (selectedFile)
    {
        ret = std::string(selectedFile);
    }

    return ret;
}

std::string ChangeFileExtension(const std::string& filename, const std::string& newExtension)
{
    // 最後のパス区切り文字の位置を検索
    size_t lastSlash = filename.find_last_of("\\/");
    // 最後の '.' の位置を検索
    size_t lastDot = filename.find_last_of('.');

    // '.' がパス区切り文字より後にある場合のみ拡張子として扱う
    if (lastDot != std::string::npos &&
        (lastSlash == std::string::npos || lastDot > lastSlash))
    {
        // 拡張子を除いたベース名を取得
        std::string base = filename.substr(0, lastDot);

        // newExtensionが空の場合は拡張子を削除
        if (newExtension.empty())
        {
            return base;
        }

        // 新しい拡張子が '.' で始まっているか確認
        if (newExtension[0] == '.')
        {
            return base + newExtension;
        }
        else
        {
            return base + "." + newExtension;
        }
    }
    else
    {
        // 拡張子がない場合
        if (newExtension.empty())
        {
            // 空の拡張子なのでそのまま返す
            return filename;
        }

        // 拡張子を追加
        if (newExtension[0] == '.')
        {
            return filename + newExtension;
        }
        else
        {
            return filename + "." + newExtension;
        }
    }
}
std::string GetDocumentsFolderPath()
{
    char path[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path);

    if (SUCCEEDED(result))
    {
        return std::string(path);
    }

    // フォールバック: 環境変数を使用
    const char* userProfile = std::getenv("USERPROFILE");
    if (userProfile && userProfile[0] != '\0')
    {
        return std::string(userProfile) + "\\Documents";
    }

    return std::string();
}