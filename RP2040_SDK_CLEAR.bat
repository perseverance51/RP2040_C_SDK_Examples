@echo off & title ����ɾ����ǰĿ¼��ָ����ʽ֮���ļ���
cd /d "%~dp0"
::����Ҫ�ų����ļ���ʽ�������ʽ֮��ʹ��Ӣ�Ķ��Ÿ���
set "match=.cmake"
set "match2=CMakeFiles"
set "match3=generated"
set "match4=pico-sdk"

::ɾ���ļ�
del cmake_install.cmake /s
del compile_commands.json /s           
del CMakeCache.txt /s
del Makefile /s 
del *.dis/s 
::Ҫ�������ļ�
::del  *.bin /s
::del  *.elf /s
::del  *.map /s
::del  *.hex /s
::del  *.uf2 /s

cd /d "build"
:: ������ǰĿ¼������Ŀ¼�е������ļ���
for /f  "tokens=*" %%i in ('dir /b /ad') do (
    if  "%%i"=="%match%" (
         echo ����ɾ���ļ���: "%%i"
	rd /s /q "%%i"
    ) else if  "%%i"=="%match2%" (
         echo ����ɾ���ļ���: "%%i"
	rd /s /q "%%i"
    ) else if  "%%i"=="%match3%" (
         echo ����ɾ���ļ���: "%%i"
	rd /s /q "%%i"
    ) else if  "%%i"=="%match4%" (
         echo ����ɾ���ļ���: "%%i"
	rd /s /q "%%i"
    )else (
        echo ɨ�赽�ļ���: "%%i"       
    )
)

::ɾ���ļ���
::rd /s /q "build\.cmake"
::rd /s /q "build\CMakeFiles"
::rd /s /q "build\generated"
::rd /s /q "build\pico-sdk"


::pause
