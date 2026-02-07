param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$TargetDir,

    [Parameter(Mandatory=$false)]
    [switch]$Restore
)

# ディレクトリの存在確認
$targetPath = Resolve-Path $TargetDir
if (-not (Test-Path $targetPath -PathType Container)) {
    Write-Error "ディレクトリが見つかりません: $TargetDir"
    exit
}

if ($Restore) {
    # --- 元に戻す (Restore) モード ---
    Write-Host "Restoring files to: $targetPath" -ForegroundColor Yellow
    
    # 1階層下のサブフォルダ内の .aex を探す
    $subFolders = Get-ChildItem -Path $targetPath -Directory
    
    foreach ($folder in $subFolders) {
        $files = Get-ChildItem -Path $folder.FullName -Filter "*.aex"
        
        foreach ($file in $files) {
            $destPath = Join-Path $targetPath $file.Name
            
            # 同名ファイルが親にある場合の衝突回避
            if (Test-Path $destPath) {
                Write-Host "Skipped: $($file.Name) already exists in parent." -ForegroundColor Red
            } else {
                Move-Item -Path $file.FullName -Destination $targetPath
                Write-Host "Moved up: $($file.Name)" -ForegroundColor Green
            }
        }
        
        # フォルダが空になったら削除
        if ((Get-ChildItem -Path $folder.FullName).Count -eq 0) {
            Remove-Item -Path $folder.FullName
            Write-Host "Removed empty folder: $($folder.Name)" -ForegroundColor Gray
        }
    }
} else {
    # --- 整理 (Organize) モード ---
    $aexFiles = Get-ChildItem -Path $targetPath -Filter "*.aex"
    $signature = [byte[]](0x4D, 0x49, 0x42, 0x38, 0x67, 0x74, 0x61, 0x63)
    $sigLen = $signature.Length

    if ($aexFiles.Count -eq 0) { Write-Warning "整理対象の .aex がありません。"; exit }

    foreach ($file in $aexFiles) {
        Write-Host "Scanning: $($file.Name)..." -NoNewline
        $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
        $foundCategory = $null

        for ($i = 0; $i -le ($bytes.Length - $sigLen); $i++) {
            $match = $true
            for ($j = 0; $j -lt $sigLen; $j++) {
                if ($bytes[$i + $j] -ne $signature[$j]) { $match = $false; break }
            }
            if ($match) {
                $stringLength = $bytes[$i + 16]
                $stringStartIndex = $i + 17
                if ($stringStartIndex + $stringLength -le $bytes.Length) {
                    $categoryBytes = $bytes[$stringStartIndex..($stringStartIndex + $stringLength - 1)]
                    $foundCategory = ([System.Text.Encoding]::UTF8.GetString($categoryBytes)).Trim() -replace '[\\\/:*?"<>|]', '_'
                    break 
                }
            }
        }

        if ($foundCategory) {
            $destFolder = Join-Path $targetPath $foundCategory
            if (-not (Test-Path $destFolder)) { New-Item -ItemType Directory -Path $destFolder | Out-Null }
            Move-Item -Path $file.FullName -Destination $destFolder -Force
            Write-Host " -> [$foundCategory]" -ForegroundColor Green
        } else {
            Write-Host " -> No Category Found" -ForegroundColor Yellow
        }
    }
}

Write-Host "`n完了しました。" -ForegroundColor Cyan