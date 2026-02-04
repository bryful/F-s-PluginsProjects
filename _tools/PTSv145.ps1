# 1. ソリューションのルートフォルダを指定
$solutionRoot = "./" 

# すべての vcxproj を取得
$projFiles = Get-ChildItem -Path $solutionRoot -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Processing: $($file.Name)" -ForegroundColor Cyan
    
    [xml]$xml = Get-Content $file.FullName
    $ns = New-Object Xml.XmlNamespaceManager $xml.NameTable
    $ns.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003")

    # 2. Target: Label="Configuration" を持つすべての PropertyGroup を取得
    # AlphaFix.vcxproj ではここに PlatformToolset が記述されています
    $configGroups = $xml.SelectNodes("//ns:PropertyGroup[@Label='Configuration']", $ns)

    if ($configGroups.Count -gt 0) {
        foreach ($group in $configGroups) {
            # 3. PlatformToolset ノードの操作
            if ($null -ne $group.PlatformToolset) {
                # 既にタグがある場合は v145 に書き換え
                $group.PlatformToolset = "v145"
            } else {
                # タグがない場合は新規作成して追加
                $newElem = $xml.CreateElement("PlatformToolset", "http://schemas.microsoft.com/developer/msbuild/2003")
                $newElem.InnerText = "v145"
                $group.AppendChild($newElem)
            }
        }
        
        # 保存
        $xml.Save($file.FullName)
        Write-Host "  Successfully set PlatformToolset to v145." -ForegroundColor Green
    } else {
        Write-Host "  Configuration PropertyGroups not found." -ForegroundColor Yellow
    }
}