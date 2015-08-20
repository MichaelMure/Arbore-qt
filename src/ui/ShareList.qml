import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: root
    property ApplicationWindow app

    TextField {
        id: search
        height: 20
        z:1
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        placeholderText: qsTr("Search")
        anchors.right: parent.right
        anchors.left: parent.left
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 12
        style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 10
                    implicitWidth: 100
                    implicitHeight: 24
                    border.color: "#333"
                    border.width: 2
                }
            }
        onTextChanged: shareModel.setFilterRegExp(text)
    }

    ListView {
        id: shareList
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: search.bottom
        anchors.topMargin: 5

        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true

        model: shareModel

        delegate: Item {
            height: 81
            width: parent.width
            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    shareList.currentIndex = index;
                    app.details.share = shareModel.getShare(index);
                }
            }

            Image {
                id: image1
                width: 65
                height: 65
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.top: parent.top
                anchors.topMargin: 8
                fillMode: Image.PreserveAspectFit
                source: "contactIcon.png"
            }

            Text {
                id: text1
                height: 36
                text: name
                wrapMode: Text.WordWrap
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.left: image1.right
                anchors.leftMargin: 8
                font.pixelSize: 12
            }
        }
    }
}


