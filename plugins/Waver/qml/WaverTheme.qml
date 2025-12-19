pragma Singleton

import QtQuick 2.15

QtObject {
    property color primaryColor: "#4CAF50"
    property color secondaryColor: "#FF5722"
    property color textColor: "#333333"
    property color backgroundColor: "#f0f0f0"
    property color buttonColor: "#2196F3"
    property color buttonTextColor: "#ffffff"
    property real fontSize: 14

    // Example of a custom component or style
    Component.onCompleted: {
        console.log("WaverTheme loaded")
    }
}
