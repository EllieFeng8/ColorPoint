#include "core.h"

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

    QObject::connect(m_proxy, &ColorPointProxy::connectBtnChanged, m_Spec, &Spectrometer::OpenSpectrometer,Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::integrationTimeChanged, m_Spec, &Spectrometer::SetIntegrationTime, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::avgTimeChanged, m_Spec,&Spectrometer::SetAVG, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::scanBtnChanged, m_Spec, &Spectrometer::Scan, Qt::QueuedConnection);
    QObject::connect(threadSusi, &QThread::started, m_susi, &susi_control::init);
    QObject::connect(threadModbus, &QThread::started, m_modbus, &ModbusHandler::init);
    QObject::connect(m_Spec, &Spectrometer::DataIntensity, this, &Core::onDataAcquires);
    QObject::connect(m_modbus, &ModbusHandler::dataReceived, this, &Core::onModbusDataReceived);
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
void Core::reset_count()
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