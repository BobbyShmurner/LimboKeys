param (
	[Switch] $Release
)

if ($Release) {
	$args += " -Release"
}

Invoke-Expression "& `"$PSScriptRoot/build.ps1`" $args"

if ($LASTEXITCODE -ne 0) {
	exit $LASTEXITCODE
}

Clear-Host

$BuildPath = "$PSScriptRoot/build"
if ($Release) {
	$BuildPath += "/Release"
} else {
	$BuildPath += "/Debug"
}

Invoke-Expression "& `"$BuildPath/LimboKeys.exe`""