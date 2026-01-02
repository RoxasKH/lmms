import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: blackKey
    
    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: blackKey.down ? theme.colors.activeKeyStart : theme.colors.blackKeyStart }
            GradientStop { position: 1.0; color: blackKey.down ? theme.colors.activeKeyStop : theme.colors.blackKeyStop }
        }
        border.color: theme.colors.keyBorder
    }

    onClicked: {
        console.log("Black key", index, "pressed")
    }
}
