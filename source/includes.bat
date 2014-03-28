@echo off
del files.txt

pushd actor
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

pushd input
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

pushd persistance
dir /S /B *.h >> ..\files.txt
popd

pushd thread
dir /S /B *.h >> ..\files.txt
popd

pushd utf8proc
dir /S /B *.h >> ..\files.txt
popd

pause
