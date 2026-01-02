import QtQuick 2.15
import QtQuick.Controls 2.15

// Define the PianoKeyboard as a reusable component
Item {
    id: piano
    //width: parent.width  // This will make it expand proportionally with the parent width
    height: 30

    WaverTheme {
        id: theme
    }

    // Colors for the keys
    property real whiteKeyWidth: piano.width / 7  // Divide width into 7 parts for keys
    property real blackKeyWidth: whiteKeyWidth * 0.7  // Black keys will be a bit smaller

    // White keys layout
    Row {
        spacing: 0
        anchors.fill: parent

        // White keys (C, D, E, F, G, A, B, etc.)
        Repeater {
            model: 7  // White keys for C, D, E, F, G, A, B

            Button {
                id: whiteKey

                width: piano.whiteKeyWidth
                height: piano.height
                
                background: Rectangle {
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: whiteKey.down ? theme.colors.activeKeyStart : theme.colors.whiteKey }
                        GradientStop { position: 1.0; color: whiteKey.down ? theme.colors.activeKeyStop : theme.colors.whiteKey }
                    }
                    border.color: theme.colors.keyBorder
                }

                onClicked: {
                    console.log("White key", index, "pressed")
                }
            }
        }

    }

    // Black keys layout (C#, D#, F#, G#, A#)
    Repeater {
        anchors.fill: parent
        z: 1
        model: 5  // Black keys for C#, D#, F#, G#, A#

        Button {
            id: blackKey

            width: piano.blackKeyWidth
            height: piano.height * 0.6  // Black keys are shorter

            background: Rectangle {
                gradient: Gradient {
                    GradientStop { position: 0.0; color: blackKey.down ? theme.colors.activeKeyStart : theme.colors.blackKeyStart }
                    GradientStop { position: 1.0; color: blackKey.down ? theme.colors.activeKeyStop : theme.colors.blackKeyStop }
                }
                border.color: theme.colors.keyBorder
            }

            // Set the X position of each black key explicitly
            x: {
                // This defines the position based on white keys
                var whiteKeyIndex = [0, 1, 3, 4, 5, 7, 8]; // Indices of white keys where black keys should go (C#, D#, F#, G#, A#)
                var xPosition = whiteKeyIndex[index] * piano.whiteKeyWidth + piano.whiteKeyWidth - (piano.blackKeyWidth / 2);
                return xPosition;
            }

            onClicked: {
                console.log("Black key", index, "pressed")
            }
        }
        
    }

}
