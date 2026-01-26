import QtQuick
import QtQuick.Controls
import Core 1.0

MainScreen {
    id: mainScreen
    property ListModel dataModel: ListModel {}

    linkBtnMouseArea.onClicked: {
        Cp.connectBtn = true
        //console.log("linkBtnMouseArea clicked",Cp.connectBtn)
    }

    confirmBtnMouseArea.onClicked: {
        if(Cp.integrationTime !== Number(confirmTextField.text)){
            Cp.integrationTime = Number(confirmTextField.text)
            console.log("integrationTime:",Cp.integrationTime,Number(confirmTextField.text))
        }
    }
    autoSettingBtnMouseArea.onClicked: {
        if(Cp.integrationTime !== Number(confirmTextField.text)){
            Cp.integrationTime = Number(confirmTextField.text)
            console.log("integrationTime:",Cp.integrationTime,Number(confirmTextField.text))
        }
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
    saveLabelBtnMouseArea.onClicked: {
        if (lableTextField.text !== labelText) {
            Cp.label = lableTextField.text
            //console.log("lableTextField: ", labelText, Cp.label, lableTextField.text)
        }
    }
    saveFileBtnMouseArea.onClicked: {
        if (fileNameTextField.text !== fileNameText  ){
            Cp.fileName = fileNameTextField.text
            //console.log("fileNameTextField: ",Cp.fileName,fileNameText,fileNameTextField.text )
        }
        smartNIR.listModelToCsv(dataModel);
    }

    SmartNIR{
        id:smartNIR
        visible:false
        tableView.model:dataModel
    }


    //test
    function addData(){
        let currentTime = new Date().toLocaleTimeString(Qt.locale("zh_TW"), "hh:mm:ss");
        // 隨機生成一個數值
        let randomValue = (Math.random() * 5000).toFixed(0);
        dataModel.append({
            time: "20260120-" + currentTime, // 更新為當前年份
            label: Cp.label,
            v1: randomValue,
            v2: (Math.random() * 5000).toFixed(0),
            v3: (Math.random() * 5000).toFixed(0),
            v4: (Math.random() * 5000).toFixed(0),
            v5: (Math.random() * 5000).toFixed(0),
            v6: (Math.random() * 5000).toFixed(0),
            v7: (Math.random() * 5000).toFixed(0)
        });
        //console.log("dataModel",dataModel.get(0).time);
    }
}
