import QtQuick 2.12
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import "../component" as C

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#141414"

    C.NavBar{
        id: navBar
    }

    property var publicTorrentId;
    property var publicTorrrentTitle;

    FileDialog {
        id: fileDialog
        currentFile: "file:///" + publicTorrrentTitle + ".torrent"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        //selectedNameFilter.index: 0
        fileMode: FileDialog.SaveFile
        nameFilters: ["Torrent files (*.torrent)"]
        onAccepted: {
            //console.log(currentFile);
            torrentHandler.downloadFile(publicTorrentId, user.getTorrentPass(), currentFile);
        }
    }

    Text {
        id: torrentSearchText
        anchors.verticalCenter: root.verticalCenter
        anchors.horizontalCenter: root.horizontalCenter
        anchors.verticalCenterOffset: -300
        text: qsTr("Torrent Search")
        font.pixelSize: 32
        color: "#ffffff"
    }

    TextField {
        id: searchField
        anchors.verticalCenter: root.verticalCenter
        anchors.horizontalCenter: root.horizontalCenter
        anchors.verticalCenterOffset: -220
        focus: true
        width: 300
        height: 40
        text: qsTr("")
        placeholderText: "Search"
        selectByMouse: true
        onTextChanged: torrentModel.applyFilter(text);
    }

     ListView {
         id: listView

         model: torrentModel
         spacing: 16
         delegate: listDelegate
         anchors.verticalCenter: root.verticalCenter
         anchors.horizontalCenter: root.horizontalCenter
         anchors.verticalCenterOffset: 150
         clip: true
         width: 1000
         height: 600

         section.property: "size"
         section.criteria: ViewSection.FullString

         ScrollBar.vertical: ScrollBar {
             active: true
             //policy: ScrollBar.AlwaysOn
         }
     }

    ListModel {
        id: torrentModel
        function applyFilter(text) {
            var torrentList = torrentHandler.getTorrentsByName(text, user.getId());
            torrentModel.clear();
            for (var title in torrentList) {
                var valueList = torrentList[title];
                append(createListElement(title, valueList));
            }
        }

        function createListElement(title, valueList) {
            return {
                title: title,
                uploaderUsername: valueList[0],
                leechers: valueList[1],
                seeders: valueList[2],
                completed: valueList[3],
                uploadDate: valueList[4],
                torrentId: valueList[5],
                isDownloaded: valueList[6],
                isSeeding: valueList[7]
            };
        }
    }

         // The delegate for each section header

    Component {
            id: listDelegate

            Item {
                id: delegateItem
                width: listView.width; height: 55
                clip: true

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10

                    Column {
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
                            id: titleText
                            text: title
                            font.pixelSize: 24
                            color: "white"
                        }

                        Text {
                            id: uploaderText
                            text: uploaderUsername
                            font.pixelSize: 15
                            color: "#c2a800"
                        }
                    }
                }

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    spacing: 10

                    Text {
                        id: completedText
                        text: completed
                        font.pixelSize: 15
                        color: "white"
                    }

                    Text {
                        id: seedersText
                        text: seeders
                        font.pixelSize: 15
                        color: "white"
                    }

                    Text {
                        id: leechersText
                        text: leechers
                        font.pixelSize: 15
                        color: "white"
                    }

                    /*Text {
                        id: uploadDateText
                        text: uploadDate
                        font.pixelSize: 15
                        color: "white"
                    }*/

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: {
                            if (isDownloaded == 1) { //Did not work without == 1
                                if (isSeeding == 1) {
                                    "../images/downloadYellow.png"
                                } else {
                                    "../images/downloadRed.png"
                                }
                            } else {
                                "../images/download.png"
                            }
                        }
                        width: maDownloadImage.containsMouse ? 34 : 32
                        height: maDownloadImage.containsMouse ? 34 : 32

                        MouseArea {
                            id: maDownloadImage
                            anchors.fill: parent
                            onClicked: {
                                publicTorrrentTitle = title;
                                publicTorrentId = torrentId;
                                fileDialog.open();
                                //torrentHandler.downloadFile(torrentId, user.getTorrentPass());
                            }
                        }
                    }

                    Text {
                        id: emptyText //Need some empty space for the scrollbar
                        text: "   "
                        font.pixelSize: 15
                        color: "white"
                    }
                }
            }
        }

         Component.onCompleted: {
             torrentModel.applyFilter("");
         }
}
