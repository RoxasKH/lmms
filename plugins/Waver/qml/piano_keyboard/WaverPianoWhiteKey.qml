import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: whiteKey

    WaverTheme {
        id: theme
    }
    
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
