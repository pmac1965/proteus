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
    if exist ..\x64\Debug64\proteus64_d.lib   copy /B /V /Y ..\x64\Debug64\proteus64_d.lib   %PROTEUS_ENGINE_DIR%\libs\proteus64_d.lib
    if exist ..\x64\Release64\proteus64_r.lib copy /B /V /Y ..\x64\Release64\proteus64_r.lib %PROTEUS_ENGINE_DIR%\libs\proteus64_r.lib


REM ---------------------------------------------------------------------------
REM - Create the includes folder
REM ---------------------------------------------------------------------------
    echo Deleting old includes directory
    rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus
    
    echo Creating include directory
    md %PROTEUS_ENGINE_DIR%\include\proteus

    echo Copying the source to include directory.
    xcopy .\..\..\..\..\source\*.h   %PROTEUS_ENGINE_DIR%\include\proteus /E /Q
    xcopy .\..\..\..\..\source\*.inl %PROTEUS_ENGINE_DIR%\include\proteus /E /Q
    copy  .\..\..\..\..\source\includes.bat %PROTEUS_ENGINE_DIR%\include\proteus\includes.bat


REM ---------------------------------------------------------------------------
REM Delete files which aren't needed in the include files folder
REM as they are either not needed, or shouldn't be there as they expose
REM interfaces which shouldn't be visible
REM
REM Also, lesser includes means a faster compile for applications
REM ---------------------------------------------------------------------------
    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\display\prSplash.h
       
    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\utf8proc\utf8proc_data.h

    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\achievements\prAchievement_android.h
    REM del %PROTEUS_ENGINE_DIR%\include\proteus\achievements\prAchievement_base.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\achievements\prAchievement_ios.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\achievements\prAchievement_pc.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\achievements\prAchievement_linux.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\achievements\prAchievement_mac.h

    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\debug\prDebugFont.h 
    del %PROTEUS_ENGINE_DIR%\include\proteus\debug\prFps_PC.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\debug\prFps_Android.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\debug\prFps_ios.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\debug\prFps_Linux.h

    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\audio\prSoundManager_PC.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\audio\prSoundManager_Ios.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\audio\prSoundManager_Android.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\audio\prSoundManager_Linux.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\audio\prWaves.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\audio\prOpenALDeviceList.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\audio\prOpenALErrors.h

    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\inAppPurchase\prStore_pc.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\inAppPurchase\prStore_mac.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\inAppPurchase\prStore_linux.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\inAppPurchase\prStore_android.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\inAppPurchase\prStore_ios.h

    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\analytics\Flurry.h

    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    del %PROTEUS_ENGINE_DIR%\include\proteus\social\twitter\prTwitter_Android.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\social\twitter\prTwitter_ios.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\social\twitter\prTwitterBase.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\social\facebook\prFacebook_Android.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\social\facebook\prFacebook_ios.h
    del %PROTEUS_ENGINE_DIR%\include\proteus\social\facebook\prFacebookBase.h

    REM ****************
    REM * NOT REQUIRED *
    REM ****************
    rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\libzip
    rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\android\AL
    rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\audio\external
    REM rd /Q /S %PROTEUS_ENGINE_DIR%\include\proteus\android


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
REM     if exist D:\NaturalDocs-1.52\doc_engine.bat (
REM         echo Updating docs
REM         cd D:\NaturalDocs-1.52
REM         doc_engine.bat
REM     )
