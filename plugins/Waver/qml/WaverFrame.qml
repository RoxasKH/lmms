import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: layout

    property Component content
    property string title

    spacing: 0

    WaverTheme {
        id: theme
    }

    // Header
    Frame {
        id: header
        Layout.fillWidth: true
        padding: 0

        background: Rectangle {
            color: theme.colors.border
        }

        WaverText {
            id: textItem
            text: layout.title.toUpperCase()
            anchors.leftMargin: theme.spacing.l
            anchors.rightMargin: theme.spacing.l
            anchors.fill: parent
            font.bold: true
        }
    }

    Frame {
        id: frame
        Layout.fillWidth: true

        leftPadding: theme.spacing.l
        rightPadding: theme.spacing.l

        background: Rectangle {
            id: borderContainer
            color: theme.colors.container
            border.color: theme.colors.border
            border.width: 1
        }

        Loader {
            id: loader
            anchors.fill: parent
            sourceComponent: content
        }
    }

}

