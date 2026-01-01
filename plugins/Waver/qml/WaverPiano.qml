import QtQuick 2.15
import QtQuick.Controls 2.15

// Define the PianoKeyboard as a reusable component
Item {
    id: piano
    width: parent.width  // This will make it expand proportionally with the parent width
    height: 25

    // Colors for the keys
    property color whiteKeyColor: "white"
    property color blackKeyColor: "black"
    property real whiteKeyWidth: parent.width / 7  // Divide width into 7 parts for keys
    property real blackKeyWidth: parent.width / 12 //whiteKeyWidth * 0.6  // Black keys will be a bit smaller

    // White keys layout
    Row {
        spacing: 0
        anchors.fill: parent

        // White keys (C, D, E, F, G, A, B, etc.)
        Repeater {
            model: 7  // White keys for C, D, E, F, G, A, B
            Rectangle {
                width: piano.whiteKeyWidth
                height: piano.height
                color: piano.whiteKeyColor
                border.color: "black"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("White key", index, "pressed")
                    }
                }
            }
        }

    }

    // Black keys layout (C#, D#, F#, G#, A#)
    Repeater {
        anchors.fill: parent
        z: 1
        model: 7  // Black keys for C#, D#, F#, G#, A#
        Rectangle {

            property var validIndices: ({
                1: true,
                3: true,
                6: true,
                8: true,
                10: true
            })

            width: piano.blackKeyWidth
            height: piano.height * 0.6  // Black keys are shorter
            color: piano.blackKeyColor
            border.color: "black"

            // Set the X position of each black key explicitly
            x: {
                // This defines the position based on white keys
                var whiteKeyIndex = [0, 1, 3, 4, 5, 7, 8]; // Indices of white keys where black keys should go (C#, D#, F#, G#, A#)
                var xPosition = whiteKeyIndex[index] * piano.whiteKeyWidth + piano.whiteKeyWidth - (piano.blackKeyWidth / 2);
                console.log("key", index, "position", xPosition);
                return xPosition;
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Black key", index, "pressed")
                }
            }
        }
    }

}
