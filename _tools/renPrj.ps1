param (
    [Parameter(Mandatory=$true)][string]$TargetDir,
    [Parameter(Mandatory=$true)][string]$OldText,
    [Parameter(Mandatory=$true)][string]$NewText
)

# 拡張子の定義
$extensions = "*.cpp", "*.h", "*.hpp", "*.c", "*.cxx", "*.r","*.rc", "*.sln","*.slnx", "*.vcxproj", "*.filters"

Write-Host "--- 1. 内容置換 & 文字コード変換を開始します ---" -ForegroundColor Cyan

$files = Get-ChildItem -Path $TargetDir -Include $extensions -Recurse
foreach ($file in $files) {
    # ファイルをバイナリとして読み込み、簡易的に文字コードを判定
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    
    # BOM(EF BB BF)があるかチェック
    $isUtf8Bom = ($bytes.Count -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF)
    
    if ($isUtf8Bom) {
        $content = Get-Content -Path $file.FullName -Raw -Encoding utf8
    } else {
        # BOMがない場合は一旦Shift-JIS(932)として読み込む
        $content = Get-Content -Path $file.FullName -Raw -Encoding String
    }

    # 文字列置換
    $newContent = $content -replace $OldText, $NewText
    
    # UTF-8(BOM付き)で保存
    # 内容が変わっていなくても、Shift-JISだったものはここでUTF-8に変換されます
    $newContent | Out-File -FilePath $file.FullName -Encoding utf8 -Force
    Write-Host "Processed: $($file.Name)"
}

Write-Host "`n--- 2. 名前（ファイル・フォルダ）の置換を開始します ---" -ForegroundColor Cyan

# 親ディレクトリ自体も置換対象に含めるため、$TargetDir 自体もリストに加える
$items = Get-ChildItem -Path $TargetDir -Recurse | Sort-Object {$_.FullName.Length} -Descending
$items += Get-Item -Path $TargetDir

foreach ($item in $items) {
    if ($item.Name -like "*$OldText*") {
        $newName = $item.Name -replace $OldText, $NewText
        $parentPath = Split-Path -Path $item.FullName -Parent
        $destination = Join-Path -Path $parentPath -ChildPath $newName
        
        if ($item.FullName -ne $destination) {
            Write-Host "Renaming: $($item.Name) -> $newName"
            Move-Item -Path $item.FullName -Destination $destination -Force
        }
    }
}

Write-Host "`nすべての処理が完了しました。" -ForegroundColor Green