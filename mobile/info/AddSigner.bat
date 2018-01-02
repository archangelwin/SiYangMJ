@echo off
echo ÃÜÂë¶ÌÓï=jiuhuogame

set h=%time:~0,2%
set h=%h: =0%
set folder=%date:~0,4%-%date:~5,2%-%date:~8,2%-%h%%time:~3,2%

jarsigner.exe -verbose -keystore michael.keystore -signedjar ..\client_publish\jiuhuogame.apk ..\client_publish\%folder%\LuaMBClient_LY.apk jiuhuogame

pause