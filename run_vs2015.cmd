REM call gen_vs2015.cmd
call build_vs2015.cmd

@ECHO Running bechmark ... 

".build/vs2015/Release/bechmark.exe" -r html -o results.html

@ECHO Done.
@ECHO Results are saved to results.html
