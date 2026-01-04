import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// Define the PianoKeyboard as a reusable component
Item {
    id: piano
    width: parent.width  // This will make it expand proportionally with the parent width
    height: 30

    WaverTheme {
        id: theme
    }

    // White keys layout
    RowLayout {
        id: whiteKeys
        spacing: 0
        anchors.fill: parent

        // White keys (C, D, E, F, G, A, B, etc.)
        Repeater {
            model: 7  // White keys for C, D, E, F, G, A, B

            WaverPianoWhiteKey {
                id: whiteKey
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

    }

    // Black keys layout (C#, D#, F#, G#, A#)
    Repeater {
        id: blackKeys
        anchors.fill: parent
        z: 1
        model: 5  // Black keys for C#, D#, F#, G#, A#

        WaverPianoBlackKey {
            id: blackKey

            property real whiteKeyWidth: whiteKeys.width / 7

            width: whiteKeyWidth * 0.6 // Black keys are narrower
            height: piano.height * 0.6 // Black keys are shorter

            // Set the X position of each black key explicitly
            x: {
                // This defines the position based on white keys
                var whiteKeyIndex = [0, 1, 3, 4, 5, 7, 8]; // Indices of white keys where black keys should go (C#, D#, F#, G#, A#)
                var xPosition = whiteKeyIndex[index] * whiteKeyWidth + whiteKeyWidth - (blackKey.width / 2);
                return xPosition;
            }
        }
        
    }

}
