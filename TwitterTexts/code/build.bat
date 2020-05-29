@echo off

set INCLUDE=C:\Users\Joel Gahr\Desktop\vcpkg-master\vcpkg-master\installed\x64-windows\include\curl\;%INCLUDE%
rem set LIBPATH=C:\Users\Joel Gahr\Desktop\curl-7.70.0\builds\libcurl-vc-x64-release-static-ipv6-sspi-winssl\lib\;%LIBPATH%
set LIB=C:\Users\Joel Gahr\Desktop\vcpkg-master\vcpkg-master\installed\x64-windows\lib;%LIB%
rem set WindowsLibPath=C:\Users\Joel Gahr\Desktop\curl-7.70.0\builds\libcurl-vc-x64-release-static-ipv6-sspi-winssl\lib\;%WindowsLibPath%
mkdir ..\..\build
pushd ..\..\build
cl -Zi ..\TwitterTexts\code\TwitterTexts.cpp Ws2_32.lib libcurl.lib

popd
