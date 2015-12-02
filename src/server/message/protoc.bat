@echo off

@set SRC_DIR=proto

cd %SRC_DIR%

for /f "delims=" %%i in ('dir /b /a-h "*.proto"') do (call protoc.exe -I=./ --cpp_out=../source %%i)

@pause