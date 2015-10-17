import QtQuick 2.0
import QtQuick.Controls 1.3
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
        anchors.top: search.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 5

        focus: true

        model: shareModel

        delegate: Item {
            id: item1
            width: parent.width
            height: 90

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    shareList.currentIndex = index;
                    app.details.share = shareModel.getShare(index);
                }
            }

            Rectangle {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                anchors.topMargin: 2
                anchors.bottomMargin: 2
                z: -1
                color: shareList.currentIndex == index ? "white" : "#7b7c7e"
                radius: 5
            }

            Image {
                id: contactIcon
                width: 35
                height: 35
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.top: parent.top
                anchors.topMargin: 8
                fillMode: Image.PreserveAspectFit
                source: "images/bonhomme"
            }

            Text {
                id: contactName
                height: 18
                text: name
                color: shareList.currentIndex == index ? "#7b7c7e" : "white"
                wrapMode: Text.WordWrap
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.left: contactIcon.right
                anchors.leftMargin: 8
                font.pixelSize: 12
            }

            ProgressBar {
                y: 73
                height: 5
                value: progress
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5
                style: ProgressBarStyle {
                    background: Rectangle {
                        radius: 5
                        color: "#b0b2b3"
                    }
                    progress: Rectangle {
                        radius: 5
                        color: "#fa4c00"
                    }
                }
            }
        }
    }
}


