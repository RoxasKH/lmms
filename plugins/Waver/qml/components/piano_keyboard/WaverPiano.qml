import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

RowLayout {
    id: piano
    spacing: 0

    property int octaveNumber: 10

    width: parent.width

    Repeater {
        model: piano.octaveNumber

        WaverPianoOctave {
            Layout.fillWidth: true
        }
    }

}
