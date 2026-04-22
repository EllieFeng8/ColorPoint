#include "core.h"
#include <cmath>  
#include <cstdint> 
#include <QFileDialog>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

Core::Core(QObject* parent)
    : QObject(parent)
{

}
Core::~Core()
{
   
    if (threadSpec && threadSpec->isRunning()) {
        if (m_Spec) {
            // 停止定時器並關閉設備
            QMetaObject::invokeMethod(m_Spec, [this] { m_Spec->stop(); }, Qt::BlockingQueuedConnection);

            QMetaObject::invokeMethod(m_Spec, [this] {
                m_Spec->CloseSpectrometer(0);
                }, Qt::BlockingQueuedConnection);

            m_Spec->deleteLater();
        }
        threadSpec->quit();
        if (!threadSpec->wait(3000))
        { 
            threadSpec->terminate();   
            threadSpec->wait();
        }
    }

    if (threadSusi && threadSusi->isRunning()) {
        if (m_susi) {
            // 停止定時器並關閉設備
            QMetaObject::invokeMethod(m_susi, [this] { m_susi->stop(); }, Qt::BlockingQueuedConnection);

            QMetaObject::invokeMethod(m_susi, [this] {
                m_susi->uninit();
                }, Qt::BlockingQueuedConnection);

            m_susi->deleteLater();
        }


        threadSusi->quit();


        if (!threadSusi->wait(3000))
        {
            threadSusi->terminate();
            threadSusi->wait();
        }
    }

    if (threadModbus && threadModbus->isRunning()) {
        if (m_modbus) {
            // 停止定時器並關閉設備
            QMetaObject::invokeMethod(m_modbus, [this] { m_modbus->stop(); }, Qt::BlockingQueuedConnection);

            m_modbus->deleteLater();
        }


        threadModbus->quit();


        if (!threadModbus->wait(3000))
        {
            threadModbus->terminate();
            threadModbus->wait();
        }
    }
 }
Core& Core::instance()
{
    static Core inst;
    return inst;
}

void Core::init()
{
    m_proxy = new ColorPointProxy(this);

    threadSpec = new QThread(this);
    m_Spec = new Spectrometer();
    m_Spec->moveToThread(threadSpec);
    threadSpec->start();

    threadSusi = new QThread(this);
    m_susi = new susi_control();
    m_susi->moveToThread(threadSusi);
    threadSusi->start();

    threadModbus = new QThread(this);
    m_modbus = new ModbusHandler();
    m_modbus->moveToThread(threadModbus);
    threadModbus->start();

    Core_connect();
   
}

void Core::Core_connect()
{
    //TODO connect proxy的lable 到 Spectrometer::setlable
    QObject::connect(m_proxy, &ColorPointProxy::labelChanged, m_Spec, &Spectrometer::SetLable, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::clearNirListBtnChanged, m_Spec, &Spectrometer::clearlist, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::autoSetHeightBtnChanged, m_Spec, &Spectrometer::autoset, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::inferAutoScanBtnChanged, this,[this](bool v)
        {
            if(v)
            {
                if(m_Spec)
                {
                    QMetaObject::invokeMethod(m_Spec, [this]()
                        {
                            m_Spec->StartContinuousAcq(500);
                        });
                }
            }
            else
            {
                if (m_Spec)
                {
                    QMetaObject::invokeMethod(m_Spec, [this]()
                        {
                            m_Spec->StopContinuousAcq();
                        });
                }
            }
        }, Qt::QueuedConnection);
    //QObject::connect(m_proxy, &ColorPointProxy::saveFileNameBtnChan, &Spectrometer::saveToCSV, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::saveFileNameBtnChanged, this, &Core::saveData, Qt::QueuedConnection);
    //QObject::connect(m_proxy, &ColorPointProxy::autoSetHeightBtnChanged, this, [this](bool v)
    //    {
    //        m_AutoHeight = v;
    //    }, Qt::QueuedConnection);

    QObject::connect(m_proxy, &ColorPointProxy::connectBtnChanged, m_Spec, &Spectrometer::OpenSpectrometer,Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::integrationTimeChanged, m_Spec, &Spectrometer::SetIntegrationTime, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::avgTimeChanged, m_Spec,&Spectrometer::SetAVG, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::scanBtnChanged, m_Spec, &Spectrometer::Scan, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::heightSetChanged, this, &Core::setHeight, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::resetBtnChanged, this, &Core::onResetBtnChanged, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::whiteBtnChanged, this, &Core::whiteScan, Qt::QueuedConnection);

    QObject::connect(threadSusi, &QThread::started, m_susi, &susi_control::init);
    QObject::connect(threadModbus, &QThread::started, m_modbus, &ModbusHandler::init);
    QObject::connect(m_Spec, &Spectrometer::DataIntensity, this, &Core::onDataAcquires);
    QObject::connect(m_Spec, &Spectrometer::isOpen, this, [this]() 
        {
            m_proxy->setConnectedLight(true);
        });

    QObject::connect(m_modbus, &ModbusHandler::dataReceived, this, &Core::onModbusDataReceived);
    QObject::connect(m_susi, &susi_control::count, this, &Core::Height);

}

void Core::OpenSpectrometer(bool v)
{
//    qDebug() << "thy Search & Open ";
//    m_Spec->SearchSpectrometer();
//    m_Spec->OpenSpectrometer();
}
void Core::setIntegrationTime(int v)
{
//    m_Spec->SetIntegrationTime(0, v);
//    m_IntegrationTime = v;
}

void Core::setAVG(int v)
{
//    Avg = v;
//    m_Spec->SetAVG(v);
}

void Core::onDataAcquires(vector<float> values)
{
    m_proxy->setChartData(m_Spec->wavelength,values);

}

void Core::onModbusDataReceived(int address, QVector<quint16> data)
{
    if ( !data.isEmpty()) 
    {
        int status = data[0]; // 1:正轉, 2:反轉, 3:停止
        //m_currentDirection = data[0];
        // 1. 將方向狀態傳遞給 susi_control 執行緒
        if (m_susi) 
        {
            // 使用 QMetaObject::invokeMethod 確保跨執行緒呼叫安全
            QMetaObject::invokeMethod(m_susi, [this, status]()
                {
                    m_susi->setDirection(status);
                });
        }
    }
}
void Core::Height(int h)
{    
    //if (m_height != h) 
    //{
        m_height = h;
    //    if (!m_Spec->m_pendingRecords.empty())
    //    {
    //        qWarning() << "don't move";
    //    }
    //}
    if (m_proxy) {
        if (h > 120) {
            m_proxy->setHeight(120);
        }
        else {
        m_proxy->setHeight(h);
    }
    }// qDebug() << m_height;

    if (!m_AutoHeight || m_targetHeight == 0)
    {
        if (m_currentDirection != 3) {
            //m_susi->setGPIO2(0);
            //m_susi->setGPIO3(0);
            m_currentDirection = 3; // 重設狀態
            qDebug() << "!m_AutoHeight || m_targetHeight == 0";
        }
        return;
    }

    // 計算差距
    double diff = h - m_targetHeight;

     //1. 到達目標範圍 (容許誤差 0.25)
    if (qAbs(diff) <= 0.25)
    {

            m_currentDirection = 3;    // 標記目前已停止
            //this->controlInverter(3);
            QMetaObject::invokeMethod(m_susi, [this]()
                {
                    m_susi->setGPIO2(0); 
                    m_susi->setGPIO3(0); 
                });
            m_AutoHeight = false;      
            qDebug() << "Target Reached. Stopped.";
            return;
        
    }
    // 2. 目前太高 -> 需要下降 (假設 2 是反轉/下降)
    else if (h > m_targetHeight)
    {

            m_currentDirection = 2;    

            //this->controlInverter(2);
            if (m_susi)
            {

                QMetaObject::invokeMethod(m_susi, [this]()
                    {
                        m_susi->setGPIO2(0); //確認上升停止
                        m_susi->setGPIO3(1); //開始下降
                    });
            }
            qDebug() << "Moving Down...";
        
    }
    // 3. 目前太低 -> 需要上升 (假設 1 是正轉/上升)
    else if (h < m_targetHeight)
    {

            m_currentDirection = 1;  

            //this->controlInverter(1);
            if (m_susi)
            {

                QMetaObject::invokeMethod(m_susi, [this]()
                    {
                        m_susi->setGPIO3(0); //確認下降停止
                        m_susi->setGPIO2(1); //開始上升
                    });
            }
            qDebug() << "Moving Up...";
        }
    
}


void Core::setHeight(int h)
{
    m_targetHeight = h;
    m_AutoHeight = true; // 開啟
}

void Core::controlInverter(int command)
{


    //485
    //if (m_modbus) {
    //    //qDebug() << m_targetHeight;
    //    // 跨執行緒安全呼叫 ModbusHandler 的 writeInverterControl
    //    QMetaObject::invokeMethod(m_modbus, [this, command]() {
    //        m_modbus->writeInverterControl(command);
    //        });
    //}
}

void Core::onResetBtnChanged(bool v)
{

    if (m_susi)
    {
        // 使用 QMetaObject::invokeMethod 確保跨執行緒呼叫安全
        QMetaObject::invokeMethod(m_susi, [this]()
            {
                m_susi->reset();
            });
    }
}
void Core::whiteScan(bool v)
{
    QMetaObject::invokeMethod(m_Spec, [this]()
        {
            m_Spec->WhiteScan(100000,1);
        });
}
void Core::saveData(bool v)
{
    if (m_Spec->m_pendingRecords.empty()) {
        qDebug() << "No data to save.";
        return;
    }
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/data_log.csv";
    QString csvPath = QFileDialog::getSaveFileName(
        nullptr,
        "select Path",
        initialPath,
        "CSV Files (*.csv);;All Files (*)"
    );

    if (csvPath.isEmpty()) {
        return; // 使用者取消選擇
    }
    QFile file(csvPath);
    bool fileExists = file.exists();

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing";
        return;
    }

    QTextStream out(&file);

    // 1. 寫入表頭 (僅在檔案剛建立時)
    if (!fileExists || file.size() == 0) {
        out << "Time,Label";
        for (float w : m_Spec->wavelength) {
            out << "," << QString::number(w, 'f', 1); // 輸出波長作為欄位名
        }
        out << "\n";
    }

    // 2. 遍歷所有暫存記錄，確保每一筆掃描佔用一行
    for (const auto& record : m_Spec->m_pendingRecords) {
        out << record.time << "," << (record.label.isEmpty() ? "None" : record.label);

        for (float val : record.data) {
            out << "," << QString::number(val, 'g', 10); // 每一筆資料點接在同一行後方
        }
        out << "\n"; // 這筆記錄結束，換行
    }

    file.close();
    QString jsonPath = csvPath;
    jsonPath.replace(".csv", ".json");
    saveConfigJson(jsonPath);

    // 3. 儲存完畢後清空記憶體，避免下次儲存時重複寫入舊資料
    m_Spec->m_pendingRecords.clear();
    qDebug() << "All data saved to CSV and memory cleared.";
}
void Core::saveConfigJson(QString filePath)
{
    if (filePath.isEmpty()) return;

    // 1. 建立 JSON 物件並填入資料
    QJsonObject root;
    root.insert("Height", m_height);

    // 從 m_Spec 取得目前的積分時間與平均次數
    if (m_Spec) {
        root.insert("IntegrationTime", m_Spec->Time); // 假設變數名為此
        root.insert("AverageCount", m_Spec->Avg);
    }
    root.insert("Timestamp", QDateTime::currentDateTime().toString(Qt::ISODate));

    // 2. 轉換為文件
    QJsonDocument doc(root);

    // 3. 寫入檔案
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "JSON Config saved to:" << filePath;
    }
    else {
        qDebug() << "Failed to save JSON:" << file.errorString();
    }
}