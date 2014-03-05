REM ---------------------------------------------------------------------------
REM 
REM Copies the libraries to the lib folder.
REM Creates the include folder and copies the engine folders
REM 
REM ---------------------------------------------------------------------------


set PROTEUS_ENGINE_DIR=D:\engine


REM ---------------------------------------------------------------------------
REM - Copy libs
REM ---------------------------------------------------------------------------

    if exist ..\Debug\proteus_d.lib           copy /B /V /Y ..\Debug\proteus_d.lib           %PROTEUS_ENGINE_DIR%\libs\proteus_d.lib
    if exist ..\Release\proteus_r.lib         copy /B /V /Y ..\Release\proteus_r.lib         %PROTEUS_ENGINE_DIR%\libs\proteus_r.lib
    if exist "..\Tool debug\proteus_td.lib"   copy /B /V /Y "..\Tool debug\proteus_td.lib"   %PROTEUS_ENGINE_DIR%\libs\proteus_td.lib
    if exist "..\Tool release\proteus_tr.lib" copy /B /V /Y "..\Tool release\proteus_tr.lib" %PROTEUS_ENGINE_DIR%\libs\proteus_tr.lib


REM ---------------------------------------------------------------------------
REM - Create the includes
REM ---------------------------------------------------------------------------
    echo Deleting old includes directory
    rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus
    
    echo Creating include directory
    md %PROTEUS_ENGINE_DIR%\include\proteus

    echo Copy the source to include directory.
    xcopy .\..\..\..\..\source\*.h   %PROTEUS_ENGINE_DIR%\include\proteus /E /Q
    xcopy .\..\..\..\..\source\*.inl %PROTEUS_ENGINE_DIR%\include\proteus /E /Q
    copy  .\..\..\..\..\source\includes.bat %PROTEUS_ENGINE_DIR%\include\proteus\includes.bat


REM --------------------------------------
REM Delete files which you aren't needed
REM --------------------------------------
    del %PROTEUS_ENGINE_DIR%\include\proteus\display\prSplash.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\debug\prDebugFont.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\utf8proc\utf8proc_data.h

    rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\libzip
    REM rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\audio\external


REM --------------------------------------
REM Update build number
REM --------------------------------------
    echo Updating build number
    setlocal EnableDelayedExpansion

    set /P BUILD_NUM=<vernum.txt
    echo BUILD_NUMBER=%BUILD_NUM%
    set /A BUILD_NUM+=1
    echo %BUILD_NUM% > vernum.txt
    echo #define BUILD_NUMBER %BUILD_NUM% > ../../../../source/prVerNum.h


REM ---------------------------------------------------------------------------
REM Update engine docs
REM ---------------------------------------------------------------------------
    echo Updating docs
    cd D:\NaturalDocs-1.52
    doc_engine.bat
