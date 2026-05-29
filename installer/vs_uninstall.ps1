# บังคับเช็คและขอสิทธิ์ Admin ตั้งแต่บรรทัดแรก
if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Start-Process powershell -ArgumentList "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    Exit
}
winget uninstall --name "Visual Studio Community 2026" --source winget --silent