# --- 実行スイッチ ---
$doUpdatePlatformToolset   = $false
$doUpdateLanguageStandard  = $true
$doUpdateOutDirDebug       = $true
$doUpdateOutDirRelease     = $true
$doUpdateWarningLevel      = $true
$doDisableWarnings         = $true
$doUpdateDebuggerCommand   = $true

# --- 設定値 ---
$targetFolder = "./" 
$newOutDirDebug = "C:\Program Files\Adobe\Common\Plug-ins\7.0\MediaCore\Fs_Plugins_Debug\"
$newOutDirRelease = "..\..\Fs_plugins_release\"
$newPlatformToolset = "v145"
$newLanguageStandard = "stdcpp20"
$newWarningLevel = "Level3"
$disableWarnings = "4201;4103;4189;4505;4996;4100;4458;4456"
# 書き換えたいAEのパス
$newDebugCommand = "C:\Program Files\Adobe\Adobe After Effects 2025\Support Files\AfterFX.exe"

$projFiles = Get-ChildItem -Path $targetFolder -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Checking: $($file.Name)" -ForegroundColor Cyan
    
    [xml]$xml = Get-Content $file.FullName
    $ns = New-Object Xml.XmlNamespaceManager $xml.NameTable
    $ns.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003")

    $isChanged = $false

    # --- 1. デバッグコマンドの更新 (ColorSwitchのような1行XMLにも対応) ---
    if ($doUpdateDebuggerCommand) {
        # Debug構成を含むすべての PropertyGroup を対象に、適切な挿入場所を探す
        $debugGroups = $xml.SelectNodes("//ns:PropertyGroup[contains(@Condition, 'Debug')]", $ns)
        foreach ($group in $debugGroups) {
            # すでにタグがあるかチェック
            if ($null -ne $group.LocalDebuggerCommand) {
                if ($group.LocalDebuggerCommand -ne $newDebugCommand) {
                    $group.LocalDebuggerCommand = $newDebugCommand
                    $isChanged = $true
                }
            } 
            # タグがない場合、Label属性がないメインのPropertyGroupに追加を試みる
            elseif ($null -eq $group.Attributes["Label"]) {
                $newElem = $xml.CreateElement("LocalDebuggerCommand", $ns.LookupNamespace("ns"))
                $newElem.InnerText = $newDebugCommand
                $group.AppendChild($newElem)
                $isChanged = $true
            }
        }
    }

    # --- 2. OutDir / ClCompile 等の同期 (既存ロジック) ---
    $outDirs = $xml.SelectNodes("//ns:PropertyGroup/ns:OutDir", $ns)
    foreach ($node in $outDirs) {
        if ($doUpdateOutDirDebug -and $node.Condition -like "*Debug*" -and $node.InnerText -ne $newOutDirDebug) { $node.InnerText = $newOutDirDebug; $isChanged = $true }
        if ($doUpdateOutDirRelease -and $node.Condition -like "*Release*" -and $node.InnerText -ne $newOutDirRelease) { $node.InnerText = $newOutDirRelease; $isChanged = $true }
    }

    $clNodes = $xml.SelectNodes("//ns:ItemDefinitionGroup[contains(@Condition, 'Debug') or contains(@Condition, 'Release')]/ns:ClCompile", $ns)
    foreach ($node in $clNodes) {
        if ($doUpdateLanguageStandard -and ($null -eq $node.LanguageStandard -or $node.LanguageStandard -ne $newLanguageStandard)) { $node.LanguageStandard = $newLanguageStandard; $isChanged = $true }
        if ($doUpdateWarningLevel -and $node.WarningLevel -ne $newWarningLevel) { $node.WarningLevel = $newWarningLevel; $isChanged = $true }
        if ($doDisableWarnings) {
            $val = "$disableWarnings;`$(DisableSpecificWarnings)"
            if ($node.DisableSpecificWarnings -ne $val) { $node.DisableSpecificWarnings = $val; $isChanged = $true }
        }
    }

    # --- 3. 保存 (引用符破損防止) ---
    if ($isChanged) {
        # 1行XMLでも構造を壊さないように保存
        $xmlText = $xml.OuterXml -replace '&quot;', '"'
        $utf8Encoding = New-Object System.Text.UTF8Encoding($true)
        [System.IO.File]::WriteAllText($file.FullName, $xmlText, $utf8Encoding)
        Write-Host "  -> Updated." -ForegroundColor Green
    } else {
        Write-Host "  -> No changes needed." -ForegroundColor DarkGray
    }
}