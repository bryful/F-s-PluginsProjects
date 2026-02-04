# --- 修復スクリプト ---
$targetFolder = "./" 

# 全vcxproj取得
$projFiles = Get-ChildItem -Path $targetFolder -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Force Repairing (Binary Level): $($file.Name)" -ForegroundColor Cyan
    
    # 1. ファイルをテキストとして一括読み込み
    $text = [System.IO.File]::ReadAllText($file.FullName)

    # 2. 破壊の原因となっているXML実体参照を「生の文字」に置換
    # これらが原因で MSBuild のメタデータ評価が壊れています
    $fixed = $text -replace '&#xD;&#xA;', "`r`n"  # XML実体参照の改行を生の改行に戻す
    $fixed = $fixed -replace '&quot;', '"'       # &quot; を " に戻す

    # 3. 前回の失敗で発生した余計なクォートの重なりを掃除
    $fixed = $fixed -replace '""', '"'
    $fixed = $fixed -replace '\\\\"', '"'
    $fixed = $fixed -replace '\\"\\"', '"'
    
    # 4. メタデータ記述の正規化 (NGファイルに見られた構文ミスを修正)
    $fixed = $fixed -replace '%"FullPath"', '%(FullPath)'
    $fixed = $fixed -replace '%"Filename"', '%(Filename)'

    # 5. 保存（Visual Studio が正しく認識できる BOM付きUTF-8 で書き出し）
    $utf8Encoding = New-Object System.Text.UTF8Encoding($true)
    [System.IO.File]::WriteAllText($file.FullName, $fixed, $utf8Encoding)
    
    Write-Host "  -> Repair Finished." -ForegroundColor Green
}

Write-Host "`nすべての修復が完了しました。Visual Studio で開き直してください。" -ForegroundColor White