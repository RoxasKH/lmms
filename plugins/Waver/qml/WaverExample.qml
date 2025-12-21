import QtQuick 2.15
import QtQuick.Controls 2.15

// Define a reusable custom button component
Item {
    id: customButton

    // Define properties for the button
    property string text: "Default Title"  // Title of the button

    Text {
        text: parent.text
        color: "black"
        font.bold: true
        font.pointSize: 18
    }

}
