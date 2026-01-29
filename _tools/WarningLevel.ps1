# 1. ソリューションのルートフォルダを指定
$solutionRoot = "./" 

# すべての vcxproj を取得
$projFiles = Get-ChildItem -Path $solutionRoot -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Processing: $($file.Name)" -ForegroundColor Cyan
    
    # XMLとして読み込み
    [xml]$xml = Get-Content $file.FullName
    
    # MSBuild用ネームスペースの設定
    $ns = New-Object Xml.XmlNamespaceManager $xml.NameTable
    $ns.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003")

    # 2. ターゲットを抽出（Debug, Release, Release-MT すべてを対象にする場合）
    # 特定の構成だけにしたい場合は [contains(...)] の中身を調整してください
    $xpath = "//ns:ItemDefinitionGroup[contains(@Condition, 'Debug') or contains(@Condition, 'Release')]/ns:ClCompile"
    $targetNodes = $xml.SelectNodes($xpath, $ns)

    if ($targetNodes.Count -gt 0) {
        foreach ($node in $targetNodes) {
            # 3. WarningLevelの操作
            if ($null -ne $node.WarningLevel) {
                # 既にタグがある場合は書き換え
                $node.WarningLevel = "Level3"
            } else {
                # タグがない場合は新規作成して追加
                $newElem = $xml.CreateElement("WarningLevel", "http://schemas.microsoft.com/developer/msbuild/2003")
                $newElem.InnerText = "Level4"
                $node.AppendChild($newElem)
            }
            
            # おまけ：TreatWarningAsError（警告をエラーとして扱う）も一括でTrueにする場合
            if ($null -ne $node.TreatWarningAsError) {
                $node.TreatWarningAsError = "true"
            }
        }
        
        # 保存（UTF-8 BOM付きで保存されます）
        $xml.Save($file.FullName)
        Write-Host "  Done!" -ForegroundColor Green
    } else {
        Write-Host "  Target nodes not found." -ForegroundColor Yellow
    }
}