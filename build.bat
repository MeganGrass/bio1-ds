@echo off
rem build.bat [version] [build|rebuild|clean] [extra args for the version's build script]
rem
rem   version   pc-j     Biohazard.exe v1.01 (PC Japan), MSVC 4.2 -> OBJ\PC\J\Biohazard.exe   (default)
rem             psx-usa  Director's Cut Dual Shock USA, PsyQ -> OBJ\PSX\USA\SLUS_007.47 + PROG2\*.EXE (makefile + PSYLINK + tools\*.exe; run setup.bat once)
rem   action    build (default) | rebuild | clean. pc-j always rebuilds from scratch, so build == rebuild.
rem
rem OBJ\ holds compiled binaries only; logs and staged tools/libs go to tmp\build\<version>.
rem Called by the NMake commands of bio1-ds.vcxproj (one configuration per version), also usable from a prompt.
cd /d "%~dp0"
set VER=pc-j
set ACT=build
set EXTRA=

if /i "%~1"=="pc-j"    (set VER=pc-j&   shift)
if /i "%~1"=="psx-usa" (set VER=psx-usa& shift)
if /i "%~1"=="build"   (set ACT=build&   shift)
if /i "%~1"=="rebuild" (set ACT=rebuild& shift)
if /i "%~1"=="clean"   (set ACT=clean&   shift)
:collect
if "%~1"=="" goto run
set EXTRA=%EXTRA% %1
shift
goto collect

:run
if /i "%VER%"=="pc-j" goto pcj
if /i "%VER%"=="psx-usa" goto psxusa
echo unknown version %VER%
exit /b 2

:pcj
if /i "%ACT%"=="clean" (
    if exist OBJ\PC\J rmdir /s /q OBJ\PC\J
    if exist tmp\build\pc-j rmdir /s /q tmp\build\pc-j
    echo cleaned OBJ\PC\J and tmp\build\pc-j
    exit /b 0
)
py tmp\tools\re1pc_link.py --check-hygiene%EXTRA%
if errorlevel 1 (
    echo.
    echo BUILD FAILED - see above for details.
    exit /b 1
)
echo.
echo built OBJ\PC\J\Biohazard.exe
exit /b 0

:psxusa
rem the PS1 build is the makefile (GNU make), as in bio2-ds: tools\cc263.exe, PSYLINK, tools\cpe2exe.exe. Run setup.bat once first.
if /i "%ACT%"=="clean" (
    make clean
    exit /b %errorlevel%
)
if /i "%ACT%"=="rebuild" (
    make rebuild%EXTRA%
) else (
    make -j 8%EXTRA%
)
if errorlevel 1 (
    echo.
    echo BUILD FAILED - see above for details.
    exit /b 1
)
echo.
echo built OBJ\PSX\USA\SLUS_007.47 and OBJ\PSX\USA\PROG2\*.EXE
exit /b 0
