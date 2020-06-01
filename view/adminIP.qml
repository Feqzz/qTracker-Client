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

    property var currentIP;
    property var currentSearchBool:true;

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


        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: navBar.verticalCenter
            anchors.verticalCenterOffset: 96
            TextField {
                id: searchField
                //anchors.horizontalCenter: parent.horizontalCenter
                focus: true
                width: 300
                height: 40
                text: qsTr("")
                placeholderText: "Search"
                selectByMouse: true
                onTextChanged: {
                    ipListModel.applyFilter(text);
                }
            }

            C.PushButton {
                id: searchToggle
                height: 40
                text: "IP Search"
                onClicked: {
                    if(currentSearchBool){
                        this.text = "User Search";
                    } else {
                        this.text = "IP Search"
                    }
                    currentSearchBool = !currentSearchBool;
                }
            }
        }

        ListModel {
            id: ipListModel
            function applyFilter(text) {
                var userList;
                if(currentSearchBool){
                    userList = adminHandler.getIPsByIP(text);
                }else{
                    userList = adminHandler.getIPsByUser(text);
                }
                ipListModel.clear();
                for (var userId in userList) {
                    var valueList = userList[userId];
                    append(createListElement(userId, valueList));
                }
            }

            function createListElement(userId, valueList) {
                return {
                    id: userId,
                    user: valueList[2],
                    ipa: valueList[0],
                    isbanned: valueList[1]

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
                role: "user"
                title: "User"
                width: 333
            }
            TableViewColumn {
                role: "ipa"
                title: "IP Address"
                width: 333
            }

            TableViewColumn {
                role: "isbanned"
                title: "Is Banned"
                width: 333
            }
            onClicked:  {
                if (ipListModel.get(currentRow)) {
                    currentIP = ipListModel.get(currentRow)
                } else {
                    currentIP = null;
                }
            }

            model: ipListModel
        }



        Row {
            id: buttonRow
            spacing: 32
            C.PushButton {
                id: banUser
                text: "Ban"
                onClicked: {
                    adminHandler.changeIPPrivilege(currentIP.id, 1);
                    ipListModel.applyFilter(searchField.text);
                    userTableView.focus = true;
                }
            }

            C.PushButton {
                id: unbanUser
                text: "Unban"
                onClicked: {
                    adminHandler.changeIPPrivilege(currentIP.id, 0);
                    ipListModel.applyFilter(searchField.text);
                    userTableView.focus = true;
                }
            }

        }
    }

    Component.onCompleted: {
        ipListModel.applyFilter(searchField.text);
        setTitle("qTracker :: Admin");
    }
}
