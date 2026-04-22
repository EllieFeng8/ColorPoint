import QtQuick
import QtQuick.Controls
import Core 1.0
import QtQuick.Dialogs
import QtQuick.Layouts

MainScreen {
    id: mainScreen
    property ListModel mainDataModel: ListModel {}
    property int whiteScanReminderMs: 60 * 60 * 1000

    function addData(label){
        let now = new Date()

        // 取得日期
        let year = now.getFullYear()
        let month = String(now.getMonth() + 1).padStart(2, '0')
        let day = String(now.getDate()).padStart(2, '0')

        // 取得時間
        let time = now.toLocaleTimeString(Qt.locale("zh_TW"), "hh:mm:ss")

        let currentDateTime = `${year}${month}${day}-${time}`
        mainDataModel.append({
            time: currentDateTime, // 更新為當前年份
            label: label,
            // wavelength:0,
            // listData:1
        });
        console.log("mainDataModel",mainDataModel);
    }
    //nav
    inferenceMouseArea.onClicked: {
        inference.visible = true
    }

    linkBtnMouseArea.onClicked: {
        Cp.connectBtn = true
        //console.log("linkBtnMouseArea clicked",Cp.connectBtn)
    }
    confirmBtnMouseArea.onClicked: {
        Cp.integrationTime = confirmTextField.text
        console.log("integrationTime:",Cp.integrationTime,confirmTextField.text,typeof(Cp.integrationTime))
    }
    confirmAveTimeBtnMouseArea.onClicked: {
        Cp.avgTime = Number(average_TimestextField.text)
        console.log("integrationTime:",Cp.avgTime,average_TimestextField.text,typeof(average_TimestextField.text))
    }
    autoSettingBtnMouseArea.onClicked: {
        // Cp.integrationTime = confirmTextField.text
        Cp.autoBtn = true
        showAbnormal()
        console.log("autoSettingBtnMouseArea Clicked")
    }
    scanBtnMouseArea.onClicked: {

        addData(Cp.label);//test
        Cp.scanBtn = true
        console.log("scanBtnMouseArea Main clicked",Cp.scanBtn)
    }
    whiteScanBtnMouseArea.onClicked: {
        Cp.whiteBtn = true
        addData(Cp.whiteLabel);
        Cp.saveCurrentInstrumentConfigFile()
        whiteScanReminderTimer.restart()
        whiteScanClicked = true
    }

    lableTextField.onTextChanged:{
        Cp.label = lableTextField.text
        console.log("lableTextField: ",  Cp.label, lableTextField.text)
    }

    saveFileBtnMouseArea.onClicked: {
        // smartNIR.listModelToCsv(mainDataModel);
        Cp.saveFileNameBtn = true
    }
    heightconfirmBtnMouseArea.onClicked: {
        // if(Cp.autoSetHeightBtn === true) return
        if(heightSet1.text < 70 ) {
            Cp.heightSet = 70
        }
        else if(heightSet1.text > 120) {
            Cp.heightSet = 120
        }
        else {
            Cp.heightSet = Number(heightSet1.text)
        }

        console.log("heightSet1: ",  Cp.heightSet, heightSet1.text,Cp.autoSetHeightBtn )
    }

    resetbuttomMouseArea.onClicked: {
        Cp.resetBtn = true
        Cp.heightSet = 120
        console.log("resetBtn: ",  Cp.resetBtn)
    }
    SmartNIR{
        id:smartNIR
        visible:false
        tableView.model:mainDataModel
    }


    Timer {
        id: whiteScanReminderTimer
        interval: mainScreen.whiteScanReminderMs
        repeat: false
        onTriggered: whiteScanReminderDialog.open()
    }
    //標準白校正提醒
    Dialog {
        id: whiteScanReminderDialog
        modal: true
        focus: true
        anchors.centerIn: parent
        title: "提醒"
        standardButtons: Dialog.Ok
        width: 300
        height: 150

        contentItem: Label {
            text: "標準白校正已超過 1 小時，請重新量測。"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
        }
    }
    function showAbnormal() {
        lightLowerReminderDialog.open()
        // abnormalDialog.message = msg
    }
    //光源亮度不足
    Dialog {
        id: lightLowerReminderDialog
        modal: true
        focus: true
        anchors.centerIn: parent
        title: "⚠ 提醒"
        standardButtons: Dialog.Ok
        width: 300
        height: 150

        contentItem: Label {
            text: "光源亮度不足"
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            // verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
        }
    }
    Inference{
        id: inference
        visible:false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    //test

}
//console.log("dataModel",dataModel[2].length);
