import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    id: win
    width: 400
    height: 800
    visible: true
    title: qsTr("Piklema")

    // properties
    property int layout_margin: 5
    property int item_spacing: 5


    // size's
    minimumWidth: 400
    maximumWidth: Screen.width

    minimumHeight: 900 + 2 * layout_margin
    maximumHeight: Screen.height

    Page {

        id: page
        anchors.fill: parent


        header: HeaderItem {
            id: header_rect
            color: "#161616"
            height: 100
            width: parent.width
        }

        footer: FooterItem {
            id: footer_rect
            color: "darkgray"
            height: 70
            width: 800
        }

        background: Rectangle {
            color: "lightgray"
        }

        // layout
        BasicItem {
            id: clayout
            anchors.fill: parent
            anchors.margins: layout_margin
            spacing: item_spacing

            // default values
            host_default: host
            port_default: port
            nick_default: username
            pass_default: password
            topic_default: topic
            path_default: filepath
        }

        Rectangle {

            visible: !footer_rect.log_pushed
            anchors.fill: clayout
            border.width: 1
            border.color: "blue"
            color: "transparent"
            radius: 10
            antialiasing: true
        }


        LogItem {
            id: log_list
            anchors.fill: clayout
            visible: footer_rect.log_pushed
        }

    }

}
