@echo off
rem ----------------------------------------------------------------------
rem 	MD: 26.05.01
rem 	LE: 26.05.01
rem ----------------------------------------------------------------------
echo.
echo Archive Engine for Sources ver. 1.0 (!A_SRC.BAT)
echo Troy Computing, Inc.
echo Copyright (c) 1994-2001 Sergy Kolevatov 
echo.
rem ----------------------------------------------------------------------
rem %1 - path
rem %2 - arcmask
rem %3 - build
rem %4 - service file
rem %5 - xfile
rem %6 - backup dir
rem %7 - pwd
rem ----------------------------------------------------------------------
if %1.==. goto nocom
if %2.==. goto nocom
if %3.==. goto nocom
if %4.==. goto nocom
if %5.==. goto nocom
if %6.==. goto nocom
rem ----------------------------------------------------------------------
echo Summary:
echo Path         = %1*.*
echo ArcMask      = %2
echo BuildNum     = %3
echo Service File = %4
echo Exclude File = %5
echo BackUp Dir   = %6
if not %7.==. echo Password     = [enabled]
echo.
rem ----------------------------------------------------------------------
if exist %2*.rar copy %2*.rar %6
if exist %2*.rar del %2*.rar
rem ----------------------------------------------------------------------
rem Generates autobuild number
rem ----------------------------------------------------------------------
echo // --- >>%4
echo // Auto Generated file with build number. (c) Sergy/08.04.01 >>%4
echo // --- >>%4
echo #ifdef AUTO_BUILDNUMBER >>%4
echo #undef AUTO_BUILDNUMBER >>%4
echo #endif >>%4
echo #define AUTO_BUILDNUMBER	(%3) >>%4
rem ----------------------------------------------------------------------
rem No pwd
rem ----------------------------------------------------------------------
if %7.==.     call rar a %2%3 -x@%5 %1*.* -m5 -md1024 -s 
rem ----------------------------------------------------------------------
rem Pwd exists
rem ----------------------------------------------------------------------
if not %7.==. call rar a %2%3 -x@%5 %1*.c* %1*.h -p%7 -m5 -md1024
if not %7.==. call rar a %2%3 -x@%5 %1*.* -x*.c* -x*.h -m5 -md1024
rem ----------------------------------------------------------------------
copy %2*.rar %6
goto end
rem ----------------------------------------------------------------------
:nocom
echo Usage: !A_SRC (path) (arcmask) (buildnum) (autosrv) (xfile) (backupdir) 
echo               [(pwd)]
echo.
echo        path      - path to sources
echo        arcmask   - filemask of new archive name
echo        buildnum  - digit is the number of backup copy of build number
echo        autosrv   - name of header file with build information
echo        xfile     - file with list of excluded file extensions
echo        backupdir - a folder to save previous copies 
echo        pwd       - pwd for src (option)
echo.
echo Example: !A_SRC ..\* MyPrg 10 ..\MyBuild.h exclude.txt C:\MyProgs\
echo.
echo This command adds files form '..\' to archive file 'MyPrg10', 
echo excludes files from 'exclude.txt'  and saves archive into backup 
echo directory 'C:\MyProgs\' and generates buildinfo in file '..\MyBuild.h'
echo.
goto end
:end
