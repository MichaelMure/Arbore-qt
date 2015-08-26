import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    ListView {
        id: shareList
        width: 400
        height: 400
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        spacing: 10
        anchors.fill: parent

        highlight: Rectangle {
            color: "lightsteelblue";
            radius: 5
            anchors.leftMargin: 30
        }
        focus: true

        model: ListModel {
            ListElement {
                name: "News"
                element: 3
                filter: "blah"
            }
            ListElement {
                name: "Inbox"
                element: 8
                filter: "bloh"
            }
            ListElement {
                name: "My shares"
                element: 4
                filter: "bluh"
            }
        }

        delegate: Text {
                id: menuLabel
                text: name
                font.pointSize: 15
                anchors.left: parent.left
                anchors.leftMargin: 30
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        // Todo something with filter
                    }
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    source: "plus.png"
                    anchors.left: parent.left
                    anchors.leftMargin: -30
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }

                Text {
                    anchors.left: parent.right
                    anchors.leftMargin: 5
                    text: element
                    font.pointSize: 15
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    Rectangle {
                        anchors.fill: parent
                        color: "blue"
                        radius: 9
                        z: -1
                    }
                }
        }
    }
}

