Invoke-Expression "& `"$PSScriptRoot/build.ps1`" $args"

if ($LASTEXITCODE -ne 0) {
	exit $LASTEXITCODE
}

Clear-Host

& "$PSScriptRoot/build/Debug/LimboKeys.exe"