import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as C

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#141414"

    C.NavBar {
        id: navBar
    }

    Column {
        id: column
        anchors.verticalCenterOffset: 0
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: navBar.horizontalCenter
        anchors.verticalCenter: navBar.verticalCenter
        spacing: 8

        Row {
            id: labelsRow
            spacing: 32

            Label {
                id: userSearchLabel
                color: "#ffffff"
                text: qsTr("User Search")
            }

            Label {
                id: torrentSearchLabel
                color: "#ffffff"
                text: qsTr("Torrent Search")
            }
        }

        Row {
            id: searchRow

            TextField {
                id: searchField
                focus: true
                width: 300
                height: 40
                text: qsTr("")
                placeholderText: "Search"
                selectByMouse: true

                onTextChanged: {
                    //timer.restart();
                    if (text.length > 0) {
                        listModel.applyFilter(text);
                    }
                }


            }

            /*C.PushButton {
                id: searchButton

                height: 40
                text: "Search"
            }*/
        }

        ScrollView {
            id: scrollView
            clip: true
            width: 802
            height: 364

            ListView {
                id: listView
                width: 100; height: 100

                model: listModel
                delegate: Component {
                    Item{
                        width: 180; height: 40
                        Column{
                            Text{
                                text: '<b>Name:</b> ' + name
                                color:"white"
                            }
                            Text{
                                text: '<b>Privilege:</b> ' + privlege
                                color:"white"
                            }
                        }
                        MouseArea{
                            hoverEnabled: true
                            anchors.fill: parent
                            onClicked: {
                                //var newPrivilege = adminHandler.
                                console.log(id+" "+privlege);
                            }
                        }}
                }
            }
            ListModel {
                id: listModel
                function applyFilter(text) {
                    var userList = adminHandler.getUsersByName(text);
                    listModel.clear();
                    for (var userId in userList) {
                        var valueList = userList[userId];
                        append(createListElement(userId,valueList));
                    }
                }

                function createListElement(userId,valueList) {
                    return {
                        id:userId,
                        name: valueList[0],
                        privlege: valueList[1]
                    };
                }
            }
        }
    }
}
