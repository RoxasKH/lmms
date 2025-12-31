import QtQuick 2.15
import QtQuick.Layouts 1.15

GridLayout {
    id: table

    property int gridHeight: 100
    property int gridColumns: 88
    property int gridRows: 2

    columns: gridColumns
    rowSpacing: 0
    columnSpacing: 0

    width: parent.width

    Repeater {
        model: gridColumns * gridRows

        Item {
            Layout.fillWidth: true
            //Layout.fillHeight: true
            height: gridHeight / 2

            // column & row index
            property int col: index % table.columns
            property int row: Math.floor(index / table.columns)

            // LEFT border (only first column)
            Rectangle {
                visible: col === 0
                width: 1
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: "#444"
            }

            // TOP border (only first row)
            Rectangle {
                visible: row === 0
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                color: "#444"
            }

            // RIGHT border (always)
            Rectangle {
                width: 1
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: "#444"
            }

            // BOTTOM border (always)
            Rectangle {
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                color: "#444"
            }
        }
    }
}