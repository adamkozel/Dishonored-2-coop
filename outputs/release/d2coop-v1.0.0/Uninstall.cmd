@echo off
rem Dishonored 2 co-op: removes the mod and restores the original files (or uses the folder given as the first argument).
rem A module path inherited from PowerShell 7 breaks Windows PowerShell; use its defaults.
set "PSModulePath="
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Uninstall-D2Coop.ps1" %*
set result=%errorlevel%
echo.
if not "%D2COOP_NO_PAUSE%"=="1" pause
exit /b %result%
