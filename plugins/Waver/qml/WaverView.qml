import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15 // Import Layouts module

Rectangle {

    // Create an instance of WaverTheme
    WaverTheme {
        id: theme
    }

    anchors.fill: parent
    color: theme.backgroundColor

    // Use ColumnLayout for vertical layout
    ColumnLayout {
        id: column
        anchors.fill: parent
        anchors.margins: 20

        WaverContainer {
            WaverText {
                text: waverModel.name
                Layout.fillWidth: true
            }
        }

        WaverContainer {
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent

                WaverButton {
                    text: "Settings"  // Custom title
                    iconSource: ""  // Custom icon path
                    // Custom click action (when the button is clicked)
                    onClicked: {
                        console.log("Settings button clicked!")
                        // Additional code or actions you want to trigger on click
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    WaverKnob {
                        label: qsTr("Knob")
                        Layout.alignment: Qt.AlignTop
                    }

                    WaverKnob {
                        label: qsTr("Super cool other knob")
                        Layout.alignment: Qt.AlignTop
                    }
                }
            }
        }

        

        WaverContainer {
            Layout.fillWidth: true

            WaverFrame {
                anchors.fill: parent

                title: qsTr("Sample map")
                
                content: ColumnLayout {
                    WaverGrid {}
                    //WaverPianoOctave {}
                    WaverPiano {}
                }
            }
        }

        WaverText {
            text: "Text text test text"
            Layout.fillWidth: true
        }

    }
}
