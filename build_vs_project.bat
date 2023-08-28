@echo off
set target_dir=%CD%\Build\WinVS
set target_vs_version=Visual Studio 17 2022
echo -- Build target directory is %target_dir%
if exist %target_dir% (
    echo -- %target_dir% exists
    echo -- Clearing %target_dir% ...
    del %target_dir%\*.* /Q /F /A
) else (
    echo -- %target_dir% not exists, make %target_dir% ...
    md %target_dir%
)
echo 
cmake --version
echo 
echo ************************** Starting build VS project for %target_vs_version% ************************
echo 
cmake -B %target_dir% -G "%target_vs_version%"
pause