import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4
import "../component" as C

Rectangle {
    id: root
    //width: 1920
    //height: 1080
    anchors.fill: parent
    color: "#141414"

    C.NavBar {
        id: navBar
    }

    property var currentUser;

    Column {
        id: column
        anchors.horizontalCenter: navBar.horizontalCenter
        //anchors.verticalCenter: navBar.verticalCenter
        //anchors.verticalCenterOffset: 96
        y: 300
        spacing: 32

        Row {
            id: labelsRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 32

            C.YellowLabel {
                id: userSearchLabel
                text: "User Search"
                C.MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        changePage("adminUser")

                    }
                }
            }

            C.YellowLabel {
                id: ipSearchLabel
                text: "IP Search"
                C.MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        changePage("adminIP")

                    }
                }
            }

            C.YellowLabel {
                id: torrentSearchLabel
                text: "Torrent Search"
                C.MouseArea {
                    anchors.fill: parent
                    onClicked: {
                         changePage("adminTorrent")
                    }
                }
            }
        }

        TextField {
            id: searchField
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            width: 300
            height: 40
            text: qsTr("")
            placeholderText: "Search"
            selectByMouse: true
            onTextChanged: {
                    userListModel.applyFilter(text);
            }
        }

        ListModel {
            id: userListModel
            function applyFilter(text) {
                var userList = adminHandler.getUsersByName(text);
                userListModel.clear();
                for (var userId in userList) {
                    var valueList = userList[userId];
                    append(createListElement(userId, valueList));
                }
            }

            function createListElement(userId, valueList) {
                return {
                    id: userId,
                    username: valueList[0],
                    privilege: valueList[1],
                    canLeech: valueList[2]
                };
            }

        }



            /*
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
            */

            /*
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
            */

        TableView {
            id: userTableView
            width: 1000
            height: 500
            TableViewColumn {
                role: "username"
                title: "Username"
                width: 333
                }
            TableViewColumn {
                role: "privilege"
                title: "Privilege"
                width: 333
            }
            TableViewColumn {
                role: "canLeech"
                title: "Leeching privilege"
                width: 333
            }
            onClicked:  {
                if (userListModel.get(currentRow)) {
                     currentUser = userListModel.get(currentRow)
                } else {
                    currentUser = null;
                }
                removeButton.enabled = true;
            }

            model: userListModel
        }



        Row {
            id: buttonRow
            spacing: 32
            C.PushButton {
                id: banUser
                text: "Ban"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, -1);
                    adminHandler.changeLeechingPrivilege(currentUser.id, 0);
                    userListModel.applyFilter(searchField.text);
                    userTableView.focus = true;
                }
                enabled: false
            }

            C.PushButton {
                id: unbanUser
                text: "Unban"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 0);
                    adminHandler.changeLeechingPrivilege(currentUser.id, 1);
                    userListModel.applyFilter(searchField.text);
                    userTableView.focus = true;
                }
            }

            C.PushButton {
                id: promoteUser
                text: "Promote"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 1);
                    userListModel.applyFilter(searchField.text);
                   userTableView.focus = true;

                }
            }

            C.PushButton {
                id: demoteUser
                text: "Demote"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 0);
                    userListModel.applyFilter(searchField.text);
                    userTableView.focus = true;
                }
            }

            C.PushButton {
                id: removeButton
                text: "Remove"
                onClicked: {
                        adminHandler.removeUser(currentUser.id);
                        userListModel.applyFilter(searchField.text);
                        userTableView.focus = true;
                }
            }
        }
    }

    Component.onCompleted: {
        userListModel.applyFilter(searchField.text);
        setTitle("qTracker :: Admin");
    }
}
