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
		count = 0;
	}
	void test()
	{
		 
		SusiGPIOGetLevel(1, 1, &currentLevel); //id 0  , pin 0 , Level

		if (currentLevel == 1 && lastLevel == 0)
		{
			if (currentLevel == 1 && lastLevel == 0) // GPIO 觸發（上升緣）
			{
				// 根據 m_direction 判斷 count 增減
				if (m_direction == 1) 
				{ // 正轉
					count++;
					qDebug() << "++" << count;
				}
				else if (m_direction == 2&&count>0) 
				{ // 反轉
					count--;
					qDebug() << "--" << count;
				}
				else 
				{ // 停止或未知狀態不計數
				  //qDebug() << "";
				}
			}
		}
		lastLevel = currentLevel;
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
private:
	uint32_t currentLevel = 0;
	uint32_t lastLevel = 1; // 預設為 High，確保第一次偵測正確
	uint32_t count = 0;
	int m_direction;
	QTimer* m_timer = nullptr;
};