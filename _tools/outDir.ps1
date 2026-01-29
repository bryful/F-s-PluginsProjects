# 1. ソリューションのルートフォルダを指定
$solutionRoot = "./" 

# すべての vcxproj を取得
$projFiles = Get-ChildItem -Path $solutionRoot -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Processing: $($file.Name)" -ForegroundColor Cyan
    
    [xml]$xml = Get-Content $file.FullName
    $ns = New-Object Xml.XmlNamespaceManager $xml.NameTable
    $ns.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003")

    # 2. XPath: Condition属性に 'Debug' が含まれる PropertyGroup 内の OutDir を取得
    # AlphaFix.vcxproj では <OutDir> は <PropertyGroup> 直下にあります 
    $xpath = "//ns:PropertyGroup/ns:OutDir[contains(@Condition, 'Debug')]"
    $outDirNodes = $xml.SelectNodes($xpath, $ns)

    if ($outDirNodes.Count -gt 0) {
        foreach ($node in $outDirNodes) {
            # 値の書き換え
            $node.InnerText = "..\..\Fs_Plugins_debug\"
            Write-Host "  -> Updated OutDir to: $($node.InnerText)" -ForegroundColor Green
        }
        
        # 保存
        $xml.Save($file.FullName)
    } else {
        # 万が一ノードがない場合（新規追加が必要な場合）
        Write-Host "  -> Debug OutDir node not found. Check if the project has a Debug config." -ForegroundColor Yellow
    }
}