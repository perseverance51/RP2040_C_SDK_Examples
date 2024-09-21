@echo off & title 批量删除当前目录下指定格式之外的文件和文件夹
::批处理所在目录
cd /d "%~dp0"
::删除文件
del cmake_install.cmake /s
del compile_commands.json /s           
del CMakeCache.txt /s
del Makefile /s 
del *.dis /s
::要保留的文件
title  *.bin /s
title  *.elf /s
title  *.map /s
title  *.hex /s
title  *.uf2 /s

::删除文件夹
rd /s /q "build\.cmake"
rd /s /q "build\CMakeFiles"
rd /s /q "build\generated"
rd /s /q "build\pico-sdk"

exit