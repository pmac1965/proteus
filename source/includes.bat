@echo off
del files.txt

pushd achievements
dir /S /B *.h > ..\files.txt
popd

pushd actor
dir /S /B *.h > ..\files.txt
popd

pushd adverts
dir /S /B *.h > ..\files.txt
popd

pushd analytics
dir /S /B *.h > ..\files.txt
popd

pushd android
dir /S /B *.h > ..\files.txt
popd

pushd audio
dir /S /B *.h >> ..\files.txt
popd

pushd collision
dir /S /B *.h >> ..\files.txt
popd

pushd core
dir /S /B *.h >> ..\files.txt
popd

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

pushd particle
dir /S /B *.h >> ..\files.txt
popd

pushd persistance
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
