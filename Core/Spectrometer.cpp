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
			inference_intensity.resize(framesize);
			b_intensity.resize(framesize);
			w_intensity.resize(framesize);
			m_intensity.resize(framesize);
			auto_intensity.resize(framesize);

			//將intensity的大小設定為framesize
			UAI_SpectrometerWavelengthAcquire(handles[0], wavelength.data());
			emit isOpen();
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

void Spectrometer::SetLable(QString lable)
{
	m_label = lable;
}
void Spectrometer::DataAcquires(int dev ,int Time ,int average)
{
	if (handles[dev] != nullptr)
	{
		
		UAI_SpectrometerDataAcquires(handles[dev], Time, intensity.data(), average);
		

		//for (float val : intensity) {
		//	list.append(val);
		//}
		for (int i = 0; i < intensity.size(); i++)
		{
			float val = intensity[i]-b_intensity[i];
			m_intensity[i] = val;
		}
			ScanRecord record;
			record.time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
			record.label = m_label;
			record.data = m_intensity; // 直接拷貝當前的 vector 內容
			m_pendingRecords.push_back(record);
		
		cout << "Intensity DataAcquires FINISH" << endl << endl << endl;
		emit DataIntensity(m_intensity,Time);
	}
	else
	{
		cout << "error DataAcquires";
	}
}

void Spectrometer::inference_DataAcquires(int dev, int Time, int average)
{
	if (handles[dev] != nullptr)
	{

		UAI_SpectrometerDataAcquires(handles[dev], Time, inference_intensity.data(), average);


		//for (float val : intensity) {
		//	list.append(val);
		//}
		for (int i = 0; i < inference_intensity.size(); i++)
		{
			float val = inference_intensity[i]- b_intensity[i];
			m_intensity[i] = val;
		}
		ScanRecord record;
		record.time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
		record.label = m_label;
		record.data = m_intensity; // 直接拷貝當前的 vector 內容
		inference_pendingRecords.push_back(record);

		cout << "Intensity DataAcquires FINISH" << endl << endl << endl;
		emit Inference_Data(m_intensity,Time);
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
			emit DataIntensity(intensity,Time);
	}
}

// 黑校正 ShutterSwitch 並讀取一次

void Spectrometer::Scan()
{
	qDebug() << "on ScanBtn clicked";
	if (!handles.empty() && handles[0] != nullptr)
	{
		unsigned int x;

		UAI_SpectrometerGetShutterSwitch(handles[0], &x);// 1 = shutter enable;
		qDebug() << "shutter = " << x;
		UAI_SpectrometerSetShutterSwitch(handles[0], 0);// 1 = shutter enable;
		QTimer::singleShot(100, this, &Spectrometer::CaptureDarkOneshot);
	}
	else
	{
		qDebug() << "handles is empty or handles[0] = nullptr)";
	}
}
void Spectrometer::inference_Scan()
{
	qDebug() << "on ScanBtn clicked";
	if (!handles.empty() && handles[0] != nullptr)
	{
		unsigned int x;

		UAI_SpectrometerGetShutterSwitch(handles[0], &x);// 1 = shutter enable;
		qDebug() << "shutter = " << x;
		UAI_SpectrometerSetShutterSwitch(handles[0], 0);// 1 = shutter enable;
		QTimer::singleShot(100, this, &Spectrometer::inference_CaptureDarkOneshot);
	}
	else
	{
		qDebug() << "handles is empty or handles[0] = nullptr)";
	}
}
void Spectrometer::inference_CaptureDarkOneshot()
{
	if (!handles.empty() && handles[0] != nullptr)
	{
		UAI_SpectrometerDataAcquires(handles[0], Time, b_intensity.data(), 1);
		UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 0 = shutter disable;
		unsigned int x;
		UAI_SpectrometerGetShutterSwitch(handles[0], &x);// 1 = shutter enable;
		qDebug() << "shutter = " << x;
		qDebug() << "CaptureDarkOneshot Finish";

		QTimer::singleShot(100, this, [this]() {inference_DataAcquires(0, Time, Avg); });
	}
}
void Spectrometer::CaptureDarkOneshot()
{
	if (!handles.empty() && handles[0] != nullptr)
	{
		UAI_SpectrometerDataAcquires(handles[0], Time, b_intensity.data(), 1);
		UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 0 = shutter disable;
		unsigned int x;
		UAI_SpectrometerGetShutterSwitch(handles[0], &x);// 1 = shutter enable;
		qDebug() << "shutter = " << x;
		qDebug() << "CaptureDarkOneshot Finish";

		QTimer::singleShot(100, this, [this]() {DataAcquires(0, Time, Avg); });
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
	QTimer::singleShot(300, this, &Spectrometer::inference_CaptureDarkOneshot);

	Run = true;
	m_timer->start(MeasurePeriod); // 以指定的毫秒間隔執行
	qDebug() << "Continuous Acquisition Started with interval:" << MeasurePeriod;
}

void Spectrometer::StopContinuousAcq()
{
	if (m_timer && m_timer->isActive()) {
		m_timer->stop();
		Run = false;
		qDebug() << "Continuous Acquisition Stopped.";
	}
}

void Spectrometer::autoset(bool v)
{
	// 確保設備已開啟
	if (handles.empty() || handles[0] == nullptr) {
		qDebug() << "Autoset failed: No device handle.";
		return;
	}

	// 1. 確保 auto_intensity 空間足夠 (與 framesize 同大)
	if (auto_intensity.size() != framesize) {
		auto_intensity.resize(framesize);
	}

	// 進行 15 次讀取與調整循環
	for (int i = 0; i < 15; i++)
	{
		// 2. 獲取當前曝光時間下的資料
		// 注意：這裡使用的是成員變數 Time (單位通常為微秒)
		status = UAI_SpectrometerDataAcquires(handles[0], Time, auto_intensity.data(), Avg);

		if (status != 0) {
			qDebug() << "Data acquisition failed during autoset, error code:" << status;
			break;
		}

		// 3. 尋找目前光譜中的最大強度值 (Intensity)
		float maxVal = 0;
		for (float val : auto_intensity) {
			if (val > maxVal) maxVal = val;
		}

		// 4. 根據邏輯調整曝光時間 (Time)
		if (maxVal > 65535)
		{
			// 超過 65535：減少 30%
			Time = static_cast<int>(Time * 0.7);
		}
		else if (maxVal > 50000)
		{
			// 大於 50000 且小於等於 65535：減少 10%
			Time = static_cast<int>(Time * 0.9);
		}
		else if (maxVal < 45000)
		{
			// 小於 45000：增加 30%
			Time = static_cast<int>(Time * 1.3);
		}
		else
		{
			// 在 45000 ~ 50000 之間：已達理想範圍，直接結束循環
			qDebug() << "Optimization finished: Intensity within target range.";
			break;
		}

		if (Time > 1000000)
		{
			Time = 1000000; 
		}
		if (Time < 1)
		{
			Time = 1; // 避免時間變為 0 導致錯誤
		}
		emit currentTime(Time);

	}

	qDebug() << "Autoset finished. Final Integration Time:" << Time;
}
void Spectrometer::OnTimerAcq()
{
	if (!Run || handles.empty() || handles[0] == nullptr) return;
	inference_DataAcquires(0, Time, Avg);
}

void Spectrometer::WhiteScan(double height,int time,int avg)
{
	//emit _intensity(height,1000 );
	float max_val = 0;
	if (handles[0] != nullptr)
	{
		qDebug() << "test";
		UAI_SpectrometerDataAcquires(handles[0], Time, w_intensity.data(), Avg);
		ScanRecord record;
		record.time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
		record.label = "White";
		record.data = w_intensity; // 直接拷貝當前的 vector 內容
		m_pendingRecords.push_back(record);
		if (!w_intensity.empty()) 
		{
			max_val = *std::max_element(w_intensity.begin(), w_intensity.end());
			std::cout << "max val : " << max_val << std::endl;
		}
		emit _intensity(height,max_val);
		emit DataIntensity(w_intensity, time);
	}
}
void Spectrometer::inference_WhiteScan(double height, int time, int avg)
{
	//emit _intensity(height,1000 );
	float max_val = 0;
	if (handles[0] != nullptr)
	{
		qDebug() << "test";
		UAI_SpectrometerDataAcquires(handles[0], Time, w_intensity.data(), Avg);
		ScanRecord record;
		record.time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
		record.label = "White";
		record.data = w_intensity; // 直接拷貝當前的 vector 內容
		inference_pendingRecords.push_back(record);
		if (!w_intensity.empty())
		{
			max_val = *std::max_element(w_intensity.begin(), w_intensity.end());
			std::cout << "max val : " << max_val << std::endl;
		}
		emit _intensity(height, max_val);
		emit Inference_Data(w_intensity, time);

	}
}
void Spectrometer::clearlist(bool v)
{
	m_pendingRecords.clear();
	inference_pendingRecords.clear();
}
void Spectrometer::saveToCSV(bool v)
{
	if (m_pendingRecords.empty()) {
		qDebug() << "No data to save.";
		return;
	}

	QFile file("tmp.csv");
	bool fileExists = file.exists();

	if (!file.open(QIODevice::Append | QIODevice::Text)) {
		qDebug() << "Failed to open file for writing";
		return;
	}

	QTextStream out(&file);

	// 1. 寫入表頭 (僅在檔案剛建立時)
	if (!fileExists || file.size() == 0) {
		out << "Time,Label";
		for (float w : wavelength) {
			out << "," << QString::number(w, 'f', 1); // 輸出波長作為欄位名
		}
		out << "\n";
	}

	// 2. 遍歷所有暫存記錄，確保每一筆掃描佔用一行
	for (const auto& record : m_pendingRecords) {
		out << record.time << "," << (record.label.isEmpty() ? "None" : record.label);

		for (float val : record.data) {
			out << "," << QString::number(val, 'g', 10); // 每一筆資料點接在同一行後方
		}
		out << "\n"; // 這筆記錄結束，換行
	}

	file.close();

	// 3. 儲存完畢後清空記憶體，避免下次儲存時重複寫入舊資料
	m_pendingRecords.clear();
	qDebug() << "All data saved to CSV and memory cleared.";
}