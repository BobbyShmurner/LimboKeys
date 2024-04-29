param (
	[switch] $Clean
)

if ($Clean) {
	Remove-Item -Recurse -Force -Path "$PSScriptRoot/build" -ErrorAction SilentlyContinue | Out-Null
}

New-Item -ItemType Directory -Force -Path "$PSScriptRoot/build" | Out-Null

Push-Location "$PSScriptRoot/build"
& cmake ..
& cmake --build .
Pop-Location