import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.3

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1366
    height: 768
    title: qsTr("QTracker")

    // Main stackview
    StackView{
        id: stackView
        focus: true
        anchors.fill: parent
    }

    // After loading show initial Login Page
        Component.onCompleted: {
            stackView.push("qrc:/home.qml")
        }
}
