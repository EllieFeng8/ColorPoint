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
		uint32_t supportInput, supportOutput;
		SusiLibInitialize();

		uint32_t d1 = 0; //0=output
		uint32_t d2 = 0; //0=output
		SusiGPIOSetDirection(1, 4, d1);
		SusiGPIOSetDirection(1, 8, d2);

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

		auto status = SusiGPIOSetLevel(1,8, 1);//1=high 0=low
		qDebug() << "reset status =" <<status;
		m_WaitReset = true;

	}

	void setGPIO2(uint32_t value)
	{
		SusiGPIOSetLevel(1, 4, value);//1=high 0=low

	}
	void setGPIO3(uint32_t value)
	{
		SusiGPIOSetLevel(1, 8, value);//1=high 0=low

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
			}
			if(currentLevel==1)
			{
				Check = 0;
			}
			if (Check > 600)
			{
				m_count = 0;
				m_WaitReset = false;
			}
		}

		lastLevel = currentLevel;
		m_Height = 120 - m_count * 0.25;
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