call gen_vs2015.cmd

@echo off
if not defined VS140COMNTOOLS (
	echo In order to build this project you need MS Visual Studio 2015 to be installed.
	echo http://msdn.microsoft.com/vstudio/express/downloads/default.aspx
	exit
)

call "%VS140COMNTOOLS%\vsvars32.bat" || exit /b 1

cd .build/vs2015
devenv.com cpp2lua-bechmarks.sln /build release

cd ../..
rem call run.cmd
rem pause
