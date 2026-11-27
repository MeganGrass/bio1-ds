@echo off
rem setup.bat - prepares the PS1 build, once: builds the GCC 2.6.3 compiler (tools\gcc-2.6.3-psx\cpp.exe and cc1.exe) and the custom
rem PSY-Q libraries (tools\custom-sdk).
rem Run this before compiling for the first time, and again whenever tools\sdkroot.ini or Bio1\lib changes.
rem "make" alone never rebuilds these; it only reads what setup.bat put in tools. The compiler step is skipped when cpp.exe and cc1.exe are
rem already there (tools\build_gcc.bat /force builds them again); it needs an internet connection, git and MSYS2, see tools\build_gcc.bat.
rem
rem Before running this, edit tools\sdkroot.ini to point at your own PSY-Q SDK installs.
cd /d "%~dp0"
call tools\build_gcc.bat
if errorlevel 1 (
    echo.
    echo SETUP FAILED - the GCC 2.6.3 compiler could not be built, see above for details.
    exit /b 1
)
make sdklib
if errorlevel 1 (
    echo.
    echo SETUP FAILED - see above for details.
    exit /b 1
)
echo.
echo built the GCC 2.6.3 compiler (tools\gcc-2.6.3-psx) and the custom PSY-Q libraries (tools\custom-sdk)
