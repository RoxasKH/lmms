import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15  // Import Layouts module

ColumnLayout {
    anchors.fill: parent

    // Create an instance of WaverTheme
    WaverTheme {
        id: theme
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: theme.backgroundColor

        // Use ColumnLayout for vertical layout
        ColumnLayout {
            id: column
            anchors.fill: parent
            anchors.margins: 20

            // Example of a button that will resize based on the layout
            Button {
                text: "Click Me"
                Layout.fillWidth: true
                onClicked: console.log("Button clicked!")
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

            WaverText {
                text: "Text text test text"
                Layout.fillWidth: true
            }

            // Other QML elements
            Text {
                text: waverModel.name
                Layout.fillWidth: true
            }
        }
    }

}

