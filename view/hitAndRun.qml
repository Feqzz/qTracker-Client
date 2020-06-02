import QtQuick 2.12
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import "../component" as C

Rectangle {
    id: root
    anchors.fill: parent
    color: "#141414"

    C.NavBar{
        id: navBar
    }

    function convertMinutes(minutes) {
        var d = Math.floor(minutes/1440);
        var h = Math.floor((minutes-(d*1440))/60);
        var m = Math.round(minutes%60);
        if (d>0) {
            return (d+"d"+h+"h");
        } else {
            return (h+"h"+m+"m");
        }
    }

    ListView {
        id: listView
        model: torrentModel
        spacing: 16
        delegate: listDelegate
        anchors.horizontalCenter: parent.horizontalCenter
        y: 300
        clip: true
        width: 1400
        height: 600
        headerPositioning: ListView.OverlayHeader
           header: Item {
               height: 40
               anchors.left: parent.left
               anchors.right: parent.right
               Row {
                   anchors.fill: parent

                   spacing: 20
                   Text {
                       horizontalAlignment: Text.AlignLeft
                       width: 350
                       text: "Title"
                       color: "white"
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       text: "Uploaded"
                       color: "white"
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       text: "Downloaded"
                       color: "white"
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignLeft
                       text: "Ratio"
                       color: "white"
                       width: 40
                   }

                   Image {
                       horizontalAlignment: Text.AlignLeft
                       source: "../images/downloadWhiteUp.png"
                       width: 16
                       height: 16
                   }

                   Image {
                       horizontalAlignment: Text.AlignLeft
                       source: "../images/downloadWhiteDown.png"
                       width: 16
                       height: 16
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       text: "Last Action"
                       color: "white"
                       width: 200
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       text: "Seeding"
                       color: "white"
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       text: "Seed Time Left"
                       color: "white"
                       width: 120
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       text: "Seed Time"
                       color: "white"
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       text: "HnR"
                       color: "white"
                       width: 60
                   }
               }
           }

        section.property: "size"
        section.criteria: ViewSection.FullString

        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }

   ListModel {
       id: torrentModel
       function applyFilter(text) {
           var torrentList = hitAndRunHandler.getTorrents(user.getId());
           torrentModel.clear();
           for (var title in torrentList) {
               var valueList = torrentList[title];
               append(createListElement(title, valueList));
           }
       }

       function createListElement(title, valueList) {
           return {
               title: title,
               uploaded: valueList[0],
               downloaded: valueList[1],
               ratio: valueList[2],
               seeders: valueList[3],
               leechers: valueList[4],
               lastActivity: valueList[5],
               isActive: valueList[6],
               seedTimeLeft: valueList[7],
               timeActive: valueList[8],
               hnr: valueList[9]
           };
       }
   }

   Component {
           id: listDelegate

           Item {
               id: delegateItem
               width: listView.width; height: 55
               clip: true

               Row {
                   anchors.verticalCenter: parent.verticalCenter
                   anchors.right: listView.right
                   spacing: 20

                   Text {
                       id: titleText
                       text: title
                       font.pixelSize: 16
                       color: "white"
                       width: 350
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: uploadedText
                       text: getGiPrefix(uploaded)
                       font.pixelSize: 16
                       color: "white"
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: downloadedText
                       text: getGiPrefix(downloaded)
                       font.pixelSize: 16
                       color: "white"
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: ratioText
                       text: (uploaded > 0 && downloaded == 0) ? "Inf" : ratio.toFixed(2)
                       font.pixelSize: 16
                       color: {
                           if (uploaded > 0 && downloaded == 0) {
                               "gainsboro"
                           }
                           else if (ratio >= 0.5) {
                               if (ratio >= 0.75 && ratio < 1) {
                                   "orange"
                               } else {
                                   "gainsboro"
                               }
                           } else {
                               "#e90000"
                           }
                        }
                        width: 40
                    }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: seedersText
                       text: seeders
                       font.pixelSize: 16
                       color: "white"
                       width: 16
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: leechersText
                       text: leechers
                       font.pixelSize: 16
                       color: "white"
                       width: 16
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: lastActivityText
                       text: lastActivity.toString().substr(0, 10) + " " + lastActivity.toString().substr(11, 12)
                       font.pixelSize: 16
                       color: "white"
                       width: 200
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: isActiveText
                       text: {
                           if (isActive) {
                               "Yes"
                           } else {
                               "No"
                           }
                       }
                       font.pixelSize: 16
                       color: {
                           if (isActive) {
                               "#00e940"
                           } else {
                               "#e90000"
                           }
                       }
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: seedTimeLeftText
                       text: convertMinutes(seedTimeLeft)
                       font.pixelSize: 16
                       color: "white"
                       width: 120
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: timeActiveText
                       text: convertMinutes(timeActive)
                       font.pixelSize: 16
                       color: "white"
                       width: 100
                   }

                   Text {
                       horizontalAlignment: Text.AlignHCenter
                       id: hnrText
                       text: {
                           if (hnr) {
                               "Yes"
                           } else {
                               "No"
                           }
                       }
                       font.pixelSize: 16
                       color: {
                           if (!hnr) {
                               "#00e940"
                           } else {
                               "#e90000"
                           }
                       }
                       width: 60
                   }

                   Text {
                       id: emptyText //Need some empty space for the scrollbar
                       text: "   "
                       font.pixelSize: 16
                       color: "white"
                   }
               }
           }
       }

    Component.onCompleted: {
        torrentModel.applyFilter("");
        setTitle("qTracker :: Hit And Runs");
    }
}
