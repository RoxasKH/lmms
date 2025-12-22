import QtQuick 2.15
import QtQuick.Controls 2.15

// Define a reusable custom button component
Text {

    // Create an instance of WaverTheme
    WaverTheme {
        id: theme
    }

    text: qsTr("Default Text")
    color: theme.textColor
    font.bold: true
    font.pointSize: 18
}
