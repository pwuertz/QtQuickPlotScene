find_package(Qt6 COMPONENTS QuickTest REQUIRED)

add_executable(tst_basic "tst_basic.cpp" "tst_basic.qml")

add_test(NAME tst_basic COMMAND tst_basic)

target_link_libraries(tst_basic PRIVATE
    Qt6::QuickTest
    QtQuickPlotScene::QtQuickPlotScene
    )

target_compile_definitions(tst_basic PRIVATE
    QUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_LIST_DIR}"
    )
