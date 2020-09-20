@echo on

rem MCD - make and change directory in one command
rem
rem See :help section for more information, or run with /? as a command line argument
rem
rem Note: don't use setlocal here as that prevents the CD from applying to the calling environment

if .%1.==./?. (
    goto :help
) 

rem Set up for later where we will be issuing a MD, followed by an CD
set cdArgs=%*
set mdArgs=%*

rem Note that if 
if /I .%1.==./D. (
    set mdArgs=%mdArgs:~3%
) 

MD %mdArgs%
CD %cdArgs%

set cdArgs=
set mdArgs=
goto :eof

:help
echo MCD - make and change directory
echo:
echo Displays the name of or changes the current directory. If the directory doesn't exist, it is created.
echo:
echo %~n0 [/D] [drive:][path]
echo %~n0 [..]
echo:
echo   ..   Specifies that you want to change to the parent directory.
echo:
echo Type %~n0 drive: to display the current directory in the specified drive.
echo Type %~n0 without parameters to display the current drive and directory.
echo:
echo Use the /D switch to change current drive in addition to changing current
echo directory for a drive.
echo:
echo Type %~n0 /? to display this help information.
echo:
echo %~n0 does not treat spaces as delimiters, so it is possible to
echo create and change into a subdirectory name that contains a space without surrounding
echo the name with quotes. For example:
echo:
echo     %~n0 \winnt\profiles\username\programs\start menu
echo:
echo is the same as:
echo:
echo     %~n0 "\winnt\profiles\username\programs\start menu"
echo:
echo which is what you would have to type if extensions were disabled.
echo:
echo %~n0 will not modify the case of the path when creating directories, but this does not
echo necessarily mean that the underlying operating system will behave similarly.
