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
    property var currentTorrent;
    property var torrentModel: false;

    function refreshButtons(user) {
        banUser.enabled = false;
        unbanUser.enabled = false;
        promoteUser.enabled = false;
        demoteUser.enabled = false;
        removeButton.enabled = false;

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
        anchors.horizontalCenter: navBar.horizontalCenter
        //anchors.verticalCenter: navBar.verticalCenter
        //anchors.verticalCenterOffset: 96
        y: 300
        spacing: 32

        Row {
            id: labelsRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 32

            Label {
                id: userSearchLabel
                color: maUserSearchLabel.containsMouse ? "#d0d046" : "#ffffff"
                text: "User Search"
                MouseArea {
                    id: maUserSearchLabel
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: {
                        torrentModel = false;
                        torrentTableView.visible = false;
                        userTableView.visible = true;
                        userListModel.applyFilter(searchField.text)

                    }
                }
            }

            Label {
                id: torrentSearchLabel
                color: maTorrentSearchLabel.containsMouse ? "#d0d046" : "#ffffff"
                text: "Torrent Search"
                MouseArea {
                    id: maTorrentSearchLabel
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: {
                        banUser.enabled = false;
                        unbanUser.enabled = false;
                        promoteUser.enabled = false;
                        demoteUser.enabled = false;
                        torrentModel = true;
                        userTableView.visible = false;
                        torrentTableView.visible = true;
                        torrentListModel.applyFilter(searchField.text)


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
                if (torrentModel) {
                    torrentListModel.applyFilter(text);
                } else {
                    userListModel.applyFilter(text);
                }
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

        ListModel {
            id: torrentListModel
            function applyFilter(text) {
                var torrentList = adminHandler.getTorrentsByTitle(text);
                torrentListModel.clear();
                for (var torrentId in torrentList) {
                    var valueList = torrentList[torrentId];
                    append(createListElement(torrentId, valueList));
                }
            }

            function createListElement(torrentId, valueList) {
                return {
                    id: torrentId,
                    title: valueList[0],
                    infoHash: valueList[1],
                    uploadDate: valueList[2],
                    uploader: valueList[3]

                }
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
                refreshButtons(currentUser);
                removeButton.enabled = true;
            }

            model: userListModel
        }

        TableView {
            id: torrentTableView
            width: 1000
            height: 500
            visible: false
            TableViewColumn {
                role: "title"
                title: "Title"
                width: 350
            }
            TableViewColumn {
                role: "infoHash"
                title: "info_hash"
                width: 250
            }
            /*TableViewColumn {
                role: "completed"
                title: "Completed"
                width: 142
            }
            TableViewColumn {
                role: "seeders"
                title: "Seeders"
                width: 142
            }
            TableViewColumn {
                role: "leechers"
                title: "Leechers"
                width: 142
            }*/
            TableViewColumn {
                role: "uploadDate"
                title: "Uploaded"
                width: 200
            }
            TableViewColumn {
                role: "uploader"
                title: "Uploader"
                width: 200
            }
            onClicked:  {
                if (torrentListModel.get(currentRow)) {
                     currentTorrent = torrentListModel.get(currentRow)
                } else {
                    currentTorrent = null;
                }
                removeButton.enabled = true;
            }

            model: torrentListModel
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
                    userListModel.applyFilter(searchField.text);
                    userTableView.focus = true;
                    refreshButtons(currentUser);
                }
            }

            C.PushButton {
                id: promoteUser
                text: "Promote"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 1);
                    userListModel.applyFilter(searchField.text);
                   userTableView.focus = true;
                    refreshButtons(currentUser);
                }
            }

            C.PushButton {
                id: demoteUser
                text: "Demote"
                onClicked: {
                    adminHandler.changeUserPrivilege(currentUser.id, 0);
                    userListModel.applyFilter(searchField.text);
                    userTableView.focus = true;
                    refreshButtons(currentUser);
                }
            }

            C.PushButton {
                id: removeButton
                text: "Remove"
                onClicked: {
                    if (torrentModel) {
                        var deleteSuccess = adminHandler.removeTorrent(currentTorrent.id);
                        //console.log("torrent deletion: "+deleteSuccess);
                        torrentListModel.applyFilter(searchField.text);
                        torrentTableView.focus = true;
                    } else {
                        adminHandler.removeUser(currentUser.id);
                        userListModel.applyFilter(searchField.text);
                        userTableView.focus = true;
                    }
                    refreshButtons(null);
                }
            }
        }
    }

    Component.onCompleted: {
        userListModel.applyFilter(searchField.text);
        refreshButtons(null);
        setTitle("qTracker :: Admin");
    }
}
