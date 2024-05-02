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

try {
	Push-Location "$BuildPath/bin"
	Invoke-Expression "& `".\LimboKeys.exe`""
}
finally {
	Pop-Location
}