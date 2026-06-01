param(
    [string]$Version = "1.0.0"
)

$ErrorActionPreference = "Stop"

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildDir = Join-Path $projectDir "build"
$packageDir = Join-Path $projectDir "package"
$distDir = Join-Path $projectDir "dist"
$qtDir = "D:\Qt\6.10.3\mingw_64"
$toolDir = "D:\Qt\Tools\mingw1310_64\bin"
$ninjaDir = "D:\Qt\Tools\Ninja"
$nsis = "C:\Program Files (x86)\NSIS\makensis.exe"

$env:Path = "$toolDir;$ninjaDir;$qtDir\bin;$env:Path"

cmake -S $projectDir -B $buildDir -G Ninja `
    "-DCMAKE_PREFIX_PATH=$qtDir" `
    "-DCMAKE_CXX_COMPILER=$toolDir\g++.exe" `
    "-DCMAKE_C_COMPILER=$toolDir\gcc.exe"
cmake --build $buildDir

$resolvedPackageDir = [System.IO.Path]::GetFullPath($packageDir)
$resolvedProjectDir = [System.IO.Path]::GetFullPath($projectDir)
if (-not $resolvedPackageDir.StartsWith($resolvedProjectDir, [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "Package directory must stay inside the project."
}

if (Test-Path $packageDir) {
    Remove-Item -LiteralPath $packageDir -Recurse -Force
}
New-Item -ItemType Directory -Path $packageDir | Out-Null
New-Item -ItemType Directory -Path $distDir -Force | Out-Null

Copy-Item -LiteralPath (Join-Path $buildDir "todo_app.exe") -Destination $packageDir
& "$qtDir\bin\windeployqt.exe" --release --no-translations --dir $packageDir (Join-Path $packageDir "todo_app.exe")

if (-not (Test-Path $nsis)) {
    throw "NSIS was not found at $nsis"
}

Push-Location $projectDir
try {
    & $nsis "/DAPP_VERSION=$Version" "installer.nsi"
} finally {
    Pop-Location
}

Write-Host ""
Write-Host "Installer created in: $distDir"
