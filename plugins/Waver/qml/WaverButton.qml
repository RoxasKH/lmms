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
        color: control.down ? theme.colors.text : "#21be2b"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 50
        opacity: enabled ? 1 : 0.3
        color: control.down ? theme.colors.border : "transparent"
        border.color: theme.colors.border
        border.width: 1
        radius: 2
    }

}
