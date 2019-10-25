@echo off
echo.
echo Fault Tolerant Petri Processor ver. 1.0 (30.04.2002)
if %1.==. goto nocom
echo !A_SRC ..\ FTPP %1 ..\AutoBuild.h !exclude.txt D:\Sergy\BACKUP
echo.
call !A_SRC ..\ FTPP %1 ..\AutoBuild.h !exclude.txt D:\Sergy\BACKUP %2
rem if %2.==.     call !A_SRC ..\ FTPP %1 ..\AutoBuild.h !exclude.txt D:\Sergy\BACKUP
rem if not %2.==. call !A_SRC ..\ FTPP %1 ..\AutoBuild.h !exclude.txt D:\Sergy\BACKUP %2
goto end
:nocom
echo Type Build number !!! Ex: !makeres 10
:end
