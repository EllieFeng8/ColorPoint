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
    void OpenSpectrometer(bool v);
    void onDataAcquires(vector<float> values);
    void onModbusDataReceived(int address, QVector<quint16> data);
    void controlInverter(int command);
    void Height(int h);
    void setHeight(int h);
    void onResetBtnChanged(bool v);
private:
    explicit Core(QObject* parent = nullptr);
    ~Core();
    bool m_AutoHeight=false;
    bool direction = false;
    int m_height = 0;
    int m_targetHeight = 0;
    int m_currentDirection = 0;
};