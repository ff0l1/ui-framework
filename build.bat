@echo off
setlocal EnableExtensions
cd /d "%~dp0"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "CMAKE="
where cmake >nul 2>&1 && set "CMAKE=cmake"
if not defined CMAKE if exist "%ProgramFiles%\CMake\bin\cmake.exe" set "CMAKE=%ProgramFiles%\CMake\bin\cmake.exe"
if not defined CMAKE if exist "%ProgramFiles%\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" set "CMAKE=%ProgramFiles%\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if not defined CMAKE if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" set "CMAKE=%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

if exist "%~dp0CMakeLists.txt" goto :cmake
goto :sln

:cmake
if not defined CMAKE (
    echo [-] cmake was not found. Install CMake or Visual Studio with C++.
    exit /b 1
)
echo [+] cmake
"%CMAKE%" -S "%~dp0." -B "%~dp0build" -A x64
if errorlevel 1 exit /b 1
"%CMAKE%" --build "%~dp0build" --config Release
exit /b %errorlevel%

:sln
set "SLN="
for %%S in ("%~dp0*.sln") do set "SLN=%%~fS"
if not defined SLN (
    echo [-] no CMakeLists.txt or solution
    exit /b 1
)
set "MSBUILD="
if exist "%VSWHERE%" (
    for /f "usebackq delims=" %%M in (`"%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do set "MSBUILD=%%M"
)
if not defined MSBUILD (
    echo [-] MSBuild was not found. Install Visual Studio with C++.
    exit /b 1
)
echo [+] msbuild
"%MSBUILD%" "%SLN%" /m /p:Configuration=Release /p:Platform=x64
exit /b %errorlevel%
