#include "Spectrometer.h"
#include <iostream>


QString Spectrometer::SearchSpectrometer()
{
	QString result;
	buffersize = 0;
	UAI_SpectrometerGetDeviceList(&buffersize, nullptr); //取得 buffersize(設備數量)
	VIDPID.resize(buffersize * 2); //將VIDPID的大小設為buffersize的兩倍
	UAI_SpectrometerGetDeviceList(&buffersize, VIDPID.data());//寫入VIDPID
	for (int dev = 0; dev < buffersize; dev++)
	{
		unsigned int i;
		uint32_t vid = VIDPID[dev * 2];
		uint32_t pid = VIDPID[dev * 2 + 1];
		cout <<  "vid:" << vid << "pid" << pid << endl;
		status = UAI_SpectrometerGetDeviceAmount(vid, pid, &i);
		cout <<  endl << "數量" << i << endl;

		result += QString("Device %1  VID:%2 PID:%3  Count=%4\n")
			.arg(dev)
			.arg(vid)
			.arg(pid)
			.arg(i);
	}
	return result;
}

void Spectrometer::OpenSpectrometer(bool v)
{
	SearchSpectrometer();
		uint32_t vid = 1592;
		uint32_t pid = 2732;
		unsigned int bufsize;
		void* handle = nullptr;
		status = UAI_SpectrometerOpen(0, &handle, vid, pid);
		if (status == 0)//確認error code 為 0
		{
			handles.resize(buffersize) ;
			cout << "dev:" << 0 << "    handle=" << handle << endl;
			handles[0] = handle; //將每一台光譜儀的handle存入handles[];
			UAI_SpectromoduleGetFrameSize(handle, &framesize);

			cout << "取得Framesize   :" << framesize << endl << endl;
			wavelength.resize(framesize);
			intensity.resize(framesize);
			b_intensity.resize(framesize);
			w_intensity.resize(framesize);
			//將intensity的大小設定為framesize
			UAI_SpectrometerWavelengthAcquire(handles[0], wavelength.data());
			if (!m_timer) {
				m_timer = new QTimer(this);
				connect(m_timer, &QTimer::timeout, this, &Spectrometer::StartAcq);
			}
		}
		else
		{
			cout << "erroropen:" << status << endl << endl; //顯示error code
		}
}



void Spectrometer::CloseSpectrometer(int dev)
{
	
	if (handles[dev] != nullptr)
	{
		UAI_SpectrometerClose(handles[dev]);
		handles[dev] = nullptr;
		cout << "dev:" << dev << " close" << endl;
	}
}

void Spectrometer::SetIntegrationTime( int time)
{
	Time = time*1000;
	qDebug() << Time;
	//設定積分時間
	//if (!handles.empty() && handles[0] != nullptr)
	//{
	//	UAI_SpectrometerSetIntegrationTime(handles[0], Time);
	//	cout << "Set IntegrationTime="<< Time << endl;
	//}
}

void Spectrometer::DataAcquires(int dev ,int Time ,int average)
{
	vector<float> m_intensity;
	m_intensity.resize(framesize);
	if (handles[dev] != nullptr)
	{
		
		UAI_SpectrometerDataAcquires(handles[dev], Time, intensity.data(), average);
		

		//for (float val : intensity) {
		//	list.append(val);
		//}
		for (int i = 0; i < intensity.size(); i++)
		{
			float val = intensity[i] - b_intensity[i];
			m_intensity[i] = (val < 0.0f) ? 0.0f : val;
			//cout << intensity[i] << endl << endl;
		}
		cout << "Intensity DataAcquires FINISH" << endl << endl << endl;
		emit DataIntensity(m_intensity);
	}
	else
	{
		cout << "error DataAcquires";
	}
}
void Spectrometer::GetIntegrationTime(int dev)
{
	UAI_SpectrometerGetIntegrationTime(handles[dev], &time);
}

unsigned int Spectrometer::check()
{
	uint32_t vid = 1592;
	uint32_t pid = 2732;
	UAI_SpectrometerGetDeviceAmount(vid, pid, &count);
	return count;
}

void Spectrometer::StartAcq()
{
	if (!handles.empty() && handles[0] != nullptr)
	{
		UAI_SpectrometerDataAcquires(handles[0], Time, intensity.data(), Avg);
			qDebug() << "Acq" << handles[0];
			qDebug() << "Time" << Time;
			//for (float val : intensity)
			//{
			//	qDebug() << val;
			//}
			emit DataIntensity(intensity);
	}
}

// 黑校正 ShutterSwitch 並讀取一次

void Spectrometer::Scan()
{
	qDebug() << "on ScanBtn clicked";
	if (!handles.empty() && handles[0] != nullptr)
	{
		UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 1 = shutter enable;
		QTimer::singleShot(300, this, &Spectrometer::CaptureDarkOneshot);
	}
	else
	{
		qDebug() << "handles is empty or handles[0] = nullptr)";
	}
}

void Spectrometer::CaptureDarkOneshot()
{
	if (!handles.empty() && handles[0] != nullptr)
	{
		UAI_SpectrometerDataAcquires(handles[0], Time, b_intensity.data(), Avg);
		UAI_SpectrometerSetShutterSwitch(handles[0], 0);// 0 = shutter disable;
		qDebug() << "CaptureDarkOneshot Finish";

		QTimer::singleShot(300, this, [this]() {DataAcquires(0, Time, Avg); });
	}
}

// 連續讀取

void Spectrometer::StartContinuousAcq(int interval_ms)
{
	if (handles.empty() || handles[0] == nullptr) {
		qDebug() << "No device handle available.";
		return;
	}

	if (m_timer && m_timer->isActive()) {
		m_timer->stop();
	}

	if (!m_timer) {
		m_timer = new QTimer(this);
		// 連接 Timer 的 timeout 訊號到讀取動作
		connect(m_timer, &QTimer::timeout, this, &Spectrometer::OnTimerAcq);
	}

	UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 1 = shutter enable
	QTimer::singleShot(300, this, &Spectrometer::CaptureDarkOneshot);

	Run = true;
	m_timer->start(interval_ms); // 以指定的毫秒間隔執行
	qDebug() << "Continuous Acquisition Started with interval:" << interval_ms;
}

void Spectrometer::StopContinuousAcq()
{
	if (m_timer && m_timer->isActive()) {
		m_timer->stop();
		Run = false;
		qDebug() << "Continuous Acquisition Stopped.";
	}
}

void Spectrometer::OnTimerAcq()
{
	if (!Run || handles.empty() || handles[0] == nullptr) return;
	DataAcquires(0, Time, Avg);
}

void Spectrometer::WhiteScan(int time,int avg)
{
	if (handles[0] != nullptr)
	{
		UAI_SpectrometerDataAcquires(handles[0], time, w_intensity.data(), avg);
		
	}
}