param (
    [Parameter(Mandatory=$false, Position=0)]
    [string]$TargetFile
)

# 引数が空だった場合の処理
if (-not $TargetFile) {
    Write-Host "Usage: .\Get-AexCategory.ps1 '.\filename.aex'" -ForegroundColor Yellow
    exit
}

# 相対パスを絶対パスに変換（エラー防止）
$FullPath = Resolve-Path $TargetFile -ErrorAction SilentlyContinue

if (-not $FullPath -or -not (Test-Path $FullPath)) {
    Write-Error "ファイルが見つかりません: $TargetFile"
    exit
}

try {
    # ファイルをバイト配列として読み込み
    $bytes = [System.IO.File]::ReadAllBytes($FullPath)
    
    # ターゲット: MIB8gtac
    $signature = [byte[]](0x4D, 0x49, 0x42, 0x38, 0x67, 0x74, 0x61, 0x63)
    $sigLen = $signature.Length
    $found = $false

    for ($i = 0; $i -le ($bytes.Length - $sigLen); $i++) {
        $match = $true
        for ($j = 0; $j -lt $sigLen; $j++) {
            if ($bytes[$i + $j] -ne $signature[$j]) {
                $match = $false
                break
            }
        }

        if ($match) {
            # 16バイト目が長さ、17バイト目から文字列
            $lengthIndex = $i + 16
            if ($lengthIndex -lt $bytes.Length) {
                $stringLength = $bytes[$lengthIndex]
                $stringStartIndex = $i + 17
                
                if ($stringStartIndex + $stringLength -le $bytes.Length) {
                    $categoryBytes = $bytes[$stringStartIndex..($stringStartIndex + $stringLength - 1)]
                    # AfterEffectsのPiPLリソースは通常ASCII/UTF8
                    $categoryName = [System.Text.Encoding]::UTF8.GetString($categoryBytes)
                    
                    Write-Host "--------------------------"
                    Write-Host "Found at Offset: 0x$($i.ToString("X"))" -ForegroundColor Cyan
                    Write-Host "Category Name  : $categoryName" -ForegroundColor Green
                    Write-Host "--------------------------"
                    $found = $true
                    break 
                }
            }
        }
    }

    if (-not $found) {
        Write-Warning "シグネチャ 'MIB8gtac' が見つかりませんでした。別のリソース形式の可能性があります。"
    }
}
catch {
    Write-Error "エラーが発生しました: $_"
}