# --- 設定項目 ---
$rcFileName = "Resource.rc"
$companyName = "bry-ful"
$copyright   = "Copyright (C) 2026 bry-ful All rights reserved."
$description = "NF-Plugins/F's Plugins https://github.com/bryful/F-s-PluginsProjects/"
$productName = "NF-Plugins"
$version     = "1,0,0,0"
$versionStr  = "1.0.0.0"

# --- 雛形（ファイルがない場合に使用） ---
$template = @"
#include "winres.h"

VS_VERSION_INFO VERSIONINFO
 FILEVERSION $version
 PRODUCTVERSION $version
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x40004L
 FILETYPE 0x1L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "041104b0"
        BEGIN
            VALUE "CompanyName", "$companyName"
            VALUE "FileDescription", "$description"
            VALUE "FileVersion", "$versionStr"
            VALUE "InternalName", "$rcFileName"
            VALUE "LegalCopyright", "$copyright"
            VALUE "OriginalFilename", "$rcFileName"
            VALUE "ProductName", "$productName"
            VALUE "ProductVersion", "$versionStr"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x411, 1200
    END
END
"@

# 1. ファイルが存在しない場合は新規作成
if (-Not (Test-Path $rcFileName)) {
    $template | Out-File -FilePath $rcFileName -Encoding utf8
    Write-Host "Created new resource file: $rcFileName" -ForegroundColor Cyan
} else {
    # 2. 存在する場合は既存の項目を置換
    $content = Get-Content $rcFileName -Raw
    $content = $content -replace '(VALUE "CompanyName",\s*)"[^"]*"', "`$1`"$companyName`""
    $content = $content -replace '(VALUE "LegalCopyright",\s*)"[^"]*"', "`$1`"$copyright`""
    $content = $content -replace '(VALUE "FileDescription",\s*)"[^"]*"', "`$1`"$description`""
    $content = $content -replace '(VALUE "ProductName",\s*)"[^"]*"', "`$1`"$productName`""
    $content | Out-File -FilePath $rcFileName -Encoding utf8
    Write-Host "Updated existing resource file." -ForegroundColor Green
}