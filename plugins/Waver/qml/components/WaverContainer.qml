import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Frame {
    id: container
    padding: theme.spacing.m

    WaverTheme {
        id: theme
    }

    background: Rectangle {
        id: borderContainer
        color: theme.colors.containerDark
        border.color: theme.colors.border
        border.width: 1
        radius: 10
    }

}
