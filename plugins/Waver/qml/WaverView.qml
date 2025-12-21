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
        onClicked: console.log("Another button clicked!")
    }

    WaverButton {
        Layout.fillWidth: true  // This will make WaverButton fill the width of the layout
        text: "Settings"  // Custom title
        iconSource: ""  // Custom icon path
        // Custom click action (when the button is clicked)
        onClicked: {
            console.log("Settings button clicked!")
            // Additional code or actions you want to trigger on click
        }
    }

    WaverExample {
        text: "Text text test text"
        Layout.fillWidth: true
    }

    // Other QML elements
    Text {
        text: "This text will also adjust"
        Layout.fillWidth: true
    }
}
