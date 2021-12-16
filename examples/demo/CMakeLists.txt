find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

add_executable(demo "demo.cpp")

target_link_libraries(demo PRIVATE
    Qt6::Quick Qt6::QuickControls2 QtQuickPlotScene::QtQuickPlotScene
    )

qt_add_resources(demo "resources" PREFIX "/" FILES
    "demo.qml"
    "pages/Containers.qml"
    "pages/LinePlot.qml"
    )
