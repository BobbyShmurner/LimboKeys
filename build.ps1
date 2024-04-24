New-Item -ItemType Directory -Force -Path "$PSScriptRoot/build" | Out-Null

Push-Location "$PSScriptRoot/build"
& cmake ..
& cmake --build .
Pop-Location