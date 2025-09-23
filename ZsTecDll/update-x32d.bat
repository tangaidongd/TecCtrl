set entry_zstecdll=%cd%

cd %~dp0
if exist "export" cd "export"

copy *.* ..\..\TestZsTec\module\libs\ZsTecDll
copy *.dll ..\..\TestZsTec\debug
copy *.lib ..\..\TestZsTec\debug

cd %entry_zsmotion%
