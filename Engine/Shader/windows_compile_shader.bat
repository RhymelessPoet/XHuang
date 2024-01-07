@echo off
setlocal enabledelayedexpansion

set compiler=%Vulkan_SDK%\Bin\glslc.exe
set target_dir=Generated
set vert_file_type=vert
set frag_file_type=frag

echo -- Start Compile Vertex Shader
call :Compile_Shader %vert_file_type%

echo -- Start Compile Fragment Shader
call :Compile_Shader %frag_file_type%

:Compile_Shader
for %%F in (GLSL\*.%~1) do (
    set filename=%%~nxF
    rem echo !filename!
    echo -- Compile %%F to %target_dir%\!filename!.spv
    %compiler% %%F -o %target_dir%\!filename!.spv
)
goto:eof