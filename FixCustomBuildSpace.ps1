# --- 設定 ---
$targetFolder = "./" # vcxprojがあるフォルダ

# 全vcxproj取得
$projFiles = Get-ChildItem -Path $targetFolder -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Target: $($file.Name)" -ForegroundColor Cyan
    
    # 1. テキストとして一括読み込み（BOM付きUTF-8を維持）
    $content = [System.IO.File]::ReadAllText($file.FullName)
    $original = $content

    # --- 修正1: 不正な半角スペースの削除 ---
    # "..\ %(Filename).r" を "..\%(Filename).r" に修正
    $content = $content.Replace('"..\ %(Filename).r"', '"..\%(Filename).r"')

    # --- 修正2: 属性内の不正な改行コード実体参照を「生の改行」に戻す ---
    # これがビルド失敗（メタデータ評価エラー）の主因です
    $content = $content -replace '&#xD;&#xA;', "`r`n"

    # --- 修正3: 属性内の引用符実体参照を「生の二重引用符」に戻す ---
    $content = $content -replace '&quot;', '"'

    # --- 修正4: 万が一重複した二重引用符があれば掃除 ---
    $content = $content -replace '""', '"'

    # 2. 変更があった場合のみ保存（XML操作を通さず直接上書き）
    if ($content -ne $original) {
        $utf8Encoding = New-Object System.Text.UTF8Encoding($true) # BOMありUTF-8
        [System.IO.File]::WriteAllText($file.FullName, $content, $utf8Encoding)
        Write-Host "  -> Successfully Fixed." -ForegroundColor Green
    } else {
        Write-Host "  -> No issues found." -ForegroundColor DarkGray
    }
}

Write-Host "`n修正が完了しました。ColorChangeEng等が正常にビルドできるか確認してください。" -ForegroundColor White