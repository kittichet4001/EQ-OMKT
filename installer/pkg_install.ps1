# บังคับเช็คและขอสิทธิ์ Admin ตั้งแต่บรรทัดแรก
if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Start-Process powershell -ArgumentList "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    Exit
}

# รายการโปรแกรมที่ต้องการติดตั้ง (ใส่ args ยืนยันอัตโนมัติไว้ท้ายสุด)
$apps = @("CMake", "LLVM", "ninja")
foreach ($app in $apps) {
    if($app -eq "CMake") {
        winget install --name $app --source winget --accept-package-agreements --accept-source-agreements
    } else {
         winget install --name $app --source winget --silent --accept-package-agreements --accept-source-agreements
    }
}