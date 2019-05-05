cd /d %~dp0

SET currentSourceDir=%~dp0

@ECHO off
SETLOCAL enabledelayedexpansion
cls
COLOR 1f

ECHO.
ECHO.
ECHO   ##############################################################
ECHO   #               ��ӭʹ�� SOUI ����������                   #
ECHO   #                                ������� 2017.04.01         #
ECHO   ##############################################################
ECHO.
ECHO.

SET cfg=
SET specs=
SET target=x86
SET targetx86andx64=0
SET selected=
SET mt=1
SET unicode=1
SET wchar=1
SET supportxp=0
SET vsvarbat=

rem ѡ�����汾
SET /p selected=1.ѡ�����汾[1=x86;2=x64;3=x86+x64]:
if %selected%==1 (
	SET target=x86
) else if %selected%==2 (
	SET target=x64
	SET cfg=!cfg! x64
) else if %selected%==3 (
	SET target=x86
	SET targetx86andx64=1
) else (
	goto error
)

rem ѡ�񿪷�����
SET /p selected=2.ѡ�񿪷�����[1=2008;2=2010;3=2012;4=2013;5=2015;6=2017;7=2005]:

if %selected%==1 (
	SET specs=win32-msvc2008
	SET vsvarbat="!VS90COMNTOOLS!..\..\VC\vcvarsall.bat"
	call !vsvarbat! %target%
	rem call "%VS90COMNTOOLS%..\..\VC\vcvarsall.bat" %target%
	goto built
) else if %selected%==2 (
	SET specs=win32-msvc2010
	SET vsvarbat="%VS100COMNTOOLS%..\..\VC\vcvarsall.bat"
	call !vsvarbat! %target%
	rem call "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat" %target%
	goto built
) else if %selected%==3 (
	SET specs=win32-msvc2012
	SET vsvarbat="%VS110COMNTOOLS%..\..\VC\vcvarsall.bat"
	call !vsvarbat! %target%
	rem call "%VS110COMNTOOLS%..\..\VC\vcvarsall.bat" %target%
	goto built
) else if %selected%==4 (
	SET specs=win32-msvc2013
	SET vsvarbat="%VS120COMNTOOLS%..\..\VC\vcvarsall.bat"
	call !vsvarbat! %target%
	rem call "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" %target%
	goto toolsetxp
) else if %selected%==5 (
	SET specs=win32-msvc2015
	SET vsvarbat="%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
	call !vsvarbat! %target%
	rem call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" %target%
	goto toolsetxp
)else if %selected%==6 (
	SET specs=win32-msvc2017
	for /f "skip=2 delims=: tokens=1,*" %%i in ('reg query "HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VS7" /v "15.0" /reg:32') do (
	    set str=%%i
	   set var=%%j
	   set "var=!var:"=!"
	   if not "!var:~-1!"=="=" set value=!str:~-1!:!var!
	 )
	 SET value=!value!\VC\Auxiliary\Build\vcvarsall.bat
	 rem ECHO Vs2017 path is:!value!
		SET vsvarbat="!value!"
		call !vsvarbat! %target%
		rem call "!value!" %target%
		goto toolsetxp
)else if %selected%==7 (
	SET specs=win32-msvc2005
	SET vsvarbat="%VS80COMNTOOLS%..\..\VC\vcvarsall.bat"
	call !vsvarbat! %target%
	rem call "%VS80COMNTOOLS%..\..\VC\vcvarsall.bat" %target%
	goto built
) else (
	goto error
)
:toolsetxp
rem XP֧��
SET SOUI_WITH_XP_TOOLSET=ON
SET /p selected=2.�Ƿ�֧��xp[1=֧��;2=��֧��]:
if %selected%==2 (
  SET SOUI_WITH_XP_TOOLSET=OFF
)
:built
rem @echo !vsvarbat! %target%
rem ѡ���������
SET SOUI_BUILD_CORE_STATIC=OFF
SET SOUI_BUILD_COMPONENTS_STATIC=OFF
SET /p selected=3.ѡ��SOUI����ģʽ[1=ȫģ��DLL;2=ȫģ��LIB;3=�ں�LIB,���DLL(����ʹ��LUA�ű�ģ��)]:
if %selected%==1 (
  REM DO NOTHING
) else if %selected%==2 (
	SET SOUI_BUILD_CORE_STATIC=ON
  SET SOUI_BUILD_COMPONENTS_STATIC=ON
) else if %selected%==3 (
	SET SOUI_BUILD_CORE_STATIC=ON
  SET SOUI_BUILD_COMPONENTS_STATIC=OFF
) else (
	goto error
)

rem ѡ���ַ���
SET SOUI_WITH_UNICODE=ON
SET /p selected=4.ѡ���ַ���[1=UNICODE;2=MBCS]:
if %selected%==1 (
	rem do nothing
) else if %selected%==2 (
	SET SOUI_WITH_UNICODE=OFF
) else (
	goto error
)

rem ѡ��WCHAR֧��
SET /p selected=5.��WCHAR��Ϊ�ڽ�����[1=��;2=��]:
if %selected%==1 (
	SET SOUI_WITH_BUILTIN_WCHAR=ON
) else if %selected%==2 (
	SET SOUI_WITH_BUILTIN_WCHAR=OFF
) else (
	goto error
)

rem CRT
SET /p selected=6.ѡ��CRT����ģʽ[1=��̬����(MT);2=��̬����(MD)]:
if %selected%==1 (
	SET SOUI_WITH_STATIC_RUNTIME=ON
) else if %selected%==2 (
	SET SOUI_WITH_STATIC_RUNTIME=OFF
	rem do nothing
) else (
	goto error
)

rem Ϊrelease�汾���ɵ�����Ϣ
SET buildWithDebInfo=0
SET /p selected=7.�Ƿ�Ϊrelease�汾���ɵ�����Ϣ[1=����;2=������]:
if %selected%==1 (
	SET buildWithDebInfo=1
) else if %selected%==2 (
	rem do nothing
) else (
	goto error
)
cd /d %~dp0
rem @echo %cfg%
rem ������ĿĬ������
rem �����������

set buildDir=%currentSourceDir%\build\%specs%_%target%
mkdir %buildDir%
cd %buildDir%

cmake %currentSourceDir% -DSOUI_OUTPUT_DIR=%currentSourceDir%\bin -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=%currentSourceDir%\bin

SET /p selected=open[o], compile[c] "soui.sln" or quit(q) [o,c or q]?
if "%selected%" == "o" (
  %buildDir%\SOUI.sln
) else if "%selected%" == "c" (
  call devenv %buildDir%\SOUI.sln /build Debug
  if %buildWithDebInfo%==1 (
    call devenv %buildDir%\SOUI.sln /build RelWithDebInfo
  ) else (
    call devenv %buildDir%\SOUI.sln /build Release
  )
) else (
	goto final
)

goto final

:error
	ECHO ѡ�����������ѡ��
:final

rem pause
