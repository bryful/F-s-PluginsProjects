$text = Get-Content "colorNameDialog.jsx" -Raw -Encoding UTF8
$hex = [System.Text.Encoding]::UTF8.GetBytes($text) | ForEach-Object { "0x{0:x2}" -f $_ }

# 文字列展開を確実に分ける
$joinedHex = $hex -join ','
$content = "static const char FS_NAME_EDIT_DIALOG[] = { " + $joinedHex + ", 0x00 };"

Set-Content "colorNameDialog_jsx.h" $content -Encoding UTF8