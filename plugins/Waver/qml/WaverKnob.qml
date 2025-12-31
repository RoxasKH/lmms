import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: knob

    property string label: ""

    WaverTheme {
        id: theme
    }

    Dial {
        id: knobDial

        Layout.alignment: Qt.AlignCenter
        inputMode: Dial.Vertical

        onMoved: canvas.requestPaint()

        background: Rectangle {
            id: knobBackground
            implicitWidth: 40
            implicitHeight: 40

            color: "transparent"

            Rectangle {
                id: knobCenter
                anchors.fill: parent
                anchors.margins: 3
                x: knobDial.width / 2 - width / 2
                y: knobDial.height / 2 - height / 2
                width: Math.max(26, Math.min(knobDial.width, knobDial.height))
                height: width
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#474953" }
                    GradientStop { position: 1.0; color: "#23242A" }
                }
                radius: width / 2
                border.color: "#0E1014"
                opacity: knobDial.enabled ? 1 : 0.3

                transform: Scale { 
                    origin.x: knobBackground.width / 2.0; 
                    origin.y: knobBackground.height / 2.0; 
                    xScale: knobDial.pressed ? 0.8 : 0.9; 
                    yScale: knobDial.pressed ? 0.8 : 0.9;
                }
            }

            Canvas {
                id: canvas
                anchors.fill: parent
                property int lineThickness: 2
                property int dialRotation: 140
                // we rotate the canvas to the same angle of the dial, which starts at -140 and has it's 0 point at the top, so an additional -90
                rotation: - (dialRotation + 90)
                // set higher z to stay on top of the center
                z: 1 
                onPaint: {
                    var ctx = getContext("2d");
                    var x = canvas.width / 2;
                    var y = canvas.height / 2;
                    var radius = canvas.width / 2 - lineThickness;

                    // Clear before
                    ctx.clearRect(0, 0, canvas.width, canvas.height);

                    // dial starts at -140, so we add it up; arc only draws positive values
                    var normalizedAngle = knobDial.angle + dialRotation;
                    var radiansDrawAngle = normalizedAngle * Math.PI / 180;

                    var trackRadiansDrawAngle = 280 * Math.PI / 180;

                    ctx.lineWidth = lineThickness;
                    
                    ctx.lineCap = "round";
                    ctx.strokeStyle = "#474953";
                    ctx.beginPath();
                    ctx.arc(x, y, radius, 0, trackRadiansDrawAngle);
                    ctx.stroke();

                    ctx.strokeStyle = "#1BE073";
                    ctx.beginPath();
                    ctx.arc(x, y, radius, 0, radiansDrawAngle);
                    ctx.stroke();
                }
            }

        }

        handle: Rectangle {
            id: handleItem
            x: knobDial.background.x + knobDial.background.width / 2 - width / 2
            y: knobDial.background.y + knobDial.background.height / 2 - height / 2
            width: 3
            height: knobDial.height / 4
            color: knobDial.pressed ? "#1BE073" : "#ABB1BD"
            radius: 8
            antialiasing: true
            opacity: knobDial.enabled ? 1 : 0.3
            transform: [
                Translate {
                    y: -Math.min(knobDial.background.width, knobDial.background.height) * 0.4 + handleItem.height / 2
                },
                Rotation {
                    angle: knobDial.angle
                    origin.x: handleItem.width / 2
                    origin.y: handleItem.height / 2
                },
                Scale { 
                    origin.x: handleItem.width / 2.0; 
                    origin.y: handleItem.height / 2.0; 
                    xScale: knobDial.pressed ? 0.8 : 0.9; 
                    yScale: knobDial.pressed ? 0.8 : 0.9;
                }
            ]
        }
    }

    WaverText {
        id: knobLabel

        Layout.maximumWidth: 60
        wrapMode: Text.WordWrap

        text: knob.label
        font.pointSize: 8

        horizontalAlignment: Text.AlignHCenter

        Layout.alignment: Qt.AlignCenter
    }
}
