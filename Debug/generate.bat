@echo off
rem 04.06.2002
if %1.==. goto nocom
if %2.==. goto nocom
FTPP %1 %2 /m /t /pr:99.999999 /p:0.01 /tau0:30 /tau1:45
goto end
:nocom
echo: Error. Use GENERATE infile.mpn mapping.map
:end