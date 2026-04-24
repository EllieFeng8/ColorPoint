#pragma once

#include <vector>
#include <iostream>
#include <QtWidgets>
#include "UserApplication.h"

typedef uint32_t UINT32;
class Spectrometer : public QObject
{
	Q_OBJECT
public:
	struct ScanResult {
		QString time;
		QString label;
		vector<float> data;
	};

	struct ScanRecord {
		QString time;
		QString label;
		std::vector<float> data;
	};
	std::vector<ScanRecord> m_pendingRecords;
	std::vector<ScanRecord> inference_pendingRecords;

	unsigned int status=0; 
	vector<uint32_t> VIDPID; //存放得到的VIDPID list
	vector<void*> handles; //儲存各設備的handle
	uint32_t buffersize=1;
	vector<float> intensity;
	vector<float> inference_intensity;

	vector<float> b_intensity;
	vector<float> w_intensity;
	vector<float> m_intensity;
	vector<float> auto_intensity;

	vector<float> wavelength;
	unsigned int time;
	unsigned short framesize;
	unsigned int count;
	QVariantList list;
	QString m_label;
	int Avg =1 ;
	int Time = 10000 ;
	int MeasurePeriod = 1000;
	bool Run = true;
	QTimer* m_timer = nullptr;
	QTimer* m_update = nullptr;
	Spectrometer()
	{
	}
public slots:
	QString SearchSpectrometer();//尋找設備
	void autoset(bool v);
	void clearlist(bool v);
	void saveToCSV(bool v);
	void OpenSpectrometer(bool v);//開啟
	void setMeasurePeriod(int s)
	{
		MeasurePeriod = s * 1000;
	}

	void CloseSpectrometer(int dev);//關閉

	void SetIntegrationTime( int Time);//設定積分時間

	void SetLable(QString lable);

	void DataAcquires(int dev, int Time, int average);//獲取資料
	void inference_DataAcquires(int dev, int Time, int average);//獲取資料
	
	void GetIntegrationTime(int dev);

	void StartContinuousAcq(int interval_ms); // 啟動連續讀取
	void StopContinuousAcq();                 // 停止連續讀取
	void OnTimerAcq();                        // 定時觸發的讀取動作

	void Scan();
	void inference_Scan();

	void WhiteScan(double height,int time,int avg);
	void inference_WhiteScan(double height, int time, int avg);

	void CaptureDarkOneshot();
	void inference_CaptureDarkOneshot();

	void SetAVG(int v)
	{
		Avg = v;
	}
	void SetTime(int v)
	{
		Time = v;
	}
	void start()
	{
		m_timer->start(20);
	}

	void stop()
	{
		if (m_timer) m_timer->stop();
		qDebug() << "Spec Timer stopped in thread:" << QThread::currentThreadId();
	}
	unsigned int check();

	void StartAcq();

signals:
	void isOpen();
	void infoGenerated(QString);
	void DataIntensity(vector<float> v ,int time);
	void Inference_Data(vector<float> v, int time);
	void currentTime(int time);
	void inference_DataIntensity(vector<float>);
	void _intensity(double h,float v);
private:
	QMutex m_mutex;
	
};


