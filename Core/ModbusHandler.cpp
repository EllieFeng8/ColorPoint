#include "ModbusHandler.h"
#include <QVariantList>
#include <QSerialPort>
#include <Qthread>

ModbusHandler::ModbusHandler(QObject* parent)
    : QObject(parent)
{
}

ModbusHandler::~ModbusHandler()
{
    ModbusHandler::stop();
}

void ModbusHandler::init()
{
    // 1. 初始化 Modbus RTU 客戶端
    if (!m_modbusClient) {
        m_modbusClient = new QModbusRtuSerialClient(this);
    }

    // 如果目前已經連線，先斷開
    if (m_modbusClient->state() == QModbusDevice::ConnectedState) {
        m_modbusClient->disconnectDevice();
    }

    // 2. 設定通訊參數 (建議未來可由 Proxy 傳入參數)
    // 這裡以常見的 COM 埠與 9600, 8N1 為例
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM3");
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud9600);
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);

    m_modbusClient->setTimeout(200);     // 等待回覆時間 (ms)
    m_modbusClient->setNumberOfRetries(3); // 失敗重試次數

    // 3. 嘗試連線
    if (!m_modbusClient->connectDevice()) {
        qDebug() << "Modbus Connect Failed:" << m_modbusClient->errorString();
        emit errorOccurred("Connect failed: " + m_modbusClient->errorString());
    }
    else {
        qDebug() << "Modbus Connected to COM3";

        // 4. 初始化定時讀取機制 (類似於你的 SUSI 或 Spectrometer 實作)
        if (!m_pollTimer) {
            m_pollTimer = new QTimer(this);
            connect(m_pollTimer, &QTimer::timeout, this, &ModbusHandler::readData);
        }
        m_pollTimer->start(300); // 每0.5秒讀取一次
    }
}

void ModbusHandler::readData()
{
    if (!m_modbusClient) return;

    if (m_modbusClient->state() != QModbusDevice::ConnectedState) {
        qDebug() << "Modbus device not connected.";
        return;
    }

    // 定義讀取內容：讀取 Holding Registers
    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 2001, 1);

    // 送出請求 (m_serverAddress 預設為 1)
    if (auto* reply = m_modbusClient->sendReadRequest(readUnit, m_serverAddress)) {
        if (!reply->isFinished()) {
            // 當收到回覆時觸發 onReadReady
            connect(reply, &QModbusReply::finished, this, &ModbusHandler::onReadReady);
        }
        else {
            // 如果請求立刻結束 (通常是錯誤)，刪除物件
            delete reply;
        }
    }
    else {
        qDebug() << "Read Request Error:" << m_modbusClient->errorString();
    }
}

void ModbusHandler::onReadReady()
{
    auto reply = qobject_cast<QModbusReply*>(sender());
    if (!reply) return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        QVector<quint16> values;

        for (int i = 0; i < (int)unit.valueCount(); i++) {
            values.append(unit.value(i));
        }
        quint16 addr2001 = unit.value(0); 

        // 發射訊號給 Core，進而更新 Proxy
        emit dataReceived(unit.startAddress(), values);
    }
    else if (reply->error() == QModbusDevice::ProtocolError) {
        qDebug() << "Modbus Protocol Error:" << reply->errorString();
    }
    else {
        qDebug() << "Modbus Other Error:" << reply->errorString();
    }

    reply->deleteLater(); // 釋放記憶體
}

void ModbusHandler::writeInverterControl(int command)
{
    if (!m_modbusClient || m_modbusClient->state() != QModbusDevice::ConnectedState) {
        qDebug() << "Cannot write: Modbus device not connected.";
        return;
    }
    if (m_pollTimer) m_pollTimer->stop();
    // 建立寫入單元：寫入至 HoldingRegisters, 起始地址 1001, 長度 1
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, 1001, 1);
    writeUnit.setValue(0, static_cast<quint16>(command));

    // 送出寫入請求
    if (auto* reply = m_modbusClient->sendWriteRequest(writeUnit, m_serverAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qDebug() << "Write Control Error:" << reply->errorString();
                    emit errorOccurred("Write failed: " + reply->errorString());
                }
                else {
                    qDebug() << "Inverter command" << reply->result().value(0) << "sent successfully to 1001";
                }

                if (m_pollTimer) m_pollTimer->start(500);
                reply->deleteLater();
                });
        }
        else {
            if (m_pollTimer) m_pollTimer->start(500);
            reply->deleteLater();
        }
    }
    else {
        qDebug() << "Send Write Request Error:" << m_modbusClient->errorString();
        if (m_pollTimer) m_pollTimer->start(500);
    }
}

void ModbusHandler::stop()
{
    if (m_pollTimer) {
        m_pollTimer->stop();
    }

    if (m_modbusClient) {
        m_modbusClient->disconnectDevice();
        qDebug() << "Modbus disconnected in thread:" << QThread::currentThreadId();
    }
}