import QtQuick 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: control
    model: ["First", "Second", "Third"]
    padding: theme.spacing.xs

    property var onItemSelected
    property var selectedModel

    currentIndex: model.indexOf(selectedModel)

    WaverTheme {
        id: theme
    }

    // QT5 way of ItemDelegate
    delegate: ItemDelegate {
        id: delegate

        width: control.width

        contentItem: WaverText {
            // Not great as it forces you to have a name property
            text: modelData.name
            color: highlighted ? theme.colors.primary : theme.colors.text
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        // Change background if highlighted
        background: Rectangle {
            color: highlighted ? theme.colors.border : "transparent"
            radius: 2
        }

        highlighted: control.highlightedIndex === index

        onClicked: {
            console.log("Setting sampleMap", modelData.name, "at index", index);
            // Call cpp select to change the model
            onItemSelected(modelData);
        }
    }

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 12
        contextType: "2d"

        Connections {
            target: control.popup
            function onVisibleChanged() { canvas.requestPaint(); }
        }

        onPaint: {
            context.reset();
            if (control.popup.visible) {
                context.moveTo(0, height / 3);
                context.lineTo(width, height / 3);
                context.lineTo(width / 2, height * (5/6));
                context.closePath();
            } else {
                context.moveTo(width, 0);
                context.lineTo(width, height);
                context.lineTo(width / 2, height / 2);
                context.closePath();
            }
            context.fillStyle = theme.colors.content
            context.fill();
        }
    }

    contentItem: WaverText {
        leftPadding: control.leftPadding
        rightPadding: control.indicator.width + control.rightPadding

        text: control.displayText
        font: control.font
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        gradient: Gradient {
            GradientStop { position: 0.0; color: theme.colors.border }
            GradientStop { position: 1.0; color: theme.colors.container }
        }
        border.color: theme.colors.border;
        border.width: control.visualFocus ? 2 : 1
        radius: 10
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        height:  Math.min(contentItem.implicitHeight, waverModel.waverHeight - topMargin - bottomMargin)
        padding: 1

        contentItem: ListView {
            id: listView
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 2
            color: theme.colors.primary
            z: 1 // make sure it's above the ListView
        }

        background: Rectangle {
            color: theme.colors.container
            border.color: theme.colors.border
            radius: 2
        }
    }
}
