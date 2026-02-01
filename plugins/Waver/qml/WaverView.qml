import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15 // Import Layouts module
import Lmms.Visualizers 1.0

Rectangle {

    // Create an instance of WaverTheme
    WaverTheme {
        id: theme
    }

    anchors.fill: parent
    color: theme.colors.background

    // Use ColumnLayout for vertical layout
    ColumnLayout {
        id: column
        anchors.fill: parent
        anchors.margins: theme.spacing.m

        WaverContainer {
            leftPadding: 20
            rightPadding: 20

            FontLoader {
                id: ava
                source: "ava.ttf"
            }

            WaverText {
                id: logo
                text: waverModel.instrument.name
                font.family: ava.name
                font.pointSize: 24
                font.bold: true
                Layout.fillWidth: true
            }
        }

        WaverContainer {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                WaverFrame {
                    title: qsTr("Sample")
                    Layout.fillHeight: true

                    content: ColumnLayout {
                        spacing: theme.spacing.l

                        WaverComboBox {
                            model: waverModel.instrument.sampleMap
                            textRole: "name"
                            onItemSelected: waverModel.instrument.selectSampleMap

                            selectedModel: waverModel.instrument.selectedSampleMap
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: theme.colors.containerDark

                            WaverGrid {
                                anchors.fill: parent
                                gridColumns: 10
                                gridRows: 4
                                gridColor: theme.colors.primaryDark
                            }
                            
                            WaverWaveform {
                                id: waveform
                                anchors.fill: parent

                                map: waverModel.instrument.selectedSampleMap
                                color: theme.colors.primary
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: theme.spacing.s

                            WaverKnob {
                                label: qsTr("Knob")
                                Layout.alignment: Qt.AlignTop
                            }

                            WaverKnob {
                                label: qsTr("Super cool other knob")
                                Layout.alignment: Qt.AlignTop
                            }

                            WaverButton {
                                text: qsTr("Load file")  // Custom title
                                // Custom click action (when the button is clicked)
                                onClicked: {
                                    console.log("Load file button clicked!")
                                    waverModel.openFiles()
                                    // Additional code or actions you want to trigger on click
                                }
                                Layout.alignment: Qt.AlignTop
                            }
                        }
                    }
                }

                WaverFrame {
                    title: qsTr("Sample map")
                    
                    content: ColumnLayout {
                        WaverGrid {
                            height: 100
                            gridColumns: 120
                            gridRows: 2
                        }
                        WaverPiano {}
                    }
                }
            }
            
        }

    }
}
