import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls
import Core 1.0

Rectangle {
    id:imference
    height: 1080
    width: 1920
    // anchors.fill: parent
    clip: true
    color: "#16121c"
    property alias intakeMouseArea: intakeMouseArea
    property alias inferenceMouseArea: inferenceMouseArea
    //property alias saveLabelBtnMouseArea: saveLabelBtnMouseArea
    // property alias fileNameTextField: fileNameTextField
    property alias lableTextField: lableTextField
    property alias average_TimestextField: average_TimestextField
    property alias confirmTextField: confirmTextField
    property alias linkBtnMouseArea: linkBtnMouseArea
    property alias scanBtnMouseArea: scanBtnMouseArea
    property alias whiteScanBtnMouseArea: whiteScanBtnMouseArea
    property alias saveFileBtnMouseArea: saveFileBtnMouseArea
    property int integrationTime: 50
    property string labelText: "label"
    property string fileNameText: "file123"

    Item {
        anchors.fill: parent
        Layout.fillHeight: true
        Layout.preferredWidth: parent.width * 0.9
        anchors.topMargin: 50
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        Rectangle {
            y: 28
            height: parent.height * 0.9
            width: parent.width * 0.07

            color: "#21555353"
            radius: 50
            ColumnLayout {
                anchors.fill: parent
                anchors.verticalCenter: parent.verticalCenter //垂直置中
                anchors.horizontalCenter: parent.horizontalCenter //水平置中
                spacing: 10
                Image {
                    Layout.alignment: Qt.AlignHCenter
                    source: "assets/Rectangle 65.png"
                    scale: intakeMouseArea.containsMouse ? 1.6 : 1.5
                    Image {
                        y: 6
                        // anchors.verticalCenter: parent.verticalCenter //垂直置中
                        anchors.horizontalCenter: parent.horizontalCenter //水平置中
                        source: "assets/folder 1.png"
                        // scale: 1.3
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: intakeMouseArea.containsMouse ? true : false
                            shadowColor: "white"
                            shadowBlur: 0.8
                        }
                    }
                    Text {
                        // text:"intake"
                        // id: aEYEOT_SMART_NIR
                        y: 47
                        x: -16
                        height: 30
                        width: parent.width
                        color: "#ffffff"
                        font.family: "Poppins"
                        font.pixelSize: 14
                        font.weight: Font.Bold
                        // horizontalAlignment: Text.AlignHCenter
                        text: "intake"
                        textFormat: Text.PlainText
                        // verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        leftPadding: 20
                    }

                    MouseArea {
                        id: intakeMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked:imference.visible = false
                    }
                }
                Image {
                    // anchors.verticalCenter: parent.verticalCenter //垂直置中
                    anchors.horizontalCenter: parent.horizontalCenter //水平置中
                    source: "assets/Rectangle 66.png"
                    scale: inferenceMouseArea.containsMouse ? 1.6 : 1.5
                    Image {
                        y: 6
                        // anchors.verticalCenter: parent.verticalCenter //垂直置中
                        anchors.horizontalCenter: parent.horizontalCenter //水平置中
                        source: "assets/ai1.png"
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: inferenceMouseArea.containsMouse ? true : false
                            shadowColor: "white"
                            shadowBlur: 0.8
                        }
                    }
                    Text {
                        // text:"intake"
                        // id: aEYEOT_SMART_NIR
                        y: 47
                        x: -30
                        height: 30
                        width: parent.width
                        color: "#ffffff"
                        font.family: "Poppins"
                        font.pixelSize: 14
                        font.weight: Font.Bold
                        // horizontalAlignment: Text.AlignHCenter
                        text: "Inference"
                        textFormat: Text.PlainText
                        // verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        leftPadding: 20
                    }
                    MouseArea {
                        id: inferenceMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                    }
                }
            }
        }
        //anchors.centerIn: parent
        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 138
            // Layout.fillHeight: true
            // Layout.preferredWidth: parent.width * 0.9
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
                            color: "transparent"
                            radius: 20

                            RowLayout {
                                id: nirRectangle
                                anchors.fill: parent
                                spacing: 15

                                Rectangle {
                                    id: smartNIR2
                                    // Layout.fillWidth: true
                                    Layout.preferredWidth: parent.width * 0.4
                                    Layout.fillHeight: true
                                    color: "#21555353"
                                    radius: 20
                                    Layout.alignment: Qt.AlignHCenter

                                    ListView {
                                        id: listView
                                        anchors.fill: parent
                                        anchors.leftMargin:20
                                        anchors.topMargin:36
                                        model: ListModel {
                                            ListElement {
                                                name: "Target 1"
                                                colorCode: "red"
                                            }

                                            ListElement {
                                                name: "Target 2"
                                                colorCode: "green"
                                            }

                                            ListElement {
                                                name: "Target 3"
                                                colorCode: "blue"
                                            }

                                            ListElement {
                                                name: "Target 4"
                                                colorCode: "white"
                                            }
                                        }
                                        delegate: Column {
                                            spacing: 5
                                            padding:10
                                            Text {
                                                width: 100
                                                height: 20
                                                color:"#ffffff"
                                                font.family: "Poppins"
                                                font.pixelSize: 20
                                                font.weight: Font.Bold
                                                horizontalAlignment: Text.AlignLeft
                                                text: name
                                            }

                                            Text {
                                                width: 100
                                                color:"#e59263"
                                                font.family: "Poppins"
                                                font.pixelSize: 20
                                                font.weight: Font.Bold
                                                horizontalAlignment: Text.AlignRight
                                                text: colorCode
                                            }
                                        }
                                    }

                                }

                                ColumnLayout {
                                    spacing: 15
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    //anchors.fill: parent
                                    // anchors.rightMargin: 467
                                    Rectangle{
                                        Layout.preferredHeight: parent.height * 0.15
                                        Layout.fillWidth: true
                                        color: "#21555353"
                                        radius: 20
                                        Row{
                                            anchors.fill: parent
                                            spacing:50

                                        Text{
                                            anchors.fill: parent
                                            color:"#ffffff"
                                            font.family: "Poppins"
                                            font.pixelSize: 25
                                            font.weight: Font.Bold
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                            anchors.leftMargin: 10
                                            text: "高度"

                                        }
                                        Text{
                                            anchors.fill: parent
                                            // x:250
                                            color:"#ffffff"
                                            font.family: "Poppins"
                                            font.pixelSize: 35
                                            font.weight: Font.Normal
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            text: "753"

                                        }
                                        Text{
                                            anchors.fill: parent
                                            // x:250
                                            color:"#ffffff"
                                            font.family: "Poppins"
                                            font.pixelSize: 25
                                            font.weight: Font.Normal
                                            horizontalAlignment: Text.AlignRight
                                            verticalAlignment: Text.AlignVCenter
                                            anchors.rightMargin: 20
                                            text: "cm"

                                        }
                                        }

                                    }

                                    Rectangle {
                                        id: smartNIR11
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        color: "#21555353"
                                        radius: 20
                                        //TITLE
                                        Rectangle {
                                            id: smartNIRTitle

                                            width: parent.width * 0.9
                                            height: 36
                                            anchors.horizontalCenter: parent.horizontalCenter //水平置中
                                            // Layout.fillWidth: true
                                            // Layout.fillHeight: true
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
                                            anchors.top: smartNIRTitle.bottom
                                            anchors.topMargin: 20
                                            anchors.left: smartNIRTitle.left
                                            anchors.right: smartNIRTitle.right
                                            height: parent.height * 0.85
                                            width: parent.width
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
                            Layout.preferredWidth: parent.width * 0.2
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
                                    onClicked: {
                                        Cp.inferConnectBtn = true
                                        console.log("linkBtnMouseArea clicked",Cp.inferConnectBtn)
                                    }
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
                                color: Cp.inferConnectedLight ? "#8deb85" : "red"
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


                        }

                        //integration time text
                        Rectangle {
                            id: integration_time_rec
                            Layout.fillWidth: parent.width * 0.3
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
                                    font.pixelSize: 24
                                    font.weight: Font.Bold
                                    horizontalAlignment: Text.AlignLeft
                                    text: "Integration Time(ms)"
                                    textFormat: Text.PlainText
                                    verticalAlignment: Text.AlignTop
                                    // wrapMode: Text.WordWrap
                                }

                                TextField {
                                    id: confirmTextField
                                    anchors.top: integration_Time_text.bottom
                                    anchors.topMargin: 20
                                    anchors.left: integration_Time_text.left
                                    enabled:false
                                    height: 48
                                    width: parent.width
                                    font.pixelSize: 22
                                    font.weight: Font.Bold
                                    font.family: "Poppins"
                                    text: Cp.inferIntegrationTime
                                    color: "#ffffff"
                                    validator: IntValidator {}
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    background: Rectangle {
                                        color: "#4de59263" // 背景黑色
                                        radius: 5 // 圓角
                                    }
                                }


                                Text {
                                    id: average_Timestext

                                    anchors.top: confirmTextField.bottom
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
                                    enabled:false
                                    height: 48
                                    width: parent.width
                                    font.pixelSize: 22
                                    font.weight: Font.Bold
                                    font.family: "Poppins"
                                    text: Cp.inferAvgTime
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
                            Layout.preferredWidth: parent.width * 0.2
                            Layout.fillHeight: true
                            color: "#4d474747"
                            radius: 20
                            Column {
                                id: scanRow
                                anchors.centerIn: parent
                                width: parent.width * 0.6
                                spacing: 15
                                Rectangle {
                                    id: rectangle_48
                                    width: parent.width
                                    height: 90
                                    color: "#e59263"
                                    radius: 5
                                    //anchors.centerIn: parent
                                    Text {
                                        id: scan
                                        color: "#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: scanBtnMouseArea.containsMouse ? 30 : 28
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
                                        onClicked: {
                                            addData();//test
                                            Cp.inferScanBtn = true
                                            console.log("scanBtnMouseArea clicked",Cp.inferScanBtn)
                                        }
                                    }
                                }
                                Rectangle {
                                    id: rectangle11
                                    width: parent.width
                                    height: parent.height *0.3
                                    color: Cp.inferAutoScanBtn? "red" : "#653e96"
                                    radius: 5
                                    //anchors.centerIn: parent
                                    Text {
                                        id: auto1
                                        color: "#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: autoScanBtnMouseArea.containsMouse ? 30 : 28
                                        font.weight: Font.Bold
                                        text: Cp.inferAutoScanBtn? "Stop" : "Auto"
                                        textFormat: Text.PlainText
                                        anchors.verticalCenter: parent.verticalCenter //文字垂直置中
                                        anchors.horizontalCenter: parent.horizontalCenter //文字水平置中
                                        wrapMode: Text.WordWrap
                                    }
                                    layer.enabled: true
                                    layer.effect: MultiEffect {
                                        shadowEnabled: autoScanBtnMouseArea.containsMouse ? true : false
                                        shadowColor: "white"
                                        shadowBlur: 0.8
                                    }
                                    MouseArea {
                                        id: autoScanBtnMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            Cp.inferAutoScanBtn = !Cp.inferAutoScanBtn
                                            console.log("autoScanBtnMouseArea clicked",Cp.inferAutoScanBtn)
                                        }
                                    }
                                }
                                Rectangle {
                                    id: whiteScan

                                    height: parent.height *0.3
                                    width: parent.width

                                    color: "#ffffff"
                                    radius: 5
                                    Text {
                                        id: white_Scan

                                        height: 66
                                        width: 79

                                        color: "#000000"
                                        font.family: "Poppins"
                                        font.pixelSize: whiteScanBtnMouseArea.containsMouse ? 30 : 28
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
                                        onClicked: {
                                            Cp.inferWhiteBtn = true
                                            Cp.whiteScanList
                                            console.log("whiteScanBtnMouseArea clicked",Cp.inferWhiteBtn)
                                        }
                                    }
                                }
                            }
                        }
                        //modelset
                        Rectangle {
                            id: modelset
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "#21555353"
                            radius: 20
                            Layout.alignment: Qt.AlignHCenter
                            Text{
                                x: 28
                                y: 10
                                width: 109
                                height: 15
                                color: "#ffffff"
                                font.family: "Poppins"
                                font.weight: Font.Bold
                                // anchors.verticalCenter: parent.verticalCenter //文字垂直置中
                                // anchors.leftMargin: 20 //文字水平置中
                                text: "Model Setting"
                                textFormat: Text.PlainText
                                // wrapMode: Text.WordWrap
                            }

                            ListView {
                                id: listView2
                                anchors.fill: parent
                                anchors.leftMargin:20
                                anchors.topMargin:39
                                model: ListModel {
                                    ListElement {
                                        name: "Target 1"
                                        colorCode: "red"
                                    }

                                    ListElement {
                                        name: "Target 2"
                                        colorCode: "green"
                                    }

                                    ListElement {
                                        name: "Target 3"
                                        colorCode: "blue"
                                    }

                                    ListElement {
                                        name: "Target 4"
                                        colorCode: "white"
                                    }
                                }
                                delegate: Row {
                                    spacing: 20
                                    padding:10
                                    Text {
                                        width: 100
                                        height: 20
                                        color:"#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: 20
                                        font.weight: Font.Bold
                                        horizontalAlignment: Text.AlignLeft
                                        text: name
                                    }

                                    Text {
                                        width: 100
                                        color:"#ffffff"
                                        font.family: "Poppins"
                                        font.pixelSize: 20
                                        horizontalAlignment: Text.AlignRight
                                        text: colorCode
                                    }
                                }
                            }

                        }
                        //label
                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            ColumnLayout {
                                id: fileRec
                                anchors.centerIn: parent // ⭐ 上下左右置中
                                spacing: 20
                                anchors.fill: parent

                                // topPadding: 40
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
                                        anchors.topMargin: 30
                                        anchors.bottomMargin: 10

                                        Column {
                                            id: lableColumn
                                            anchors.centerIn: parent // ⭐ 上下左右置中
                                            spacing: 10
                                            anchors.fill: parent
                                            topPadding: 5

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
                                            }
                                            Rectangle {
                                                height: 48
                                                width: parent.width
                                                // anchors.top: label_text.bottom
                                                anchors.topMargin: 5
                                                color: "transparent"
                                                Column {
                                                    //id: lableColumn
                                                    anchors.centerIn: parent // ⭐ 上下左右置中
                                                    spacing: 150
                                                    anchors.fill: parent
                                                    TextField {
                                                        id: lableTextField
                                                        height: parent.height
                                                        width: parent.width
                                                        //anchors.top: parent.bottom
                                                        //anchors.topMargin: 5
                                                        font.pixelSize: 22
                                                        font.weight: Font.Bold
                                                        font.family: "Poppins"
                                                        color: "#ffffff"
                                                        text: Cp.inferLabel

                                                        horizontalAlignment: Text.AlignLeft
                                                        verticalAlignment: Text.AlignVCenter
                                                        background: Rectangle {
                                                            color: "#4de59263" // 背景黑色
                                                            radius: 5 // 圓角
                                                        }
                                                        onTextChanged:{
                                                            Cp.inferLabel = lableTextField.text
                                                            console.log("lableTextField: ",  Cp.inferLabel, lableTextField.text)

                                                        }
                                                    }
                                                    //savebotton
                                                    Rectangle {
                                                        id: saveButtom

                                                        height: parent.height
                                                        width: parent.width
                                                        //anchors.top: fileNameTextField.bottom
                                                        anchors.topMargin: 10
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
