@echo off

mkdir ..\build
pushd ..\build

set COMPILER_FLAGS=/Od /GS /Fm /Z7 /Wall /wd4668 /wd4255

cl %COMPILER_FLAGS% ..\src\main.c

popd
