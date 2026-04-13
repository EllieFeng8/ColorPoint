#pragma once

#include <QQmlEngine>
#include <QObject>
#include <Qmutex>
#include <Qvector>
#include "ColorPointProxy.h"
#include "Spectrometer.h"
#include "SUSI_control.h"
#include "ModbusHandler.h"
class Core : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    static Core& instance();
    ColorPointProxy* m_proxy = nullptr;
    
    QThread* threadSpec = nullptr;
    Spectrometer* m_Spec = nullptr;

    QThread* threadSusi = nullptr;
    susi_control* m_susi = nullptr;

    QThread* threadModbus = nullptr;
    ModbusHandler* m_modbus = nullptr;

    int m_IntegrationTime;
    int Avg;
    void init();
    void Core_connect();
    //void startAcq();
public slots:

    void setIntegrationTime(int v);
    void setAVG(int v);
    void reset_count();
    void OpenSpectrometer(bool v);
    void onDataAcquires(vector<float> values);
    void onModbusDataReceived(int address, QVector<quint16> data);
private:
    explicit Core(QObject* parent = nullptr);
    ~Core();

    bool direction = false;

};