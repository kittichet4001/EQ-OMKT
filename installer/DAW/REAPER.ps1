$input = Read-Host "Install or uninstall REAPER (I/U): "
if ($input -eq "I" -or $input -eq "i") {
    winget install --name "REAPER" --source winget
}elseif ($input -eq "U" -or $input -eq "u") {
    winget uninstall --name "REAPER" --source winget
}else {
    Write-Host "Invalid option. Please choose 'I' to install or 'U' to uninstall."
}