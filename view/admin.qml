import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4
import "../component" as C

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#141414"

    C.NavBar {
        id: navBar
    }

    property var currentUser;

    function refreshButtons(user) {
        banUser.enabled = false;
        unbanUser.enabled = false;
        promoteUser.enabled = false;
        demoteUser.enabled = false;

        if (user) {
            if (user.privilege > -1) {
                banUser.enabled = true;
                if (user.privilege > 0) {
                    demoteUser.enabled = true;
                } else {
                    promoteUser.enabled = true;
                }
            } else {
                unbanUser.enabled = true;
            }
       }
    }

    Column {
        id: column
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 96
        spacing: 32

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

        TextField {
            id: searchField
            focus: true
            width: 300
            height: 40
            text: qsTr("")
            placeholderText: "Search"
            selectByMouse: true
            onTextChanged: listModel.applyFilter(text);
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
            id: tableView
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
                if (listModel.get(currentRow)) {
                     currentUser = listModel.get(currentRow)
                } else {
                    currentUser = null;
                }
                refreshButtons(currentUser);
            }

            model: listModel
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
                    listModel.applyFilter(searchField.text);
                    tableView.focus = true;
                    refreshButtons(currentUser);
                }
                enabled: false
            }

            C.PushButton {
                id: unbanUser
                text: "Unban"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 0);
                    adminHandler.changeLeechingPrivilege(currentUser.id, 1);
                    listModel.applyFilter(searchField.text);
                    tableView.focus = true;
                    refreshButtons(currentUser);
                }
            }

            C.PushButton {
                id: promoteUser
                text: "Promote"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 1);
                    listModel.applyFilter(searchField.text);
                    tableView.focus = true;
                    refreshButtons(currentUser);
                }
            }

            C.PushButton {
                id: demoteUser
                text: "Demote"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 0);
                    listModel.applyFilter(searchField.text);
                    tableView.focus = true;
                    refreshButtons(currentUser);
                }
            }

            C.PushButton {
                id: removeUser
                text: "Remove"
                onClicked: {
                    adminHandler.removeUser(currentUser.id);
                    listModel.applyFilter(searchField.text);
                    tableView.focus = true;
                    refreshButtons(null);
                }
            }
        }
    }
    Component.onCompleted: {
        listModel.applyFilter(searchField.text);
        refreshButtons(null);
    }
}
