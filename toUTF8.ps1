# 変換対象の拡張子を指定
$extensions = "*.cpp", "*.h", "*.hpp", "*.c", "*.cxx", "*.r"

# カレントディレクトリ配下のファイルを再帰的に取得
$files = Get-ChildItem -Path . -Include $extensions -Recurse

foreach ($file in $files) {
    Write-Host "Processing: $($file.FullName)"
    
    # ファイルの内容をShift-JIS (Encoding 932) として読み込む
    # -Raw を使うことでファイル全体を一つの文字列として保持し、改行コードを維持します
    $content = Get-Content -Path $file.FullName -Encoding String -Raw
    
    # 元がShift-JISであることを前提に、UTF-8 (BOM付き) で上書き保存
    # UTF8はデフォルトでBOM付きになります
    $content | Out-File -FilePath $file.FullName -Encoding utf8 -Force
}

Write-Host "--- 変換が完了しました ---" -ForegroundColor Green