param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$TargetDir,

    [Parameter(Mandatory=$false)]
    [switch]$Clear
)

# ディレクトリの存在確認
if (-not (Test-Path $TargetDir -PathType Container)) {
    Write-Error "ディレクトリが見つかりません: $TargetDir"
    exit
}

# ターゲット日付
$today = Get-Date -Format "yyyyMMdd"
$aexFiles = Get-ChildItem -Path $TargetDir -Filter "*.aex"

foreach ($file in $aexFiles) {
    $baseName = $file.BaseName
    $extension = $file.Extension

    if ($Clear) {
        # --- 削除モード ---
        # 末尾の "_数字8桁" を探して削除する
        if ($baseName -match "(_\d{8})$") {
            $matchedDate = $Matches[1]
            $newName = $baseName.Substring(0, $baseName.Length - $matchedDate.Length) + $extension
            
            try {
                Rename-Item -Path $file.FullName -NewName $newName -ErrorAction Stop
                Write-Host "Cleared: $($file.Name) -> $newName" -ForegroundColor Yellow
            } catch {
                Write-Host "Error: Could not clear $($file.Name)" -ForegroundColor Red
            }
        }
    } else {
        # --- 付与モード ---
        # すでに日付が付いているかチェック
        if ($baseName -match "_\d{8}$") {
            Write-Host "Skipped: $($file.Name) (Already has a date stamp)" -ForegroundColor Gray
            continue
        }

        $newName = "{0}_{1}{2}" -f $baseName, $today, $extension
        try {
            Rename-Item -Path $file.FullName -NewName $newName -ErrorAction Stop
            Write-Host "Renamed: $($file.Name) -> $newName" -ForegroundColor Green
        } catch {
            Write-Host "Error: Failed to rename $($file.Name)" -ForegroundColor Red
        }
    }
}

Write-Host "`n処理が完了しました。" -ForegroundColor Cyan