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

    if exist ..\Debug\proteus_d.lib   copy /B /V /Y ..\Debug\proteus_d.lib   %PROTEUS_ENGINE_DIR%\libs\proteus_d.lib
    if exist ..\Release\proteus_r.lib copy /B /V /Y ..\Release\proteus_r.lib %PROTEUS_ENGINE_DIR%\libs\proteus_r.lib


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


REM ---------------------------------------------------------------------------
REM Delete files/folders which shouldn't be public
REM ---------------------------------------------------------------------------
@REM    IF "%~1"=="FULL" (
@REM    echo FULL VERSION
@REM    GOTO :EOF
@REM    )


    @REM --------------------------------------
    @REM Delete dirs which you don't get in the
    @REM full version.
    @REM --------------------------------------
    @REM @rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\proteus\gui
    @REM @rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\proteus\xyz
    @REM @rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\proteus\network
    @REM @rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\proteus\android
    @REM @rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\proteus\libpng
    @REM @rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\proteus\libzip


REM ---------------------------------------------------------------------------
REM Update engine docs
REM ---------------------------------------------------------------------------
echo Updating docs
cd D:\NaturalDocs-1.52
doc_engine.bat
