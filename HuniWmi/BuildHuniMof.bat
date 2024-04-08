del /f /Q Hunimof.hex
mofcomp -B:Hunimof.bmf Hunimof.mof
wmimofck -xHunimof.hex Hunimof.bmf
del /f /Q Hunimof.bmf
pause