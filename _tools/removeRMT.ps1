# 1. ソリューションのルートフォルダを指定
$solutionRoot = "./" 

# すべての vcxproj を取得
$projFiles = Get-ChildItem -Path $solutionRoot -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Processing: $($file.Name)" -ForegroundColor Cyan
    
    [xml]$xml = Get-Content $file.FullName
    $ns = New-Object Xml.XmlNamespaceManager $xml.NameTable
    $ns.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003")

    # 削除対象の抽出
    # A. ProjectConfiguration (Include属性に Release-MT を含むもの)
    $configNodes = $xml.SelectNodes("//ns:ProjectConfiguration[contains(@Include, 'Release-MT')]", $ns)

    # B. Condition属性に 'Release-MT' が含まれるすべての要素
    # (PropertyGroup, ImportGroup, ItemDefinitionGroup, その他ファイル個別設定など)
    $conditionNodes = $xml.SelectNodes("//* [contains(@Condition, 'Release-MT')]", $ns)

    # 削除実行
    $totalRemoved = 0

    # ProjectConfiguration の削除
    foreach ($node in $configNodes) {
        [void]$node.ParentNode.RemoveChild($node)
        $totalRemoved++
    }

    # Condition付き要素の削除
    foreach ($node in $conditionNodes) {
        # 既に親が削除されている可能性があるためチェック
        if ($node.ParentNode) {
            [void]$node.ParentNode.RemoveChild($node)
            $totalRemoved++
        }
    }

    if ($totalRemoved -gt 0) {
        # 保存
        $xml.Save($file.FullName)
        Write-Host "  Successfully removed $totalRemoved nodes related to 'Release-MT'." -ForegroundColor Green
    } else {
        Write-Host "  'Release-MT' not found." -ForegroundColor Yellow
    }
}