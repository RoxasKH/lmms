import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15  // Import Layouts module

// Use ColumnLayout for vertical layout
ColumnLayout {
    anchors.fill: parent

    // Example of a button that will resize based on the layout
    Button {
        text: "Click Me"
        Layout.fillWidth: true
        onClicked: console.log("Button clicked!")
    }

    // Another button
    Button {
        text: "Another Button"
        Layout.fillWidth: true
        onClicked: {
            console.log("Another button clicked!")
        }
    }

    // Other QML elements
    Text {
        text: "This text will also adjust"
        Layout.fillWidth: true
    }
}
