@echo off
rem tools\build_gcc.bat - builds cpp.exe and cc1.exe (GNU CC 2.6.3 for the PlayStation, target mips-sony-psx) into tools\gcc-2.6.3-psx.
rem
rem tools\cc263.exe needs these two programs; the repository does not need to carry them, this script makes them. It
rem   1. downloads GCC 2.6.3, the decompals/old-gcc PSX patches and a 32-bit MinGW-w64 host compiler (WinLibs, about 115 MB),
rem      every file checked against a pinned SHA-256,
rem   2. patches, configures and builds cpp (cccp) and cc1 with MSYS2's sh/make (GCC 2.6.3 only works as a 32-bit program),
rem   3. creates tools\gcc-2.6.3-psx and puts cpp.exe and cc1.exe there, then deletes the build folder.
rem
rem   build_gcc.bat           build, unless both programs are already there
rem   build_gcc.bat /force    build again and replace them
rem   build_gcc.bat /keep     keep the build folder afterwards (downloads, sources, host compiler)
rem
rem Needs: Windows 10 or later (curl.exe, tar.exe, certutil.exe), Git for Windows' git.exe on PATH (git apply does the patching), and an MSYS2
rem (sh, bash, make, sed, tar, touch) - the one of devkitPro is found by itself, or set MSYSBIN to the folder with bash.exe.
rem Environment overrides: MSYSBIN (MSYS2 usr\bin), BLD (build folder, default %TEMP%\gcc263-build; no spaces allowed, the GCC
rem makefiles cannot take them), OUT (destination folder, default tools\gcc-2.6.3-psx). Needs about 1.5 GB free while it runs.
setlocal EnableExtensions
set "BATCH=%~f0"
set "HERE=%~dp0"
set "FORCE="
set "KEEP="
:args
if /i "%~1"=="/force" (set "FORCE=1"& shift & goto args)
if /i "%~1"=="/keep" (set "KEEP=1"& shift & goto args)
if not "%~1"=="" (echo usage: build_gcc.bat [/force] [/keep]& exit /b 2)

if not defined OUT set "OUT=%HERE%gcc-2.6.3-psx"
if not defined BLD set "BLD=%TEMP%\gcc263-build"
if "%OUT:~-1%"=="\" set "OUT=%OUT:~0,-1%"
if "%BLD:~-1%"=="\" set "BLD=%BLD:~0,-1%"

if not defined FORCE if exist "%OUT%\cc1.exe" if exist "%OUT%\cpp.exe" (
    echo cpp.exe and cc1.exe are already in %OUT%; use /force to build them again.
    exit /b 0
)
if not "%BLD: =%"=="%BLD%" (echo the build folder must not contain spaces: %BLD%& echo set BLD to another folder, e.g. set BLD=C:\gcc263-build& exit /b 1)

rem ---- the tools this script needs
for %%T in (curl.exe tar.exe certutil.exe git.exe powershell.exe) do (
    where %%T >nul 2>&1 || (echo %%T not found on PATH& exit /b 1)
)
if not defined MSYSBIN for %%D in ("C:\devkitPro\msys2\usr\bin" "C:\msys64\usr\bin" "C:\msys32\usr\bin") do if exist "%%~D\bash.exe" if exist "%%~D\make.exe" if not defined MSYSBIN set "MSYSBIN=%%~D"
if not defined MSYSBIN (echo MSYS2 not found: set MSYSBIN to its usr\bin folder ^(needs bash.exe, make.exe, sed.exe, tar.exe^)& exit /b 1)
for %%F in (bash.exe make.exe sed.exe tar.exe touch.exe) do if not exist "%MSYSBIN%\%%F" (echo %%F not found in %MSYSBIN%& exit /b 1)
set "WINTAR=%SystemRoot%\System32\tar.exe"
if not exist "%WINTAR%" (echo %WINTAR% not found ^(needed to unpack the 7z host compiler^)& exit /b 1)

if not exist "%BLD%\dl" mkdir "%BLD%\dl"
if errorlevel 1 (echo cannot create %BLD%& exit /b 1)

rem ---- 1. downloads (cached in %BLD%\dl, each one checked against its SHA-256)
set "GH=https://raw.githubusercontent.com/decompals/old-gcc/cf70ec58bce58128732062bc9930045c82904cbb/patches"
set "WL=https://github.com/brechtsanders/winlibs_mingw/releases/download/16.2.0posix-23.1.3-14.0.0-ucrt-r3"
echo [1/4] downloading
call :get "https://mirrors.slackware.com/slackware/slackware-2.2.0/source/d/gcc/gcc-2.6.3.tar.gz" "%BLD%\dl\gcc-2.6.3.tar.gz" 66529b6ced4ccc580ec365809c1abed856d948283deff656801dec14f47d081a || goto fail
call :get "%GH%/obstack-2.7.2.h.patch" "%BLD%\dl\obstack-2.7.2.h.patch" c2e7e6327db8ed6d9044b4ec61bf28af1cf95338b0787d21ca4d2700beb9c553 || goto fail
call :get "%GH%/sdbout-2.6.3.c.patch" "%BLD%\dl\sdbout-2.6.3.c.patch" 53848dc995eee0cc37ed7426b7cd4b9a315dad696dd2bb88911342cc9e9049c4 || goto fail
call :get "%GH%/psx-2.5.7.patch" "%BLD%\dl\psx-2.5.7.patch" 10fc651f6a133c3fa767f2095fcaad8633cdc6f58e045257f5244af9afccda3d || goto fail
call :get "%WL%/winlibs-i686-posix-dwarf-gcc-16.2.0-mingw-w64ucrt-14.0.0-r3.7z" "%BLD%\dl\winlibs-i686.7z" e7579b167bce7f16358874133eac11dc369eb02f1735ec33695d4f5b0da77fda || goto fail

rem ---- 2. host compiler and the three small files at the end of this script (winhost.c, config.h, build.sh)
echo [2/4] unpacking the 32-bit host compiler
if exist "%BLD%\mingw32" rmdir /s /q "%BLD%\mingw32"
"%WINTAR%" -xf "%BLD%\dl\winlibs-i686.7z" -C "%BLD%" || goto fail
if not exist "%BLD%\mingw32\bin\gcc.exe" (echo the host compiler did not unpack& goto fail)
powershell -NoProfile -ExecutionPolicy Bypass -Command "$t=[IO.File]::ReadAllText($env:BATCH); foreach($n in 'winhost.c','config.h','build.sh'){ $m=[regex]::Match($t,'(?ms)^#BEGIN '+[regex]::Escape($n)+'\r?\n(.*?)\r?\n#END '+[regex]::Escape($n)); if(-not $m.Success){ throw ('missing '+$n) }; [IO.File]::WriteAllText((Join-Path $env:BLD $n),$m.Groups[1].Value.Replace(\"`r`n\",\"`n\")+\"`n\") }" || goto fail

rem ---- 3. unpack, patch, configure, build (all in build.sh, run by MSYS2's bash)
echo [3/4] building cpp and cc1 (a few minutes; log: %BLD%\build.log)
set "PATH=%MSYSBIN%;%PATH%"
bash.exe "%BLD%\build.sh" >"%BLD%\build.log" 2>&1
if errorlevel 1 (echo BUILD FAILED, last lines of %BLD%\build.log:& powershell -NoProfile -Command "Get-Content -Tail 25 $env:BLD\build.log"& exit /b 1)
if not exist "%BLD%\out\cc1.exe" goto fail
if not exist "%BLD%\out\cpp.exe" goto fail

rem ---- 4. tools\gcc-2.6.3-psx
echo [4/4] installing into %OUT%
if not exist "%OUT%" mkdir "%OUT%"
copy /y "%BLD%\out\cpp.exe" "%OUT%\cpp.exe" >nul || goto fail
copy /y "%BLD%\out\cc1.exe" "%OUT%\cc1.exe" >nul || goto fail
"%OUT%\cc1.exe" -version </nul 2>&1 | findstr /c:"2.6.3" >nul
if errorlevel 1 (echo the new cc1.exe does not report GNU C 2.6.3& exit /b 1)
if not defined KEEP rmdir /s /q "%BLD%"
echo done: %OUT%\cpp.exe and %OUT%\cc1.exe
exit /b 0

:fail
echo FAILED - see the messages above (build folder: %BLD%)
exit /b 1

rem :get url file sha256 - download unless cached, then check the hash
:get
if not exist "%~2" curl.exe -L --fail -sS -o "%~2" "%~1"
if not exist "%~2" (echo cannot download %~1& exit /b 1)
set "HASH="
for /f "skip=1 delims=" %%H in ('certutil -hashfile "%~2" SHA256') do if not defined HASH set "HASH=%%H"
set "HASH=%HASH: =%"
if /i "%HASH%"=="%~3" exit /b 0
echo SHA-256 mismatch for %~nx2 ^(got %HASH%, expected %~3^); deleting it
del "%~2"
exit /b 1

rem ---------------------------------------------------------------------------------------------------------------------------------
rem Everything below is data for the powershell step above, never executed (the script has exited).
#BEGIN winhost.c
/* winhost.c -- Windows host-portability shim linked into cc1.exe / cpp.exe.  Not part of GCC.  Runs before main():
   - makes every stream and every fopen/open default to binary mode, so the assembly and preprocessed text are written
     byte-exact with LF endings;
   - if TMPDIR is not set, points it at the Windows temp directory (the GCC source otherwise falls back to the unusable
     "backslash-slash ctXXXXXX").  */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

static void __attribute__((constructor)) winhost_init (void)
{
  char buf[MAX_PATH + 8];
  _set_fmode (_O_BINARY);
  _setmode (0, _O_BINARY);
  _setmode (1, _O_BINARY);
  _setmode (2, _O_BINARY);
  if (!getenv ("TMPDIR"))
    {
      DWORD n = GetTempPathA (MAX_PATH, buf);
      if (n > 0 && n < MAX_PATH)
	{
	  char *e;
	  /* drop trailing separators; GCC appends '/' itself */
	  while (n > 3 && (buf[n-1] == '\\' || buf[n-1] == '/')) buf[--n] = 0;
	  e = (char *) malloc (n + 8);
	  if (e) { sprintf (e, "TMPDIR=%s", buf); _putenv (e); }
	}
    }
}

/* BSD string functions that GCC 2.6.3 sources call by name and that the Windows C runtime does not provide (weak: cccp.c has
   its own bcopy/bzero).  */
#include <string.h>
__attribute__((weak)) char *index (const char *s, int c) { return strchr (s, c); }
__attribute__((weak)) char *rindex (const char *s, int c) { return strrchr (s, c); }
__attribute__((weak)) int bcmp (const void *a, const void *b, size_t n) { return memcmp (a, b, n); }
__attribute__((weak)) void bcopy (const void *s, void *d, size_t n) { memmove (d, s, n); }
__attribute__((weak)) void bzero (void *d, size_t n) { memset (d, 0, n); }
#END winhost.c
#BEGIN config.h
/* config.h / hconfig.h -- hand-written host configuration for a 32-bit Windows (MinGW-w64 i686) host.  Replaces the
   i386/xm-linux.h link that configure created.  Host-portability only; no effect on generated code. */
#include "i386/xm-i386.h"
#define bcopy(src,dst,len) memcpy ((dst),(src),(len))
#define bzero(dst,len) memset ((dst),0,(len))
#define bcmp(left,right,len) memcmp ((left),(right),(len))
#define rindex strrchr
#define index strchr
#define USG
#define HAVE_VPRINTF
#END config.h
#BEGIN build.sh
set -e
B=$(cygpath -u "$BLD")
export PATH="$B/mingw32/bin:/usr/bin"
export CC=gcc
cd "$B"
rm -rf gcc-2.6.3 out
mkdir out
tar xzf dl/gcc-2.6.3.tar.gz
cd gcc-2.6.3

# the recipe of decompals/old-gcc (gcc-2.6.3-psx.Dockerfile). git apply cannot take the file name patch(1) is given on the
# command line, so the "+++" line of the two single-file patches is pointed at the file they are meant for. cp/g++.c (the g++
# driver) is not built and not patched.
sed -i -- 's/include <varargs.h>/include <stdarg.h>/g' *.c
sed -e '2s#^+++ .*#+++ gcc-2.7.2.3/obstack.h#' "$B/dl/obstack-2.7.2.h.patch" | git apply -p1
sed -e '2s#^+++ .*#+++ gcc-2.6.3/sdbout.c#' "$B/dl/sdbout-2.6.3.c.patch" | git apply -p1
git apply -p1 "$B/dl/psx-2.5.7.patch"
touch -c cp/parse.y cp/parse.h cp/parse.c
touch insn-config.h

# Windows host: // comments are stripped by default, as in GCC 2.7.x (the project's sources use them; -lang-c restores 2.6.3)
sed -i 's|^  cplusplus_comments = 0;$|  cplusplus_comments = 1; /* windows-port */|' cccp.c
grep -q 'cplusplus_comments = 1; /\* windows-port' cccp.c

sh ./configure --target=mips-sony-psx --prefix=/opt/cross --with-endian-little --with-gnu-as \
    --host=i386-pc-linux --build=i386-pc-linux > configure.log 2>&1 || { tail -30 configure.log; exit 1; }
rm -f config.h hconfig.h
cp "$B/config.h" config.h
cp "$B/config.h" hconfig.h

gcc -O1 -c "$B/winhost.c" -o "$B/winhost.o"
CF="-std=gnu89 -O1 -fpermissive -w -fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks -static -Dbsd4_4 -Dmips -DHAVE_STRERROR -Dalloca=__builtin_alloca -DPOSIX"
make -k cc1 cccp CC=gcc CFLAGS="$CF" CLIB="$B/winhost.o"

[ -f cc1.exe ] && [ -f cccp.exe ]
strip cc1.exe cccp.exe
cp cc1.exe "$B/out/cc1.exe"
cp cccp.exe "$B/out/cpp.exe"
#END build.sh
