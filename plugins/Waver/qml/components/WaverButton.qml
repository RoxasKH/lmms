import QtQuick 2.15
import QtQuick.Controls 2.15

// Define a reusable custom button component
Button {
    id: control
    text: qsTr("Default Title")

    width: parent.width
    height: 50

    // Create an instance of WaverTheme
    WaverTheme {
        id: theme
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? theme.colors.text : theme.colors.textDark
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 50
        opacity: enabled ? 1 : 0.3
        color: control.down ? theme.colors.primary : theme.colors.border
        radius: 5
    }

}
