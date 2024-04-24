New-Item -ItemType Directory -Force -Path "$PSScriptRoot/bin" | Out-Null
g++ -o "$PSScriptRoot/bin/LimboKeys.exe" "$PSScriptRoot/src/main.cpp"