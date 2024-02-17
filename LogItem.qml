import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {

    id: log_list

    visible: false

    Rectangle {
        id: log_rect

        // anchors.leftMargin: 1
        // anchors.rightMargin: 1

        anchors.fill: parent

        color: Qt.darker("steelblue", 2)
        antialiasing: true
        radius: 10
        border.color: Qt.darker(log_rect.color, 2)
        border.width: 2

    }

    Component {
        id: contactDelegate
        Item {
            id: item_delegate

            property int min_height: 28

            height: log_msg_text.contentHeight > min_height
                    ? log_msg_text.contentHeight + 8
                      : min_height

            width: mdl_view.width;

            Rectangle {
                id: msg_rect
                anchors.fill: parent

                anchors.leftMargin: 10
                anchors.rightMargin: 30
                anchors.topMargin: 1
                anchors.bottomMargin: 1

                color: "transparent"
                border.color: Qt.darker(log_rect.color, 2)
                border.width: 1
                radius: 5
                opacity: 0.5
                antialiasing: true


                // row highlighter
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        msg_rect.opacity = 1
                        msg_rect.color = "white"
                        msg_rect.border.color = Qt.darker(log_rect.color, 2)
                        msg_rect.border.width = 2

                        // lm_timepoint.font.bold = true
                        lm_timepoint.color = "black"

                        // log_msg_text.font.bold = true
                        log_msg_text.color = "black"
                    }
                    onExited: {
                        msg_rect.opacity = 0.5
                        msg_rect.color = "transparent"
                        msg_rect.border.color = Qt.darker(log_rect.color, 4)
                        msg_rect.border.width = 1

                        // lm_timepoint.font.bold = false
                        lm_timepoint.color = "white"

                        // log_msg_text.font.bold = false
                        log_msg_text.color = "white"
                    }

                }

                RowLayout {
                    id: rl_log_msg

                    anchors.fill: parent

                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.topMargin: item_delegate.height === min_height ? 1 : 3
                    anchors.bottomMargin: item_delegate.height === min_height ? 1 : 3


                    // status (fake falue)
                    Rectangle {

                        id: lm_status
                        visible: true

                        width: item_delegate.min_height / 3
                        height: item_delegate.min_height / 3

                        Layout.alignment: Qt.AlignLeft
                        Layout.rightMargin: 10

                        radius: width / 2
                        border.color: "transparent"

                        antialiasing: true

                        function msg_status(s) {
                            return s > 0.75
                                    ? "red"
                                    : s < 0.25
                                      ? "orange"
                                        : "green";
                        }

                        color: msg_status(Math.random())
                    }

                    // timepoint (fake value)
                    Text {
                        id: lm_timepoint

                        Layout.minimumWidth: 150
                        // width: 250

                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter

                        color: "white"
                        opacity: 1
                        wrapMode: Text.NoWrap
                        antialiasing: true

                        text: getCurrentDateTime()
                    }

                    Item {
                        width: 20
                        antialiasing: Qt.AlignLeft
                    }

                    // msg
                    Text {
                        id: log_msg_text

                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter

                        Layout.fillWidth: true

                        color: "white"
                        opacity: 1
                        wrapMode: Text.WordWrap
                        antialiasing: true
                        text: display

                    }

                }


            }
        }
    }

    ListView {
        id: mdl_view
        anchors.fill: log_rect
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 5
        ScrollBar.vertical: ScrollBar {}

        clip: true

        model: listModel
        delegate: contactDelegate
    }

}
