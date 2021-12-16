find_package(Qt6 COMPONENTS Core Qml Quick REQUIRED)

qt_add_library(QtQuickPlotScene MANUAL_FINALIZATION)

add_library(QtQuickPlotScene::QtQuickPlotScene ALIAS QtQuickPlotScene)

set_target_properties(QtQuickPlotScene PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    AUTORCC ON
    )

target_link_libraries(QtQuickPlotScene PRIVATE
    Qt6::Core Qt6::Quick Qt6::Qml
    )

qt_add_qml_module(QtQuickPlotScene
    URI QtQuickPlotScene
    VERSION 1.0
    SOURCES
        "plotgroup.hpp"
        "plotgroup.cpp"
        "lineplot.hpp"
        "lineplot.cpp"
        "utils.hpp"
        "utils.cpp"
    QML_FILES
        "ZoomPanTool.qml"
        "Axes.qml"
        "Container.qml"
        "MovableContainer.qml"
        "Scale.qml"
    OUTPUT_DIRECTORY
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QtQuickPlotScene"
    )

qt_finalize_target(QtQuickPlotScene)
