import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuickPlotScene as QtQuickPlotScene

/**
  XYPlot example
  */

Page {
    id: root

    function generateTestCoords() {
        const n = 1000;
        const sig = .5;
        const coords = Array(2 * n);
        for (let i=0; i < n; ++i) {
            const x = 2 * i / n - 1;
            const y = Math.exp(-x*x/(2*sig*sig)) + .01 * Math.random();
            coords[2*i+0] = x;
            coords[2*i+1] = y;
        }
        return coords;
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 5
        // Plot axes for XY plot demonstration
        QtQuickPlotScene.Axes {
            Layout.fillHeight: true
            Layout.fillWidth: true
            xLabel: "X Axis"
            yLabel: "Y Axis"
            // Define plot group containing zoom/pan tool and XY plot item
            plotGroup: QtQuickPlotScene.PlotGroup {
                viewRect: Qt.rect(-1, -.1, 2, 1.2)
                clip: true
                plotItems: [
                    QtQuickPlotScene.ZoomPanTool {
                        maxViewRect: Qt.rect(-2., -1., 4., 3.)
                        minimumSize: Qt.size(0.1, 0.1)
                    },
                    // Line plot item with some test data
                    QtQuickPlotScene.LinePlot {
                        id: linePlot
                        lineColor: Qt.hsva(hueSlider.hue, 1., 1., 1.)
                        lineWidth: lineWidthSlider.value
                        coordinates: generateTestCoords()
                    }
                ]
            }
        }
        // Controls for changing plot properties
        RowLayout {
            Layout.margins: Qt.application.font.pixelSize
            spacing: Qt.application.font.pixelSize / 2
            // Change hue values
            Slider {
                id: hueSlider
                readonly property real hue: value * (1. / to)
                from: 0; to: 100; stepSize: 1; value: 55
                ToolTip.visible: hovered || pressed
                ToolTip.text: "Hue value"
            }
            // Change line width
            Slider {
                id: lineWidthSlider
                from: 1; to: 10; stepSize: 1; value: 3
                ToolTip.visible: hovered || pressed
                ToolTip.text: "Line width " + value
            }
        }
    }
}
