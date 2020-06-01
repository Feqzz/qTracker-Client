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

    property var currentTorrent;


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
                    torrentListModel.applyFilter(text);
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
            id: torrentTableView
            width: 1000
            height: 500
            visible: true
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
                id: removeButton
                text: "Remove"
                onClicked: {
                        var deleteSuccess = adminHandler.removeTorrent(currentTorrent.id);
                        //console.log("torrent deletion: "+deleteSuccess);
                        torrentListModel.applyFilter(searchField.text);
                        torrentTableView.focus = true;
                }
            }
        }
    }

    Component.onCompleted: {
        torrentListModel.applyFilter(searchField.text)
        setTitle("qTracker :: Admin");
    }
}
