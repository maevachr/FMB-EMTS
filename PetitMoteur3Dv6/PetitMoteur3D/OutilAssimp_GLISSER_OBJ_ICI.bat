@echo off
set /p ombfile="Enter the name of omb file: "
echo %ombfile%.omb
python %~dp0OutilAssimp.py %~dp0 %* %ombfile%.omb
pause