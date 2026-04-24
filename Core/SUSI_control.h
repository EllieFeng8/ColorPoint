#pragma once

#include <QObject>
#include <Qtimer>
#include <Qdatetime>
#include <qthread>
#include "Susi4.h"
#include "OsDeclarations.h"

class susi_control : public QObject
{
	Q_OBJECT

public:
	explicit susi_control(QObject* parent = nullptr){}
	~susi_control(){}

public slots:
	void init()
	{
		SusiLibInitialize();

		if (!m_timer) {
			m_timer = new QTimer(this);
			connect(m_timer, &QTimer::timeout, this, &susi_control::test);
			m_timer->start(5);
		}
	}
	void setDirection(int direction) {
		m_direction = direction;
	}
	void reset()
	{
		SusiGPIOSetDirection(3, 1, 0);
		SusiGPIOSetDirection(2, 1, 0);
		SusiGPIOSetLevel(3, 1, 0);//1=high 0=low
		auto status = SusiGPIOSetLevel(2,1, 1);//1=high 0=low
		qDebug() << "reset status =" << status << "set Direction"<< "bitmask 4  Direction 0";
		qDebug() << "reset status =" << status << "set Direction" << "bitmask 4  Level"<<1;

		m_WaitReset = true;

	}

	void setGPIO2(uint32_t value)
	{
		SusiGPIOSetDirection(2, 1, 0);
		SusiGPIOSetLevel(2, 1, value);//1=high 0=low
		qDebug()  << "set Direction" << "bitmask 4  Direction "<< 0;

	}
	void setGPIO3(uint32_t value)
	{
		SusiGPIOSetDirection(3, 1, 0);
		SusiGPIOSetLevel(3, 1, value);//1=high 0=low
		qDebug() << "set Direction" << "bitmask 8  Direction " << 0;
		qDebug() << "set Direction" << "bitmask 8  Level " << value;


	}
	void test()
	{
		SusiGPIOGetLevel(1, 1, &currentLevel); //id , pin , Level

		if (currentLevel == 1 && lastLevel == 0)
		{
			if (currentLevel == 1 && lastLevel == 0) // GPIO 觸發（上升緣）
			{
				// 根據 m_direction 判斷 count 增減
				if (m_direction == 1 && m_count > 0)
				{ // 正轉
					m_count--;
					qDebug() << "--" << m_count;
				}
				else if (m_direction == 2) 
				{ // 反轉
					m_count++;
					qDebug() << "++" << m_count;				}
				else 
				{ // 停止或未知狀態不計數
				  //qDebug() << "";
				}
			}
		}
		if (m_WaitReset) // 等待高度到最高點
		{

			if(currentLevel==0)
			{
				Check++;
				qDebug() << "Check" <<Check;
			}
			if(currentLevel==1)
			{
				Check = 0;
			}
			if (Check > 600)
			{
				setGPIO2(0);
				setGPIO3(0);

				m_count = 0;
				m_WaitReset = false;
			}
		}

		lastLevel = currentLevel;
		m_Height = 122.5 - m_count * 0.25;
		emit count(m_Height);
	}
	void uninit()
	{

		SusiLibUninitialize();
	}
	void stop()
	{
		if (m_timer) m_timer->stop();
		qDebug() << "Susi Timer stopped in thread:" << QThread::currentThreadId();
	}
signals:
	void count(int count);
private:
	uint32_t currentLevel = 0;
	uint32_t lastLevel = 1; // 預設為 High，確保第一次偵測正確
	uint32_t m_count = 0;
	int m_direction;
	QTimer* m_timer = nullptr;
	int m_Height = 0;
	bool m_WaitReset=false;
	int Check = 0;
};