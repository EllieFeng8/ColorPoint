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
    void whiteScan(bool v);
    void inference_whiteScan(bool v);

    void OpenSpectrometer(bool v);
    void onDataAcquires(vector<float> values,int time);
    void onInference_Data(vector<float> values, int time);
    void onWhiteData(double h,float v);
    void onModbusDataReceived(int address, QVector<quint16> data);
    void controlInverter(int command);
    void Height(int h);
    void setHeight(int h);
    void onResetBtnChanged(bool v);
    void saveData(bool v);
    void save_inferenceData(bool v);
    void saveConfigJson(QString filePath);
    //QJsonObject findRefDataByHeight(double currentHeight, QString filePath);
private:
    explicit Core(QObject* parent = nullptr);
    ~Core();
    bool m_AutoHeight=false;
    bool direction = false;
    double m_height = 0; 
    double w_height = 0;
    double m_targetHeight = 0;
    int m_currentDirection = 0;
    float white_intensity = 0.0;
};