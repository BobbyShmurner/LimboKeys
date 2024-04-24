& "$PSScriptRoot/build.ps1"

if ($LASTEXITCODE -ne 0) {
	exit $LASTEXITCODE
}

Clear-Host

& "$PSScriptRoot/build/Debug/LimboKeys.exe"