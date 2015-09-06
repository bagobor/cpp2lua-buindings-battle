REM call gen_vs2015.cmd
call build_vs2015.cmd

@ECHO Running bechmark ... 

cd results
"../.build/vs2015/Release/bechmark.exe"
cd ..

@ECHO Done.
@ECHO Results are saved to results.html
