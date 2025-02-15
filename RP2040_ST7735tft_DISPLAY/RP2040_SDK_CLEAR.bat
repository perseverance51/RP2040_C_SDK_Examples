@echo off & title 批量删除当前目录下指定格式之外文件夹
cd /d "%~dp0"
::设置要排除的文件格式，多个格式之间使用英文逗号隔开
set "match=.cmake"
set "match2=CMakeFiles"
set "match3=generated"
set "match4=pico-sdk"

::删除文件
del cmake_install.cmake /s
del compile_commands.json /s           
del CMakeCache.txt /s
del Makefile /s 
del *.dis/s 
::要保留的文件
::del  *.bin /s
::del  *.elf /s
::del  *.map /s
::del  *.hex /s
::del  *.uf2 /s

cd /d "build"
:: 遍历当前目录及其子目录中的所有文件夹
for /f  "tokens=*" %%i in ('dir /b /ad') do (
    if  "%%i"=="%match%" (
         echo 正在删除文件夹: "%%i"
	rd /s /q "%%i"
    ) else if  "%%i"=="%match2%" (
         echo 正在删除文件夹: "%%i"
	rd /s /q "%%i"
    ) else if  "%%i"=="%match3%" (
         echo 正在删除文件夹: "%%i"
	rd /s /q "%%i"
    ) else if  "%%i"=="%match4%" (
         echo 正在删除文件夹: "%%i"
	rd /s /q "%%i"
    )else (
        echo 扫描到文件夹: "%%i"       
    )
)

::删除文件夹
::rd /s /q "build\.cmake"
::rd /s /q "build\CMakeFiles"
::rd /s /q "build\generated"
::rd /s /q "build\pico-sdk"


::pause
