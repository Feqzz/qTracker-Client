import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as Components

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1920
    height: 1080
    title: qsTr("QTracker")

    // Main stackview
    StackView{
        id: stackView
        focus: true
        anchors.fill: parent


        replaceEnter: Transition {

                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 0
                }
            }
            replaceExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    duration: 0
                    from: 1
                    to:0

                }
            }
    }

    // After loading show initial Login Page
    Component.onCompleted: {
        stackView.push("qrc:/view/login.qml")
    }

    function pushPage(page) {
        var template = "qrc:/view/%1.qml";
        var url = template.arg(page);

        stackView.push(url);
    }

    function popToPage(page) {
        var template = "qrc:/view/%1.qml";
        var url = template.arg(page);

        stackView.pop(url);
    }

    function changePage(page) {
        var template = "qrc:/view/%1.qml";
        var url = template.arg(page);

        stackView.replace(url);
    }

    function logout()
    {
        loginHandler.logoutUser(user);
        popToPage("login");
    }

    function loginUser(username, password, label) {
        if(loginHandler.loginUser(user, username, password))
        {
            pushPage("home");
            label.text = "";

        }
        else
        {
            label.text = loginHandler.getErrorMessage();
        }
    }

    function registerUser(usernameField, passwordField,
                          emailField, inviteField, label) {

        if(loginHandler.registerUser(user, usernameField, passwordField,
                                                   emailField, inviteField)) {
            changePage("home");
            label.text = "";
        }
        else
        {
            label.text = loginHandler.getErrorMessage();
        }
    }

    function generateInvite(email, inviteLabel) {
        var inviteKey = inviteHandler.inviteUser(user.getId(),email);
        if(inviteKey && secureSocket.sendMessage(0,[email,inviteKey],2))
        {
            label.color = "#ffffff"
            label.text = "User has been invited!";
        }
        else
        {
            label.color = "#e90000";
            label.text = inviteHandler.getErrorMessage();
        }
    }

    function getRatio() {
        return user.getRatio();
    }

    function getUpload() {
        return user.getUpload();
    }

    function getDownload() {
        return user.getDownload();
    }

    function checkPrivilege() {
        return user.getPrivilege();
    }

    function updateText(text, id) {
        if (textHandler.updateText(user.getId(), text, id)){

        }
    }

    function getText(text, id) {
        text.text = textHandler.getText(id);
    }
}