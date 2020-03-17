import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as C

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#141414"

    C.NavBar{
        id: navBar
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
         anchors.fill: flick
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
            var torrentList = torrentHandler.getTorrentsByName(text);
            torrentModel.clear();
            for (var title in torrentList) {
                var valueList = torrentList[title];
                append(createListElement(title, valueList));
            }
        }

        function createListElement(userId, valueList) {
            return {
                title: userId,
                uploaderUsername: valueList[0],
                leechers: valueList[1],
                seeders: valueList[2],
                completed: valueList[3],
                uploadDate: valueList[4]
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

                    Text {
                        id: uploadDateText
                        text: uploadDate
                        font.pixelSize: 15
                        color: "white"
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "../images/download.png"
                        width: 32
                        height: 32
                        //onClicked: fruitModel.setProperty(index, "cost", cost + 0.25)
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
