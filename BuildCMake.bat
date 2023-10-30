@echo off
echo Deleting directory "build"...
if exist build (
    rmdir /s /q build
    echo Directory deleted "build".
)

echo Running the command "cmake -S . -B build"...
cmake -S . -B build

if %errorlevel% equ 0 (
    echo Command executed correctly.
) else (
    echo Error while executing the command.
)

pause
