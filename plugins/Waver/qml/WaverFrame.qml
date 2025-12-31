import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/*Frame {
    id: frame

    property Component content
    property string title

    padding: 0

    background: Rectangle {
        id: borderContainer
        color: "transparent"
        border.color: theme.primaryColor
        border.width: 1
    }

    WaverTheme {
        id: theme
    }

    ColumnLayout {
        id: layout

        spacing: 0

        // Header
        Rectangle {
            height: textItem.implicitHeight + 2  // Add padding
            id: header
            color: theme.primaryColor
            Layout.fillWidth: true

            WaverText {
                id: textItem
                text: frame.title
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.fill: parent
            }
        }

        Loader {
            id: loader
            sourceComponent: frame.content
        }
    }
}*/

ColumnLayout {
    id: layout

    property Component content
    property string title

    spacing: 0

    WaverTheme {
        id: theme
    }

    // Header
    Rectangle {
        height: textItem.implicitHeight + 2  // Add padding
        id: header
        color: theme.primaryColor
        Layout.fillWidth: true

        WaverText {
            id: textItem
            text: layout.title
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.fill: parent
        }
    }

    Frame {
        id: frame
        Layout.fillWidth: true
        background: Rectangle {
            id: borderContainer
            color: "transparent"
            border.color: theme.primaryColor
            border.width: 1
        }

        Loader {
            id: loader
            anchors.fill: parent
            sourceComponent: content
        }
    }

}

