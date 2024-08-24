#!/bin/bash

export QML_SOURCES_PATHS=/File-Transfer/res/qml
export QMAKE=/opt/Qt/6.5.0/gcc_64/bin/qmake
export LD_LIBRARY_PATH=/opt/Qt/6.5.0/gcc_64/lib
/home/user/linux-deploy.AppImage --appdir /File-Transfer/build/AppDir/ --output appimage --plugin qt