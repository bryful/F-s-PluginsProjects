param (
    [switch]$write,
    [switch]$outflag,
    [switch]$outflag2,
    [string]$calc = "",
    [string]$routflag = "",   # 数値を受け取る
    [string]$routflag2 = "",  # 数値を受け取る
    [string]$JsonPath = "" 
)

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $JsonPath) { $JsonPath = Join-Path $ScriptDir "AfterEffect_H.json" }

function Show-Help {
    Write-Host @"
AE_FlagManager - After Effects SDK Flag Utility

Usage:
  .\AE_FlagManager.ps1 -write               : 全リストを書き出し (; 付き)
  .\AE_FlagManager.ps1 -outflag             : outFlags1 を書き出し (; 付き)
  .\AE_FlagManager.ps1 -outflag2            : outFlags2 を書き出し (; 付き)
  .\AE_FlagManager.ps1 -calc <File>         : 有効な行の値を合算
  .\AE_FlagManager.ps1 -routflag <Value>    : 値に含まれるFlagの ";" を外して書き出し
  .\AE_FlagManager.ps1 -routflag2 <Value>   : 値に含まれるFlag2の ";" を外して書き出し
"@ -ForegroundColor Yellow
}

if ($PSBoundParameters.Count -eq 0) { Show-Help; exit }
if (-not (Test-Path $JsonPath)) { Write-Error "JSON Not Found"; exit }
$data = Get-Content $JsonPath -Raw | ConvertFrom-Json

# リスト書き出し関数 (Reverseモード対応)
function Export-List($key, $reverseValue = $null) {
    $fileName = Join-Path $ScriptDir "$key.txt"
    $targetVal = [uint64]0
    if ($reverseValue) {
        if ($reverseValue -like "0x*") { $targetVal = [Convert]::ToUInt64($reverseValue, 16) }
        else { $targetVal = [uint64]$reverseValue }
    }

    $lines = foreach ($item in $data.$key) {
        $val = [uint64]$item.value
        # ビットが含まれているか判定 (AND演算)
        if ($reverseValue -ne $null -and ($targetVal -band $val) -eq $val -and $val -ne 0) {
            "$($item.name)//$($item.description)" # セミコロンなし
        } else {
            "; $($item.name)//$($description = $item.description; $description)" # セミコロンあり
        }
    }
    $lines | Out-File -FilePath $fileName -Encoding utf8
    Write-Host "Success: $fileName を作成しました。" -ForegroundColor Green
}

# --- メイン処理 ---
if ($calc) {
    if (-not (Test-Path $calc)) { Write-Error "$calc Not Found"; exit }
    $totalValue = [uint64]0
    $found1 = $false; $found2 = $false
    Get-Content $calc | ForEach-Object {
        $line = $_.Trim()
        if ($line -and $line -notmatch "^(//|/\*|;|#)") {
            $name = ($line -replace "^;\s*", "").Split("//")[0].Trim()
            $i1 = $data.outFlags1 | Where-Object { $_.name -eq $name }
            $i2 = $data.outFlags2 | Where-Object { $_.name -eq $name }
            if ($i1) { $found1 = $true; $totalValue = $totalValue -bor [uint64]$i1.value }
            if ($i2) { $found2 = $true; $totalValue = $totalValue -bor [uint64]$i2.value }
        }
    }
    if ($found1 -and $found2) { Write-Error "Mixed Groups Error"; exit }
    Write-Host "Result: 10進:$totalValue / 16進:0x$($totalValue.ToString("X8"))"
}

if ($routflag)  { Export-List "outFlags1" $routflag }
if ($routflag2) { Export-List "outFlags2" $routflag2 }
if ($write)     { Export-List "outFlags1"; Export-List "outFlags2" }
if ($outflag -and -not $routflag)  { Export-List "outFlags1" }
if ($outflag2 -and -not $routflag2) { Export-List "outFlags2" }