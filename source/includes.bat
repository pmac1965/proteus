@echo off
REM ---------------------------------------------------------------------------
REM 
REM This file is used to generate 'proteus.h' from the includes folder
REM 
REM The includes folder only contains the public files, as many files are
REM stripped from the project as they do not need to be available publicly
REM 
REM ---------------------------------------------------------------------------


if exist files.txt (
del files.txt
)


if exist achievements (
pushd achievements
dir /S /B *.h > ..\files.txt
popd
)

if exist actor (
pushd actor
dir /S /B *.h >> ..\files.txt
popd
)

if exist adverts (
pushd adverts
dir /S /B *.h >> ..\files.txt
popd
)

if exist analytics (
pushd analytics
dir /S /B *.h >> ..\files.txt
popd
)

if exist android (
pushd android
dir /S /B *.h >> ..\files.txt
popd
)

if exist audio (
pushd audio
dir /S /B *.h >> ..\files.txt
popd
)

if exist collision (
pushd collision
dir /S /B *.h >> ..\files.txt
popd
)

if exist core (
pushd core
dir /S /B *.h >> ..\files.txt
popd
)

if exist debug (
pushd debug
dir /S /B *.h >> ..\files.txt
popd
)

if exist display (
pushd display
dir /S /B *.h >> ..\files.txt
popd
)

if exist file (
pushd file
dir /S /B *.h >> ..\files.txt
popd
)

if exist font (
pushd font
dir /S /B *.h >> ..\files.txt
popd
)

if exist gui (
pushd gui
dir /S /B *.h >> ..\files.txt
popd
)

if exist inAppPurchase (
pushd inAppPurchase
dir /S /B *.h >> ..\files.txt
popd
)

if exist input (
pushd input
dir /S /B *.h >> ..\files.txt
popd
)

if exist ios (
pushd ios
dir /S /B *.h >> ..\files.txt
popd
)

if exist linux (
pushd linux
dir /S /B *.h >> ..\files.txt
popd
)

if exist locale (
pushd locale
dir /S /B *.h >> ..\files.txt
popd
)

if exist math (
pushd math
dir /S /B *.h >> ..\files.txt
popd
)

if exist memory (
pushd memory
dir /S /B *.h >> ..\files.txt
popd
)

if exist mesh (
pushd mesh
dir /S /B *.h >> ..\files.txt
popd
)

if exist multiplayer (
pushd multiplayer
dir /S /B *.h >> ..\files.txt
popd
)

if exist notifications (
pushd notifications
dir /S /B *.h >> ..\files.txt
popd
)

if exist online (
pushd online
dir /S /B *.h >> ..\files.txt
popd
)

if exist particle (
pushd particle
dir /S /B *.h >> ..\files.txt
popd
)

if exist persistence (
pushd persistence
dir /S /B *.h >> ..\files.txt
popd
)

if exist script (
pushd script
dir /S /B *.h >> ..\files.txt
popd
)

if exist social (
pushd social
dir /S /B *.h >> ..\files.txt
popd
)

if exist system (
pushd system
dir /S /B *.h >> ..\files.txt
popd
)

if exist thread (
pushd thread
dir /S /B *.h >> ..\files.txt
popd
)

if exist utf8proc (
pushd utf8proc
dir /S /B *.h >> ..\files.txt
popd
)

if exist util (
pushd util
dir /S /B *.h >> ..\files.txt
popd
)

pause
