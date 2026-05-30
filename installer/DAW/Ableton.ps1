$input = Read-Host "Install or uninstall Ableton Live 10 Suite (I/U): "
if ($input -eq "I" -or $input -eq "i") {
    winget install --name "Ableton Live 10 Suite" --source winget
}elseif ($input -eq "U" -or $input -eq "u") {
    winget uninstall --name "Ableton Live 10 Suite" --source winget
}else {
    Write-Host "Invalid option. Please choose 'I' to install or 'U' to uninstall."
}