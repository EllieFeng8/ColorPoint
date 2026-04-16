import QtQuick
import QtQuick.Controls
import Core 1.0
import QtQuick.Dialogs

MainScreen {
    id: mainScreen
    property ListModel dataModel: ListModel {}
    property int whiteScanReminderMs: 60 * 60 * 1000
    //nav
    inferenceMouseArea.onClicked: {
        imference.visible = true
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
        Cp.avgTime = average_TimestextField.text
        console.log("integrationTime:",Cp.avgTime,average_TimestextField.text,typeof(average_TimestextField.text))
    }
    autoSettingBtnMouseArea.onClicked: {
        // Cp.integrationTime = confirmTextField.text
        console.log("autoSettingBtnMouseArea Clicked")
    }
    scanBtnMouseArea.onClicked: {
        Cp.scanBtn = true
        addData();//test
        // console.log("scanBtnMouseArea clicked",Cp.scanBtn)
    }
    whiteScanBtnMouseArea.onClicked: {
        Cp.whiteBtn = true
        //console.log("whiteScanBtnMouseArea clicked",Cp.whiteBtn)
    }

    lableTextField.onTextChanged:{
        Cp.label = lableTextField.text
        console.log("lableTextField: ",  Cp.label, lableTextField.text)

    }

    saveFileBtnMouseArea.onClicked: {
        smartNIR.listModelToCsv(dataModel);
    }
    heightconfirmBtnMouseArea.onClicked: {
        Cp.heightSet = Number(heightSet1.text)
        console.log("lableTextField: ",  Cp.heightSet, heightSet1.text)

    }

    resetbuttomMouseArea.onClicked: {
        Cp.resetBtn = true
        console.log("resetBtn: ",  Cp.resetBtn)

    }
    SmartNIR{
        id:smartNIR
        visible:false
        tableView.model:dataModel
    }

    importFileBtnMouseArea.onClicked: {
        fileDialog.open()
    }
    FileDialog {
        id: fileDialog
        title: "選擇本地檔案"
        currentFolder: "file:///"   // 開啟本地磁碟
        nameFilters: ["All files (*)", "Text files (*.txt)"]

        onAccepted: {
            console.log("選到的檔案:", selectedFile)
        }
    }
    Timer {
        id: whiteScanReminderTimer
        interval: mainScreen.whiteScanReminderMs
        repeat: false
        onTriggered: whiteScanReminderDialog.open()
    }
    Dialog {
        id: whiteScanReminderDialog
        modal: true
        focus: true
        anchors.centerIn: parent
        title: "提醒"
        standardButtons: Dialog.Ok

        contentItem: Label {
            text: "白板量測已超過 1 小時，請重新量測。"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
        }
    }
    Imference{
        id: imference
        visible:false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    //test
    function addData(){
        let currentTime = new Date().toLocaleTimeString(Qt.locale("zh_TW"), "hh:mm:ss");
        // 隨機生成一個數值
        let randomValue = (Math.random() * 5000).toFixed(0);
        dataModel.append({
            time: "20260120-" + currentTime, // 更新為當前年份
            label: Cp.label
        });
        //console.log("dataModel",dataModel.count(0));
    }
}
//console.log("dataModel",dataModel[2].length);