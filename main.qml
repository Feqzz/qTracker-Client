import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.3

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
    }

    // After loading show initial Login Page
    Component.onCompleted: {
        stackView.push("qrc:/login.qml")
        login.login()
    }

    function changePage(page) {
        var template = "qrc:/%1.qml";
        var url = template.arg(page);
        stackView.replace(url);
    }

    function loginUser(username, password, label) {
        if(login.login(username, password))
        {
            changePage("home");
            label.text = "";

        }
        else
        {
            label.text = login.getErrorMessage();
        }
    }

    function registerUser(usernameField, passwordField,
                          emailField, inviteField, label) {

        if(login.registerUser(usernameField, passwordField,
                                                   emailField, inviteField)) {
            changePage("home");
            label.text = "";
        }
        else
        {
            label.text = login.getErrorMessage();
        }
    }

    function generateInvite(email, inviteLabel) {
        var str = invite.generateKey(email);
        inviteLabel.text(str);
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
        //return user.getPrivilege();
        return 1;
    }

    function updateText(text) {
        rules.updateText(text)
    }

    function getText(text) {
        text.text = rules.getText();
    }
}
