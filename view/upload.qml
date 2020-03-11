import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as Components

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#141414"

    Components.NavBar{
        id: navBar
    }

    Column {
        id: column
        anchors.verticalCenter: navBar.verticalCenter
        anchors.horizontalCenter: navBar.horizontalCenter
        spacing: 32

        Label {
            id: text
            color: "#ffffff"
            text: "Here is your upload thingy:"
        }



        TextEdit {
            id: torrentPassLabel
            color: "#ffffff"
            text: "https://www.tarves.no:1337/" + user.getTorrentPass() + "/announce"
            selectByKeyboard: true
            selectByMouse: true
            selectedTextColor: "#d0d046"
            readOnly: true
        }
    }
}
