import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12
import "../component" as Components

Rectangle {
    id: root
    //width: 1920
    //height: 1080
    anchors.fill: parent
    color: "#141414"

    Components.NavBar{
        id: navBar
    }

    Rectangle {
        id: announceUrlRectangle
        //anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: root.horizontalCenter
        //anchors.verticalCenterOffset: -100
        y: 300
        width: 720
        height: 200
        color: "#303030"
        radius: 16
        border.width: 0

        Column {
            id: column
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 16

            /*
            Text {
                id: text
                text: "qTracker v1.0"
                color: "#ffffff"
                font.pixelSize: 24
            } */

            Text {
                id: welcomeUserText

                text: "Welcome back, " + "<font color=\"#c2a800\">" + user.getUsername() + "</font>"
                color: "#ffffff"
                font.pixelSize: 24
            }


        }


    }



    Component.onCompleted: {
        updateUsersData();
        navBar.update();
        setTitle("qTracker :: Home");
    }

}

/*##^## Designer {
    D{i:3;anchors_x:583;anchors_y:232}
}
 ##^##*/
