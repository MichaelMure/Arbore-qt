import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Rectangle {
    color: "#7b7c7e"

    ListView {
        id: menuList
        interactive: false
        anchors.fill: parent

        highlight: Rectangle {
            color: "#7b7c7e"

            Rectangle {
                id: rectangle1
                color: "#4b4b4d"
                anchors.fill: parent
                anchors.leftMargin: 40

                Rectangle {
                    width: parent.height
                    height: parent.height
                    color: "#4b4b4d"
                    radius: 25
                    z: -1
                    anchors.horizontalCenter: parent.left
                }
            }
        }
        focus: true

        model: ListModel {
            ListElement {
                name: "Share list"
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
                element: 3
                filter: "blah"
            }
            ListElement {
                name: "Active shares"
                element: 4
                filter: "bluh"
            }
            ListElement {
                name: "Starred"
                element: 4
                filter: "bluh"
            }
        }

        delegate: Text {
            id: menuLabel
            width: parent.width
            height: 36
            font.pointSize: 15
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    menuList.currentIndex = index;
                    // Todo something with filter
                }
            }

            Row {
                id: row1
                spacing: 5
                Image {
                    id: icon
                    fillMode: Image.PreserveAspectFit
                    source: "plus.png"
                }

                Text {
                    id: text
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "Courier"
                    font.pointSize: 15

                }

                Text {
                    text: element
                    anchors.verticalCenter: parent.verticalCenter

                    Rectangle {
                        color: '#fa4f00'
                        radius: 12
                        anchors.fill: parent
                        z: -1
                    }
                }
            }
        }
    }
}

