import QtQuick 2.0
import QtQuick.Controls 1.3

TextArea {

    property string placeholder: "Description"

    Text {
        anchors.fill: parent
        font: parent.font
        text: parent.text ? "" : parent.placeholder
        anchors.leftMargin: 5
        anchors.topMargin: 5
        color: "darkgray"
    }
}
