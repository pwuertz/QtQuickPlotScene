import QtQuick 2.7
import QtTest 1.1
import QtQuickPlotScene 1.0 as QtQuickPlotScene

Item {
    width: 300; height: 200

    QtQuickPlotScene.PlotGroup {
        id: plotGroup
        anchors.fill: parent
        viewRect: Qt.rect(-1, -1, 2, 2)
        plotItems: [
            QtQuickPlotScene.Container { id: container1 },
            QtQuickPlotScene.Container { id: container2 },
            QtQuickPlotScene.LinePlot { id: linePlot }
        ]
    }

    TestCase {
        function test_viewRectBinding() {
            // Verify view rect forwarding to plot items
            compare(plotGroup.viewRect, container1.viewRect);
            compare(plotGroup.viewRect, container2.viewRect);
            compare(plotGroup.viewRect, linePlot.viewRect);
        }

        function test_setLinePlotCoordinates() {
            // Test line plot coordinate roundtrip
            const testCoords = [-1, -1, 1, 1];
            linePlot.coordinates = testCoords;
            const plotCoords = linePlot.coordinates;
            plotCoords.forEach((value, i) => compare(value, testCoords[i]));
        }
    }
}
