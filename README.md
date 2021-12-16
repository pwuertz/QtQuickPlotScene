# QtQuickPlotScene

QtQuickPlotScene is a collection of QtQuick items for data visualization in Qt6.

> **_NOTE:_** This project is a work-in-progress continuation of [QmlPlotting](https://github.com/pwuertz/qmlplotting). Some functionality is still missing and requires porting from Qt5 to Qt6.

![XY Plot](doc/demo-plotxy.png)
![2D Data](doc/demo-colormappedimage.png)
![Containers](doc/demo-containers.png)

## Building / Including

QtQuickPlotScene is based on Qt 6.2 and uses CMake for building. For applications built with CMake you can fetch and add the library to your build process..
```cmake
# Fetch and include QtQuickPlotScene
include(FetchContent)
FetchContent_Declare(qtquickplotscene
    GIT_TAG        master  # <- Ideally this should be a specific tag, not a branch
    GIT_REPOSITORY https://github.com/pwuertz/QtQuickPlotScene.git
    SOURCE_SUBDIR  "QtQuickPlotScene"
    )
FetchContent_MakeAvailable(qtquickplotscene)
```
.. and add the library to your application(s) dependencies
```cmake
target_link_libraries(application PRIVATE QtQuickPlotScene::QtQuickPlotScene)
```

## Documentation
A documentation of the API does not exist yet. Until then the [example application](examples/demo) serves as a reference for using QtQuickPlotScene in other applications.

## License
QtQuickPlotScene is available under the [MIT license](LICENSE).
