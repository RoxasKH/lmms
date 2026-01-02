import QtQuick 2.15

// To use with pragma singleton in the future
QtObject {

    property QtObject colors: QtObject {
        property color primary: "#0ad45c"
        property color primaryLight: "#23f477"
        property color primaryDark: "#173b29"
        property color secondary: "#00aba2"
        property color background: "#090a0d"
        property color container: "#101215"
        property color containerDark: "#000000"
        property color border: "#272931"
        property color content: "#ffffff"
        property color contentDark: "#cbcbcb"
        property color text: "#ffffff"
        property color textDark: "#cbcbcb"

        property color keyBorder: "#1a1b1e"
        property color whiteKey: "#ffffff"
        property color blackKeyStart: "#25282e"
        property color blackKeyStop: "#393d47"
        property color activeKeyStart: "#3bc977"
        property color activeKeyStop: "#28bc66"
        property color baseKeyBorder: "#00cdc1"
        property color baseKey: "#00aba2"
    }

    property QtObject spacing: QtObject {
        property int xs: 5
        property int s: xs * 2
        property int m: xs * 3
        property int l: xs * 4
    }

    property QtObject typography: QtObject {
        property int body: 8
    }

}
