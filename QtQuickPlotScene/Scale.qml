import QtQuick
import QtQuickPlotScene as QtQuickPlotScene

Rectangle {
    id: root

    // Style settings
    color: "#ccffffff"
    border.color: "black"
    border.width: 1.
    property real padding: 2
    property alias font: label.font

    // Scale control
    property Item plotItem: parent
    property string displayText: scaleText
    property real scaleMinWidth: .25 * parent.width
    property real scaleFactor: 1.

    readonly property real _fromSceneFactor: scaleFactor * plotItem.viewRect.width / plotItem.width;
    readonly property point _niceScaleTuple: QtQuickPlotScene.Utils.niceNumPrec(_fromSceneFactor * scaleMinWidth)
    readonly property real scaleValue: _niceScaleTuple.x
    readonly property real scalePrecision: _niceScaleTuple.y
    readonly property string scaleText: scaleValue.toFixed(scalePrecision)
    width: scaleValue / _fromSceneFactor;
    implicitHeight: label.implicitHeight

    Text {
        anchors.centerIn: parent
        id: label
        text: root.displayText
    }
}
