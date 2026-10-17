@echo off
rem Dishonored 2 co-op: installs into your Steam Dishonored 2 folder (or the folder given as the first argument).
rem Runs Install-D2Coop.ps1 with Windows PowerShell; the execution policy applies to this run only.
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Install-D2Coop.ps1" %*
set result=%errorlevel%
echo.
if not "%D2COOP_NO_PAUSE%"=="1" pause
exit /b %result%
