import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls
import Core 1.0

Rectangle {
    height: 1080
    width: 1920
    // anchors.fill: parent
    clip: true
    color: "#16121c"
    property alias fileNameTextField: fileNameTextField
    property alias lableTextField: lableTextField
    property alias average_TimestextField: average_TimestextField
    property alias confirmTextField: confirmTextField
    property alias linkBtnMouseArea: linkBtnMouseArea
    property alias confirmBtnMouseArea: confirmBtnMouseArea
    property alias autoSettingBtnMouseArea: autoSettingBtnMouseArea
    property alias scanBtnMouseArea: scanBtnMouseArea
    property alias whiteScanBtnMouseArea: whiteScanBtnMouseArea
    property alias saveFileBtnMouseArea: saveFileBtnMouseArea
    property bool linkStatus:Cp.connectedLight
    property bool saveStatus:Cp.savedLight
    property int integrationTime: Cp.integTime
    property int averageTimes:Cp.avgTime
    property string labelText: Cp.label
    property string fileNameText: Cp.fileName

    Item {
        anchors.fill: parent
        anchors.topMargin: 60
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        ColumnLayout {
            anchors.fill: parent
            spacing: -60

            Rectangle {
                id: rectangleUp
                color: "#00ffffff"
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.5
                //anchors.topMargin: 20
                //spacing:10
                Item {
                    anchors.fill: parent
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    RowLayout {
                        id: rectangleUpColumncol
                        Layout.fillHeight: true
                        spacing: 15
                        anchors.fill: parent

                        ChartUI {
                            id: chartUI
                            // anchors.verticalCenter: parent.verticalCenter //垂直置中
                            // anchors.horizontalCenter: parent.horizontalCenter //水平置中
                        }

                        //list
                        Rectangle {
                            id: smartNIR
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "#21555353"
                            radius: 20

                            Item {
                                anchors.fill: parent
                                anchors.leftMargin: 20
                                anchors.rightMargin: 20
                                anchors.topMargin: 20
                                anchors.bottomMargin: 20
                                ColumnLayout {

                                    spacing: 15
                                    anchors.fill: parent
                                    //TITLE
                                    Rectangle {
                                        id: smartNIRTitle
                                        height: 36
                                        Layout.fillWidth: true

                                        color: "#e59263"
                                        radius: 10
                                        Text {
                                            id: aEYEOT_SMART_NIR
                                            height: 30
                                            width: parent.width
                                            color: "#ffffff"
                                            font.family: "Poppins"
                                            font.pixelSize: 20
                                            font.weight: Font.Bold
                                            horizontalAlignment: Text.AlignLeft
                                            text: "AEYEOT - SMART NIR"
                                            textFormat: Text.PlainText
                                            verticalAlignment: Text.AlignVCenter
                                            wrapMode: Text.WordWrap
                                            leftPadding: 20
                                        }
                                    }
                                    //DATA
                                    Rectangle {
                                        id: datalist

                                        Layout.fillHeight: true
                                        Layout.fillWidth: true
                                        radius: 8
                                        color: "transparent"
                                        SmartNIR {
                                            id: table
                                            anchors.fill: parent
                                            anchors.verticalCenter: parent.verticalCenter //垂直置中
                                            anchors.horizontalCenter: parent.horizontalCenter //水平置中
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: rectangleDown
                color: "#00ffffff"
                Layout.fillWidth: true
                Layout.preferredHeight: 450
                //Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                // anchors.verticalCenter: parent.verticalCenter
                // anchors.horizontalCenter: parent.horizontalCenter
                Item {
                    anchors.fill: parent
                    anchors.topMargin: 20
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 60

                    RowLayout {
                        id: bottomRow
                        spacing: 15
                        anchors.fill: parent

                        Rectangle {
                            id: linkDeviceRec
                            Layout.fillHeight: true
                            Layout.preferredWidth: 350
                            color: "#4d474747"
                            radius: 20

                            //linkdevice txt
                            Text {
                                id: linkDevicetext

                                anchors.top: parent.top
                                anchors.topMargin: 15
                                anchors.left: parent.left
                                anchors.leftMargin: 20

                                // height: 33
                                // width: 128
                                color: "#ffffff"
                                font.family: "Poppins"
                                font.pixelSize: 27
                                font.weight: Font.Bold
                                horizontalAlignment: Text.AlignLeft
                                text: "Link Device"
                                textFormat: Text.PlainText
                                verticalAlignment: Text.AlignTop
                                wrapMode: Text.WordWrap
                            }
                            //link_buttom
                            Rectangle {
                                id: link_buttom

                                anchors.top: linkDevicetext.bottom
                                anchors.topMargin: 30
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 20
                                height: 87.04
                                width: parent.width
                                //Layout.fillWidth: true
                                color: "#e59263"
                                radius: 20
                                Text {
                                    id: link
                                    color: "#ffffff"
                                    font.family: "Poppins"
                                    font.pixelSize: linkBtnMouseArea.containsMouse ? 44 : 40
                                    font.weight: Font.Bold
                                    text: "Link"
                                    textFormat: Text.PlainText
                                    anchors.verticalCenter: parent.verticalCenter //文字垂直置中
                                    anchors.horizontalCenter: parent.horizontalCenter //文字水平置中
                                    wrapMode: Text.WordWrap
                                }
                                layer.enabled: true
                                layer.effect: MultiEffect {
                                    shadowEnabled: linkBtnMouseArea.containsMouse ? true : false
                                    shadowColor: "white"
                                    shadowBlur: 0.8
                                }
                                MouseArea {
                                    id: linkBtnMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                }
                            }

                            //linkStatusIndicator
                            Rectangle {
                                id: linkStatusIndicator
                                anchors.top: link_buttom.bottom
                                anchors.topMargin: 50
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                height: 28
                                width: 28
                                color: linkStatus ? "#8deb85" : "red"
                                radius: 8

                                Text {
                                    id: linking_Status

                                    anchors.left: parent.right
                                    anchors.leftMargin: 15
                                    height: 33

                                    //width: 161
                                    color: "#ffffff"
                                    font.family: "Poppins"
                                    font.pixelSize: 24
                                    font.weight: Font.Bold
                                    horizontalAlignment: Text.AlignLeft
                                    text: "Linking Status"
                                    textFormat: Text.PlainText
                                    verticalAlignment: Text.AlignTop
                                    wrapMode: Text.WordWrap
                                }
                            }

                            //saveStatusIndicator
                            Rectangle {
                                id: saveStatusIndicator
                                anchors.top: linkStatusIndicator.bottom
                                anchors.topMargin: 15
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                height: 28
                                width: 28

                                color: saveStatus ? "#8deb85" : "red"
                                radius: 8

                                Text {
                                    id: save_Status

                                    anchors.left: parent.right
                                    anchors.leftMargin: 15
                                    height: 33
                                    width: 161
                                    color: "#ffffff"
                                    font.family: "Poppins"
                                    font.pixelSize: 22
                                    font.weight: Font.Bold
                                    horizontalAlignment: Text.AlignLeft
                                    text: "Saving Status"
                                    textFormat: Text.PlainText
                                    verticalAlignment: Text.AlignTop
                                    wrapMode: Text.WordWrap
                                }
                            }
                        }

                        //integration time text
                        Rectangle {
                            id: integration_time_rec
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            color: "#4d474747"
                            radius: 20
                            Item {
                                width: parent.width / 10 * 8
                                //anchors.verticalCenter: parent.verticalCenter//文字垂直置中
                                anchors.horizontalCenter: parent.horizontalCenter //文字水平置中
                                Text {
                                    id: integration_Time_text

                                    // x: 304.90
                                    // y: 448.05
                                    anchors.top: parent.top
                                    anchors.topMargin: 15

                                    // anchors.left: parent.left
                                    // anchors.leftMargin: 60
                                    height: 33
                                    width: parent.width
                                    color: "#ffffff"
                                    font.family: "Poppins"
                                    font.pixelSize: 27
                                    font.weight: Font.Bold
                                    horizontalAlignment: Text.AlignLeft
                                    text: "Integration Time(ms)"
                                    textFormat: Text.PlainText
                                    verticalAlignment: Text.AlignTop
                                    wrapMode: Text.WordWrap
                                }

                                TextField {
                                    id: confirmTextField
                                    anchors.top: integration_Time_text.bottom
                                    anchors.topMargin: 20
                                    anchors.left: integration_Time_text.left
                                    //anchors.leftMargin: 20
                                    height: 48
                                    width: (parent.width / 8 * 5 - 15)
                                    font.pixelSize: 22
                                    font.weight: Font.Bold
                                    font.family: "Poppins"
                                    text: integrationTime
                                    color: "#ffffff"
                                    validator: IntValidator {}
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    background: Rectangle {
                                        color: "#4de59263" // 背景黑色
                                        radius: 5 // 圓角
                                    }
                                }
                                Rectangle {
                                    id: confirmButton

                                    anchors.left: confirmTextField.right
                                    anchors.leftMargin: 15
                                    anchors.top: confirmTextField.top

                                    //anchors.topMargin: 15
                                    height: 48
                                    width: (parent.width / 8 * 3)

                                    color: "#e59263"
                                    radius: 10
                                    Text {
                                        id: confirm
                                        color: "#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: confirmBtnMouseArea.containsMouse ? 28 : 24
                                        font.weight: Font.Bold
                                        anchors.verticalCenter: parent.verticalCenter //文字垂直置中
                                        anchors.horizontalCenter: parent.horizontalCenter //文字水平置中
                                        text: "Confirm"
                                        textFormat: Text.PlainText
                                        wrapMode: Text.WordWrap
                                    }
                                    layer.enabled: true
                                    layer.effect: MultiEffect {
                                        shadowEnabled: confirmBtnMouseArea.containsMouse ? true : false
                                        shadowColor: "white"
                                        shadowBlur: 0.8
                                    }
                                    MouseArea {
                                        id: confirmBtnMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }

                                Rectangle {
                                    id: autoSetButtom

                                    anchors.top: confirmTextField.bottom
                                    anchors.topMargin: 20

                                    height: 48
                                    width: (parent.width)

                                    color: "#653e96"
                                    radius: 5

                                    Text {
                                        id: auto_Setting

                                        height: 48
                                        width: autoSetButtom.width

                                        color: "#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: autoSettingBtnMouseArea.containsMouse ? 28 : 24
                                        font.weight: Font.Bold
                                        text: "Auto Setting"
                                        textFormat: Text.PlainText
                                        verticalAlignment: Text.AlignVCenter //文字垂直置中
                                        horizontalAlignment: Text.AlignHCenter //文字靠左
                                        wrapMode: Text.WordWrap
                                    }
                                    layer.enabled: true
                                    layer.effect: MultiEffect {
                                        shadowEnabled: autoSettingBtnMouseArea.containsMouse ? true : false
                                        shadowColor: "white"
                                        shadowBlur: 0.8
                                    }
                                    MouseArea {
                                        id: autoSettingBtnMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }

                                Text {
                                    id: average_Timestext

                                    anchors.top: autoSetButtom.bottom
                                    anchors.topMargin: 25

                                    height: 33

                                    width: parent.width
                                    color: "#ffffff"
                                    font.family: "Poppins"
                                    font.pixelSize: 27
                                    font.weight: Font.Bold
                                    horizontalAlignment: Text.AlignLeft
                                    text: "Average Times"
                                    textFormat: Text.PlainText
                                    verticalAlignment: Text.AlignTop
                                    wrapMode: Text.WordWrap
                                }

                                TextField {
                                    id: average_TimestextField
                                    anchors.top: average_Timestext.bottom
                                    anchors.topMargin: 15

                                    height: 48
                                    width: auto_Setting.width
                                    font.pixelSize: 22
                                    font.weight: Font.Bold
                                    font.family: "Poppins"
                                    text: averageTimes
                                    color: "#ffffff"
                                    validator: IntValidator {}
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    background: Rectangle {
                                        color: "#4de59263" // 背景黑色
                                        radius: 5 // 圓角
                                    }
                                }
                            }
                        }
                        //Scan
                        Rectangle {
                            id: rectangle_59
                            Layout.preferredWidth: 300
                            Layout.fillHeight: true
                            color: "#4d474747"
                            radius: 20
                            Column {
                                id: scanRow
                                anchors.centerIn: parent
                                width: parent.width * 0.6
                                spacing: 30
                                Rectangle {
                                    id: rectangle_48
                                    width: parent.width
                                    height: 60
                                    color: "#e59263"
                                    radius: 5
                                    //anchors.centerIn: parent
                                    Text {
                                        id: scan
                                        color: "#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: scanBtnMouseArea.containsMouse ? 28 : 24
                                        font.weight: Font.Bold
                                        text: "Scan"
                                        textFormat: Text.PlainText
                                        anchors.verticalCenter: parent.verticalCenter //文字垂直置中
                                        anchors.horizontalCenter: parent.horizontalCenter //文字水平置中
                                        wrapMode: Text.WordWrap
                                    }
                                    layer.enabled: true
                                    layer.effect: MultiEffect {
                                        shadowEnabled: scanBtnMouseArea.containsMouse ? true : false
                                        shadowColor: "white"
                                        shadowBlur: 0.8
                                    }
                                    MouseArea {
                                        id: scanBtnMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }
                                Rectangle {
                                    id: whiteScan

                                    height: 166
                                    width: parent.width

                                    color: "#ffffff"
                                    radius: 5
                                    Text {
                                        id: white_Scan

                                        height: 66
                                        width: 79

                                        color: "#000000"
                                        font.family: "Poppins"
                                        font.pixelSize: whiteScanBtnMouseArea.containsMouse ? 28 : 24
                                        font.weight: Font.Bold
                                        horizontalAlignment: Text.AlignHCenter
                                        text: "White Scan"
                                        textFormat: Text.PlainText
                                        anchors.verticalCenter: parent.verticalCenter //文字垂直置中
                                        anchors.horizontalCenter: parent.horizontalCenter //文字水平置中
                                        wrapMode: Text.WordWrap
                                    }
                                    layer.enabled: true
                                    layer.effect: MultiEffect {
                                        shadowEnabled: whiteScanBtnMouseArea.containsMouse ? true : false
                                        shadowColor: "gray"
                                        shadowBlur: 0.8
                                    }
                                    MouseArea {
                                        id: whiteScanBtnMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }
                            }
                        }

                        //label
                        Rectangle {
                            id: lable
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "#4d474747"
                            radius: 10
                            Item {
                                anchors.fill: parent
                                anchors.leftMargin: 30
                                anchors.rightMargin: 30
                                anchors.topMargin: 20
                                anchors.bottomMargin: 30
                                Column {
                                    id: lableColumn
                                    anchors.centerIn: parent // ⭐ 上下左右置中
                                    spacing: 85
                                    anchors.fill: parent

                                    //LABLE
                                    Text {
                                        id: label_text
                                        height: 33
                                        width: parent.width
                                        color: "#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: 26
                                        font.weight: Font.Bold
                                        horizontalAlignment: Text.AlignLeft
                                        text: "Label"
                                        textFormat: Text.PlainText
                                        verticalAlignment: Text.AlignTop
                                        wrapMode: Text.WordWrap

                                        TextField {
                                            id: lableTextField
                                            height: 48
                                            width: parent.width
                                            anchors.top: parent.bottom
                                            anchors.topMargin: 5
                                            font.pixelSize: 22
                                            font.weight: Font.Bold
                                            font.family: "Poppins"
                                            text: labelText
                                            color: "#ffffff"

                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                            background: Rectangle {
                                                color: "#4de59263" // 背景黑色
                                                radius: 5 // 圓角
                                            }
                                        }
                                    }

                                    //filename
                                    Text {
                                        id: fileName_text
                                        height: 33
                                        width: parent.width
                                        color: "#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: 26
                                        font.weight: Font.Bold
                                        horizontalAlignment: Text.AlignLeft
                                        text: "File Name"
                                        textFormat: Text.PlainText
                                        verticalAlignment: Text.AlignTop
                                        wrapMode: Text.WordWrap
                                        TextField {
                                            id: fileNameTextField
                                            height: 48
                                            width: parent.width
                                            anchors.top: parent.bottom
                                            anchors.topMargin: 5
                                            font.pixelSize: 22
                                            font.weight: Font.Bold
                                            font.family: "Poppins"
                                            text: fileNameText
                                            color: "#ffffff"
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                            background: Rectangle {
                                                color: "#4de59263" // 背景黑色
                                                radius: 5 // 圓角
                                            }
                                        }
                                    }

                                    //savebotton
                                    Rectangle {
                                        id: saveButtom

                                        height: 48
                                        width: parent.width
                                        //anchors.top: fileNameTextField.bottom
                                        //anchors.topMargin: 120
                                        color: "#e59263"
                                        radius: 5
                                        Text {
                                            id: save_File
                                            color: "#ffffff"
                                            font.family: "Poppins"
                                            font.pixelSize: saveFileBtnMouseArea.containsMouse ? 28 : 24
                                            font.weight: Font.Bold
                                            //horizontalAlignment: Text.AlignLeft
                                            text: "Save File"
                                            textFormat: Text.PlainText
                                            verticalAlignment: Text.AlignTop
                                            wrapMode: Text.WordWrap
                                            anchors.verticalCenter: parent.verticalCenter //文字垂直置中
                                            anchors.horizontalCenter: parent.horizontalCenter //文字水平置中
                                        }
                                        layer.enabled: true
                                        layer.effect: MultiEffect {
                                            shadowEnabled: saveFileBtnMouseArea.containsMouse ? true : false
                                            shadowColor: "white"
                                            shadowBlur: 0.8
                                        }
                                        MouseArea {
                                            id: saveFileBtnMouseArea
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //}
                }
            }
        }
    }
}
