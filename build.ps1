param (
	[switch] $Clean,
	[switch] $Release,
	[ValidateSet($null, $true, $false)]
	[object] $TracyOverride
)

$BuildPath = "$PSScriptRoot/build"
if ($Release) {
	$BuildPath += "/Release"
	$BuildMode = "Release"
} else {
	$BuildPath += "/Debug"
	$BuildMode = "Debug"
}

if ($Clean) {
	Remove-Item -Recurse -Force -Path $BuildPath -ErrorAction SilentlyContinue | Out-Null
}

New-Item -ItemType Directory -Force -Path $BuildPath | Out-Null

$Tracy = $Release
if ($TracyOverride -ne $null) {
	$Tracy = $TracyOverride
}

$cmakeArgs = "-DCMAKE_BUILD_TYPE=$BuildMode "

if ($Tracy) {
	$cmakeArgs += "-DTRACY_ENABLE=ON -DTRACY_ON_DEMAND=ON"
} else {
	$cmakeArgs += "-DTRACY_ENABLE=OFF -DTRACY_ON_DEMAND=OFF"
}

Push-Location $BuildPath

try {
	Write-Output "& cmake ../.. -G Ninja $cmakeArgs"
	Invoke-Expression "& cmake ../.. -G Ninja $cmakeArgs"
	& cmake --build .
} finally {
	Pop-Location
}