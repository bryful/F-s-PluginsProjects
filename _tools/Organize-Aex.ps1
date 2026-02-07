param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$TargetDir
)

# ディレクトリの存在確認
if (-not (Test-Path $TargetDir -PathType Container)) {
    Write-Error "ディレクトリが見つかりません: $TargetDir"
    exit
}

# ターゲットディレクトリ内の .aex ファイルをすべて取得
$aexFiles = Get-ChildItem -Path $TargetDir -Filter "*.aex"

if ($aexFiles.Count -eq 0) {
    Write-Warning "指定されたディレクトリに .aex ファイルが見つかりませんでした。"
    exit
}

# シグネチャ設定
$signature = [byte[]](0x4D, 0x49, 0x42, 0x38, 0x67, 0x74, 0x61, 0x63)
$sigLen = $signature.Length

foreach ($file in $aexFiles) {
    Write-Host "Processing: $($file.Name)..." -NoNewline
    
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    $foundCategory = $null

    # バイナリスキャン
    for ($i = 0; $i -le ($bytes.Length - $sigLen); $i++) {
        $match = $true
        for ($j = 0; $j -lt $sigLen; $j++) {
            if ($bytes[$i + $j] -ne $signature[$j]) {
                $match = $false
                break
            }
        }

        if ($match) {
            $lengthIndex = $i + 16
            if ($lengthIndex -lt $bytes.Length) {
                $stringLength = $bytes[$lengthIndex]
                $stringStartIndex = $i + 17
                
                if ($stringStartIndex + $stringLength -le $bytes.Length) {
                    $categoryBytes = $bytes[$stringStartIndex..($stringStartIndex + $stringLength - 1)]
                    # カテゴリ名を取得し、Windowsのフォルダ名に使えない文字を除去
                    $rawName = [System.Text.Encoding]::UTF8.GetString($categoryBytes).Trim()
                    $foundCategory = $rawName -replace '[\\\/:*?"<>|]', '_'
                    break 
                }
            }
        }
    }

    if ($foundCategory) {
        # 移動先のフォルダパスを作成
        $destFolder = Join-Path $TargetDir $foundCategory
        
        # フォルダが存在しなければ作成
        if (-not (Test-Path $destFolder)) {
            New-Item -ItemType Directory -Path $destFolder | Out-Null
        }

        # ファイルを移動
        try {
            Move-Item -Path $file.FullName -Destination $destFolder -Force
            Write-Host " -> Moved to [$foundCategory]" -ForegroundColor Green
        } catch {
            Write-Host " -> Error moving file: $($_.Exception.Message)" -ForegroundColor Red
        }
    } else {
        Write-Host " -> Signature not found. Skipped." -ForegroundColor Yellow
    }
}

Write-Host "`n完了しました。" -ForegroundColor Cyan