@echo off

mkdir ..\..\build
pushd ..\..\build
cl -Zi ..\TwitterTexts\code\TwitterTexts.cpp user32.lib 

popd
