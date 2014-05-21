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

pushd debug
dir /S /B *.h >> ..\files.txt
popd

pushd display
dir /S /B *.h >> ..\files.txt
popd

pushd file
dir /S /B *.h >> ..\files.txt
popd

pushd gui
dir /S /B *.h >> ..\files.txt
popd

pushd inAppPurchase
dir /S /B *.h >> ..\files.txt
popd

pushd input
dir /S /B *.h >> ..\files.txt
popd

pushd ios
dir /S /B *.h >> ..\files.txt
popd

pushd linux
dir /S /B *.h >> ..\files.txt
popd

pushd locale
dir /S /B *.h >> ..\files.txt
popd

pushd math
dir /S /B *.h >> ..\files.txt
popd

pushd memory
dir /S /B *.h >> ..\files.txt
popd

pushd multiplayer
dir /S /B *.h >> ..\files.txt
popd

pushd online
dir /S /B *.h >> ..\files.txt
popd

pushd particle
dir /S /B *.h >> ..\files.txt
popd

pushd persistence
dir /S /B *.h >> ..\files.txt
popd

pushd thread
dir /S /B *.h >> ..\files.txt
popd

pushd utf8proc
dir /S /B *.h >> ..\files.txt
popd

pushd util
dir /S /B *.h >> ..\files.txt
popd

pause
