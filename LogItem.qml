import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {

    id: log_list

    visible: false

    Rectangle {
        id: log_rect

        anchors.leftMargin: 1
        anchors.rightMargin: 1

        anchors.fill: parent

        color: "steelblue"
        antialiasing: true
        radius: 10
        border.color: Qt.darker(log_rect.color, 2)
        border.width: 2

    }

    Component {
        id: contactDelegate
        Item {
            id: item_delegate

            height: 25
            width: mdl_view.width;

            Text {
                anchors.fill: parent
                text: display
                color: "white"
            }
        }
    }

    ListView {
        id: mdl_view
        anchors.fill: log_rect
        anchors.margins: 5
        spacing: 7
        ScrollBar.vertical: ScrollBar {}

        model: listModel
        delegate: contactDelegate
    }

}
