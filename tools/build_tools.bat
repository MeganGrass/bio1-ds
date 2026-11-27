@echo off
rem tools\build_tools.bat - rebuilds the native build tools in this folder from their .c sources.
rem
rem The makefile does not build anything under tools\ (the tools are checked in as .exe next to their .c);
rem this is the one command that rebuilds them (Visual Studio C++ build tools, any recent version):
rem
rem     cc263.exe        compiles one game source: cpp -> GCC 2.6.3 cc1 -> CRLF -> ASPSX (PsyQ object)
rem     cpe2exe.exe      PSYLINK CPE (or an overlay's group image) -> PS-X EXE
rem     lf2crlf.exe      LF -> CRLF copy of a text file (ASPSX needs CRLF)
rem     gen_linkscript.exe  resolves the @PLACEHOLDERS@ of the link script (makefile.link)
rem     ovlprefix.exe    prefixes the global names of one overlay's objects, so that one PSYLINK run links all 13 programs
rem     sdklibbuild.exe  composes the custom LIBPAD / LIBGS libraries (tools\custom-sdk)
rem     retailcheck.exe  checks the 13 linked executables against the retail SHA-256 values
setlocal
cd /d "%~dp0"
set TMPOBJ=%~dp0..\tmp\build\tools
if not exist "%TMPOBJ%" mkdir "%TMPOBJ%"
set VSROOT=
"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -prerelease -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath >"%TMPOBJ%\vsroot.txt"
if exist "%TMPOBJ%\vsroot.txt" set /p VSROOT=<"%TMPOBJ%\vsroot.txt"
if not defined VSROOT (
    echo Visual Studio C++ build tools not found
    exit /b 1
)
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
set FAIL=0
for %%t in (cc263 cpe2exe lf2crlf gen_linkscript sdklibbuild retailcheck ovlprefix) do (
    if exist %%t.c (
        cl /nologo /O2 /W3 /MT /D_CRT_SECURE_NO_WARNINGS /Fo"%TMPOBJ%\%%t.obj" %%t.c /Fe%%t.exe /link /INCREMENTAL:NO >"%TMPOBJ%\%%t.log" 2>&1
        if errorlevel 1 (
            echo FAILED %%t.c - see %TMPOBJ%\%%t.log
            type "%TMPOBJ%\%%t.log"
            set FAIL=1
        ) else (
            echo built %%t.exe
        )
    )
)
exit /b %FAIL%
