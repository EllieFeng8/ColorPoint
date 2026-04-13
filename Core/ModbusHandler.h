#pragma once

#include <QObject>
#include <QModbusRtuSerialMaster>
#include <QModbusDataUnit>
#include <QVariantList>
#include <QTimer>
#include <QDebug>

class ModbusHandler : public QObject
{
    Q_OBJECT
public:
    explicit ModbusHandler(QObject* parent = nullptr);
    ~ModbusHandler();

public slots:
    void init();        // 初始化連線
    void stop();        // 停止通訊
    void readData();    // 執行讀取指令

private slots:
    void onReadReady(); // 處理讀取回傳的資料

signals:
    void dataReceived(int address, QVector<quint16> data);
    void errorOccurred(QString error);

private:
    QModbusRtuSerialClient* m_modbusClient = nullptr;
    QTimer* m_pollTimer = nullptr;
    int m_serverAddress = 1; // 預設 Slave ID
};