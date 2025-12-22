import QtQuick 2.15
import QtQuick.Controls 2.15

// Define a reusable custom button component
Button {
    id: control
    text: qsTr("Default Title")

    property string iconSource: ""          // Icon source path
    property var onClickAction: null        // Custom action for the button click (a function)

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
        color: control.down ? theme.buttonTextColor : "#21be2b"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        color: control.down ? theme.buttonColor : "transparent"
        border.color: control.down ? theme.buttonColor : theme.buttonColor
        border.width: 1
        radius: 2
    }

}
