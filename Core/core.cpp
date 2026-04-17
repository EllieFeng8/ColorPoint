#include "core.h"
#include <cmath>  
#include <cstdint> 

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

    QObject::connect(m_proxy, &ColorPointProxy::connectBtnChanged, m_Spec, &Spectrometer::OpenSpectrometer,Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::integrationTimeChanged, m_Spec, &Spectrometer::SetIntegrationTime, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::avgTimeChanged, m_Spec,&Spectrometer::SetAVG, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::scanBtnChanged, m_Spec, &Spectrometer::Scan, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::heightSetChanged, this, &Core::setHeight, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::resetBtnChanged, this, &Core::onResetBtnChanged, Qt::QueuedConnection);

    QObject::connect(threadSusi, &QThread::started, m_susi, &susi_control::init);
    QObject::connect(threadModbus, &QThread::started, m_modbus, &ModbusHandler::init);
    QObject::connect(m_Spec, &Spectrometer::DataIntensity, this, &Core::onDataAcquires);
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
    m_height = h;

    // qDebug() << m_height;


    // 如果沒開啟自動高度模式，直接結束
    if (!m_AutoHeight || m_targetHeight == 0)
    {
        if (m_currentDirection != 3) {
            //this->controlInverter(3);
            m_currentDirection = 3; // 重設狀態
        }

        return;
    }

    // 計算差距
    double diff = m_height - m_targetHeight;

     //1. 到達目標範圍 (容許誤差 0.25)
    if (qAbs(diff) <= 0.25)
    {
        if (m_currentDirection != 3) // 只有在「不是停止狀態」時才發送停止指令
        {
            m_currentDirection = 3;    // 標記目前已停止
            //this->controlInverter(3);
            QMetaObject::invokeMethod(m_susi, [this]()
                {
                    m_susi->setGPIO2(0); //確認上升停止
                    m_susi->setGPIO3(0); //開始下降
                });
            m_AutoHeight = false;      // 到達目的地，關閉自動模式
            qDebug() << "Target Reached. Stopped.";
            return;
        }
    }
    // 2. 目前太高 -> 需要下降 (假設 2 是反轉/下降)
    else if (m_height > m_targetHeight)
    {
        if (m_currentDirection != 2) 
        {
            m_currentDirection = 2;    

            //this->controlInverter(2);
            if (m_susi)
            {
                // 使用 QMetaObject::invokeMethod 確保跨執行緒呼叫安全
                QMetaObject::invokeMethod(m_susi, [this]()
                    {
                        m_susi->setGPIO2(0); //確認上升停止
                        m_susi->setGPIO3(1); //開始下降
                    });
            }
            qDebug() << "Moving Down...";
        }
    }
    // 3. 目前太低 -> 需要上升 (假設 1 是正轉/上升)
    else if (m_height < m_targetHeight)
    {
        if (m_currentDirection != 1) // 只有在「還沒發送上升指令」時才發送
        {
            m_currentDirection = 1;  

            //this->controlInverter(1);
            if (m_susi)
            {
                // 使用 QMetaObject::invokeMethod 確保跨執行緒呼叫安全
                QMetaObject::invokeMethod(m_susi, [this]()
                    {
                        m_susi->setGPIO3(0); //確認下降停止
                        m_susi->setGPIO2(1); //開始上升
                    });
            }
            qDebug() << "Moving Up...";
        }
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
