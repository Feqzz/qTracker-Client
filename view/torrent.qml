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

    ListModel {
         id: animalsModel
         ListElement { name: "Parrot"; size: "Small" }
         ListElement { name: "Guinea pig"; size: "Small" }
         ListElement { name: "Dog"; size: "Medium" }
         ListElement { name: "Cat"; size: "Medium" }
         ListElement { name: "Elephant"; size: "Large" }
    }


    ListModel {
        id: test
        function applyFilter(text) {
            var torrentList = torrentHandler.getTorrentsByName(text);
            listModel.clear();
            for (var title in torrentList) {
                var valueList = torrentList[title];
                append(createListElement(title, valueList));
            }
        }

        function createListElement(userId,valueList) {
            return {
                title: userId,
                UploaderUsername: valueList[0],
                leechers: valueList[1],
                seeders: valueList[2],
                completed: valueList[3],
                uploadDate: valueList[4]
            };
        }
    }

         // The delegate for each section header
    /*
         Component {
             id: sectionHeading
             Rectangle {
                 width: container.width
                 height: childrenRect.height
                 color: "lightsteelblue"

                 Text {
                     text: section
                     font.bold: true
                 }
             }
         } */

        ScrollView {
            id: flick
            clip: true
            width: 200
            height: 700
            contentWidth: -1
            anchors.verticalCenterOffset: 50
            anchors.verticalCenter: navBar.verticalCenter
            anchors.horizontalCenter: navBar.horizontalCenter
        }

         ListView {
             anchors.fill: flick
             model: test
             delegate: Text { text: name; color: "#ffffff"; }

             section.property: "size"
             section.criteria: ViewSection.FullString
             section.delegate: sectionHeading
         }
}
