@echo off
set dst=..\mobile\client

xcopy /y /e *.exe %dst%

xcopy /y /e *.dll %dst%

xcopy /y /e DeleteRun.bat %dst%

xcopy /y /e main.lua %dst%\base\src\

pause