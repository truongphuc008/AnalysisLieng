Add-Type -AssemblyName System.IO.Compression.FileSystem

# Copy files first to avoid lock issues
$tempDir = 'D:\Desktop\C++\MINI-PROJECT LIENG ANTI\temp_extract'
if (!(Test-Path $tempDir)) { New-Item -ItemType Directory -Path $tempDir | Out-Null }

Copy-Item 'D:\Desktop\C++\MINI-PROJECT LIENG ANTI\Lieng.docx' "$tempDir\Lieng.docx" -Force
Copy-Item 'D:\Desktop\C++\MINI-PROJECT LIENG ANTI\Danhsach.xlsx' "$tempDir\Danhsach.xlsx" -Force

# Read Lieng.docx
$docxPath = "$tempDir\Lieng.docx"
$zip = [System.IO.Compression.ZipFile]::OpenRead($docxPath)
$entry = $zip.Entries | Where-Object { $_.FullName -eq 'word/document.xml' }
$stream = $entry.Open()
$reader = New-Object System.IO.StreamReader($stream, [System.Text.Encoding]::UTF8)
$content = $reader.ReadToEnd()
$reader.Close()
$zip.Dispose()
$text = $content -replace '<[^>]+>', ' '
$text = [System.Text.RegularExpressions.Regex]::Replace($text, '\s+', ' ')
[System.IO.File]::WriteAllText('D:\Desktop\C++\MINI-PROJECT LIENG ANTI\lieng_text.txt', $text, [System.Text.Encoding]::UTF8)
Write-Output "Lieng.docx done"

# Read Danhsach.xlsx - shared strings
$xlsxPath = "$tempDir\Danhsach.xlsx"
$zip2 = [System.IO.Compression.ZipFile]::OpenRead($xlsxPath)

$ssEntry = $zip2.Entries | Where-Object { $_.FullName -eq 'xl/sharedStrings.xml' }
if ($ssEntry) {
    $ssStream = $ssEntry.Open()
    $ssReader = New-Object System.IO.StreamReader($ssStream, [System.Text.Encoding]::UTF8)
    $ssContent = $ssReader.ReadToEnd()
    $ssReader.Close()
    [System.IO.File]::WriteAllText('D:\Desktop\C++\MINI-PROJECT LIENG ANTI\shared_strings.txt', $ssContent, [System.Text.Encoding]::UTF8)
}

$sheetEntry = $zip2.Entries | Where-Object { $_.FullName -eq 'xl/worksheets/sheet1.xml' }
if ($sheetEntry) {
    $shStream = $sheetEntry.Open()
    $shReader = New-Object System.IO.StreamReader($shStream, [System.Text.Encoding]::UTF8)
    $shContent = $shReader.ReadToEnd()
    $shReader.Close()
    [System.IO.File]::WriteAllText('D:\Desktop\C++\MINI-PROJECT LIENG ANTI\sheet1.txt', $shContent, [System.Text.Encoding]::UTF8)
}
$zip2.Dispose()
Write-Output "Danhsach.xlsx done"
