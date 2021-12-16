import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuickPlotScene as QtQuickPlotScene

ApplicationWindow {
    id: approot
    visible: true
    title: "QtQuickPlotScene Demo"
    minimumWidth: 800
    minimumHeight: 600

    // Demonstration pages
    ListModel {
        id: demoPages
        ListElement { name: "Line plot"; source: "qrc:/pages/LinePlot.qml"}
        ListElement { name: "Containers"; source: "qrc:/pages/Containers.qml"}
    }
    header: ToolBar {
        focus: true
        Keys.onEscapePressed: { approot.close(); }
        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: Qt.application.font.pixelSize
            Label { text: approot.title }
            ComboBox {
                id: pageSelector
                textRole: "name"
                model: demoPages
                currentIndex: Qt.application.arguments.length > 1 ? Qt.application.arguments[1] : 0
                implicitWidth: 10 * Qt.application.font.pixelSize
            }
        }
    }
    Loader {
        anchors.fill: parent
        source: demoPages.get(pageSelector.currentIndex).source
    }
}
