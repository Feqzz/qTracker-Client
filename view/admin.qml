import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as C

Rectangle {
    id: root
    width: 1366
    height: 768
    color: "#141414"

    C.NavBar {
        id: navBar
    }

    Column {
        id: column
        anchors.verticalCenterOffset: -150
        anchors.horizontalCenterOffset: 200
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
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
                        model.applyFilter(text);
                    }
                }


            }

            C.PushButton {
                id: searchButton

                height: 40
                text: "Search"
            }
        }

        ScrollView {
            id: scrollView
            clip: true
            width: 802
            height: 364

            ListView {
                id: listView
                width: 100; height: 100
                model: ListModel {
                    id: model

                    function applyFilter(name) {
                        var userList = adminHandler.getUsersByName(name);
                        console.log(userList);
                        model.clear();
                        for (var i = 0; i < userList.length; ++i) {
                            model.append(userList[i]);
                        }
                    }
                }

            }
        }
    }
}
