import QtQuick 2.15
import QtQuick.Layouts 1.15

GridLayout {
    id: grid

    property int gridColumns: 4
    property int gridRows: 4
    property color gridColor: theme.colors.border 

    columns: gridColumns
    rowSpacing: 0
    columnSpacing: 0

    width: parent.width

    WaverTheme {
        id: theme
    }

    Repeater {
        model: gridColumns * gridRows

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // column & row index
            property int col: index % grid.columns
            property int row: Math.floor(index / grid.columns)

            // LEFT border (only first column)
            Rectangle {
                visible: col === 0
                width: 1
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: gridColor
            }

            // TOP border (only first row)
            Rectangle {
                visible: row === 0
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                color: gridColor
            }

            // RIGHT border (always)
            Rectangle {
                width: 1
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: gridColor
            }

            // BOTTOM border (always)
            Rectangle {
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                color: gridColor
            }
        }
    }
}
