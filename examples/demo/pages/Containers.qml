import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuickPlotScene as QtQuickPlotScene

/**
  Container / MovableContainer example
  */

Page {
    id: root
    // Plot axes for showing container positions
    QtQuickPlotScene.Axes {
        anchors.fill: parent
        // Define plot group containing zoom/pan tool and example containers with Quick items
        plotGroup: QtQuickPlotScene.PlotGroup {
            clip: true
            viewRect: Qt.rect(-3, -2, 6, 5)
            viewMode: QtQuickPlotScene.PlotGroup.PreserveAspectFit
            plotItems: [
                QtQuickPlotScene.ZoomPanTool {},
                // Fixed container
                QtQuickPlotScene.Container {
                    itemRect: Qt.rect(-2, 0, 1, 1)
                    Rectangle {
                        color: "lightsteelblue"; border.width: 1; border.color: "steelblue"
                        Label { text: "Fixed"; anchors.centerIn: parent }
                    }
                },
                // Resize- and movable container
                QtQuickPlotScene.MovableContainer {
                    itemRect: Qt.rect(0, 0, 1, 1)
                    onItemRectChanged: plotLabel.showRect(itemRect)
                    Rectangle {
                        color: "lightsteelblue"; border.width: 1; border.color: "steelblue"
                        Label { text: "Movable"; anchors.centerIn: parent }
                    }
                },
                // Container with resize constraint
                QtQuickPlotScene.MovableContainer {
                    itemRect: Qt.rect(2, 0, 1, 1)
                    maxItemRect: Qt.rect(-3, -2, 6, 5)
                    onItemRectChanged: plotLabel.showRect(itemRect)
                    aspectFixed: true
                    Rectangle {
                        color: "lightsteelblue"; border.width: 1; border.color: "steelblue"
                        radius: width
                        Label { text: "Constrained"; anchors.centerIn: parent }
                    }
                }
            ]

            // Add label for showing rectangle coordinates
            Label {
                id: plotLabel
                text: "Move a container"
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: Qt.application.font.pixelSize
                Behavior on opacity { NumberAnimation { duration: 200 } }
                function showRect(rect) {
                    var coords = "(" + rect.x.toFixed(2) + ", " + rect.y.toFixed(2) + ")";
                    var size = rect.width.toFixed(2) + " x " + rect.height.toFixed(2)
                    text = "Bottom left " + coords + ", Size " + size;
                    opacity = 1;
                    labelTimeout.restart();
                }
                Timer {
                    id: labelTimeout
                    interval: 2000
                    running: false
                    repeat: false
                    onTriggered: plotLabel.opacity = 0;
                }
            }
        }
    }
}
