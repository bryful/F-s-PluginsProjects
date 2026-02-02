$text = Get-Content "dialog.jsx" -Raw
$hex = [System.Text.Encoding]::UTF8.GetBytes($text) | ForEach-Object { "0x{0:x2}" -f $_ }
$content = "static const char FS_ABOUT_DIALOG[] = { $($hex -join ','); 0x00 };"
Set-Content "dialog_jsx.h" $content