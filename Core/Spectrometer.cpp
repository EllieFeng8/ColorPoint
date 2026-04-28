#include "Spectrometer.h"
#include <iostream>


QString Spectrometer::SearchSpectrometer()
{
	QString result;
	buffersize = 0;
	UAI_SpectrometerGetDeviceList(&buffersize, nullptr); //���o buffersize(�]�Ƽƶq)
	VIDPID.resize(buffersize * 2); //�NVIDPID���j�p�]��buffersize���⭿
	UAI_SpectrometerGetDeviceList(&buffersize, VIDPID.data());//�g�JVIDPID
	for (int dev = 0; dev < buffersize; dev++)
	{
		unsigned int i;
		uint32_t vid = VIDPID[dev * 2];
		uint32_t pid = VIDPID[dev * 2 + 1];
		cout <<  "vid:" << vid << "pid" << pid << endl;
		status = UAI_SpectrometerGetDeviceAmount(vid, pid, &i);
		cout <<  endl << "�ƶq" << i << endl;

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
		if (status == 0)//�T�{error code �� 0
		{
			handles.resize(buffersize) ;
			cout << "dev:" << 0 << "    handle=" << handle << endl;
			handles[0] = handle; //�N�C�@�x���л���handle�s�Jhandles[];
			UAI_SpectromoduleGetFrameSize(handle, &framesize);

			cout << "���oFramesize   :" << framesize << endl << endl;
			wavelength.resize(framesize);
			intensity.resize(framesize);
			inference_intensity.resize(framesize);
			b_intensity.resize(framesize);
			w_intensity.resize(framesize);
			m_intensity.resize(framesize);
			auto_intensity.resize(framesize);

			//�Nintensity���j�p�]�w��framesize
			UAI_SpectrometerWavelengthAcquire(handles[0], wavelength.data());
			emit isOpen();
		}
		else
		{
			cout << "erroropen:" << status << endl << endl; //���error code
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
	//�]�w�n���ɶ�
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
		unsigned int x;
		UAI_SpectrometerGetShutterSwitch(handles[0], &x);// 1 = shutter enable;
		qDebug() << "shutter = " << x;
		qDebug() << "CaptureDarkOneshot Finish";
		UAI_SpectrometerDataOneshot(handles[dev], Time, intensity.data(), average);
		// UAI_SpectrometerDataAcquires(handles[dev], Time, intensity.data(), average);


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
			record.data = m_intensity; // �����������e�� vector ���e
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

		// UAI_SpectrometerDataAcquires(handles[dev], Time, inference_intensity.data(), average);
		UAI_SpectrometerDataOneshot(handles[dev], Time, inference_intensity.data(), average);


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
		record.data = m_intensity; // �����������e�� vector ���e
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

// �®ե� ShutterSwitch ��Ū���@��

void Spectrometer::Scan()
{
	qDebug() << "on ScanBtn clicked";
	if (!handles.empty() && handles[0] != nullptr)
	{
		UAI_SpectrometerSetShutterSwitch(handles[0], 0);// 1 = shutter enable;
		QTimer::singleShot(20, this, &Spectrometer::CaptureDarkOneshot);
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
		QTimer::singleShot(20, this, &Spectrometer::inference_CaptureDarkOneshot);
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
		// UAI_SpectrometerDataAcquires(handles[0], Time, b_intensity.data(), 1);
		UAI_SpectrometerDataOneshot(handles[0], Time, b_intensity.data(), 1);
		UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 0 = shutter disable;
		unsigned int x;
		UAI_SpectrometerGetShutterSwitch(handles[0], &x);// 1 = shutter enable;
		qDebug() << "shutter = " << x;
		qDebug() << "inference_CaptureDarkOneshot Finish";

		QTimer::singleShot(20, this, [this]() {inference_DataAcquires(0, Time, Avg); });
	}
}
void Spectrometer::CaptureDarkOneshot()
{
	if (!handles.empty() && handles[0] != nullptr)
	{
		unsigned int x;
		UAI_SpectrometerGetShutterSwitch(handles[0], &x);// 1 = shutter enable;
		qDebug() << "shutter = " << x;
		UAI_SpectrometerDataOneshot(handles[0], Time, b_intensity.data(), 1);
		// UAI_SpectrometerDataAcquires(handles[0], Time, b_intensity.data(), 1);
		UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 0 = shutter disable;

		QTimer::singleShot(20, this, [this]() {DataAcquires(0, Time, Avg); });
	}
}
// �s��Ū��

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
		// �s�� Timer �� timeout �T����Ū���ʧ@
		connect(m_timer, &QTimer::timeout, this, &Spectrometer::OnTimerAcq);
	}

	UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 1 = shutter enable
	QTimer::singleShot(300, this, &Spectrometer::inference_CaptureDarkOneshot);

	Run = true;
	m_timer->start(MeasurePeriod); // �H���w���@�����j����
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
	// �T�O�]�Ƥw�}��
	if (handles.empty() || handles[0] == nullptr) {
		qDebug() << "Autoset failed: No device handle.";
		return;
	}

	// 1. �T�O auto_intensity �Ŷ����� (�P framesize �P�j)
	if (auto_intensity.size() != framesize) {
		auto_intensity.resize(framesize);
	}

	// �i�� 15 ��Ū���P�վ�`��
	for (int i = 0; i < 15; i++)
	{
		// 2. ������e�n���ɶ��U�����
		// �`�N�G�o�̨ϥΪ��O�����ܼ� Time (���q�`���L��)
		status = UAI_SpectrometerDataAcquires(handles[0], Time, auto_intensity.data(), Avg);

		if (status != 0) {
			qDebug() << "Data acquisition failed during autoset, error code:" << status;
			break;
		}

		// 3. �M��ثe���Ф����̤j�j�׭� (Intensity)
		float maxVal = 0;
		for (float val : auto_intensity) {
			if (val > maxVal) maxVal = val;
		}

		// 4. �ھ��޿�վ��n���ɶ� (Time)
		if (maxVal > 65535)
		{
			// �W�L 65535�G��� 30%
			Time = static_cast<int>(Time * 0.7);
		}
		else if (maxVal > 50000)
		{
			// �j�� 50000 �B�p�󵥩� 65535�G��� 10%
			Time = static_cast<int>(Time * 0.9);
		}
		else if (maxVal < 45000)
		{
			// �p�� 45000�G�W�[ 30%
			Time = static_cast<int>(Time * 1.3);
		}
		else
		{
			// �b 45000 ~ 50000 �����G�w�F�z�Q�d��A���������`��
			qDebug() << "Optimization finished: Intensity within target range.";
			break;
		}

		if (Time > 1000000)
		{
			Time = 1000000;
		}
		if (Time < 1)
		{
			Time = 1; // �קK�ɶ��ܬ� 0 �ɭP���~
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
		// UAI_SpectrometerDataAcquires(handles[0], Time, w_intensity.data(), Avg);
		UAI_SpectrometerSetShutterSwitch(handles[0], 0);// 0 = shutter disable;
		QThread::msleep(20);
		UAI_SpectrometerDataOneshot(handles[0], Time, b_intensity.data(), 1);
		UAI_SpectrometerSetShutterSwitch(handles[0], 1);// 0 = shutter disable;
		QThread::msleep(20);
		UAI_SpectrometerDataOneshot(handles[0], Time, intensity.data(), Avg);
		ScanRecord record;
		for (int i = 0; i < intensity.size(); i++)
		{
			float val = intensity[i]-b_intensity[i];
			w_intensity[i] = val;
		}
		record.time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
		record.label = "White";
		record.data = w_intensity; // �����������e�� vector ���e
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
		record.data = w_intensity; // �����������e�� vector ���e
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

	// 1. �g�J���Y (�Ȧb�ɮ׭�إ߮�)
	if (!fileExists || file.size() == 0) {
		out << "Time,Label";
		for (float w : wavelength) {
			out << "," << QString::number(w, 'f', 1); // ��X�i���@�����W
		}
		out << "\n";
	}

	// 2. �M���Ҧ��Ȧs�O���A�T�O�C�@�����y���Τ@��
	for (const auto& record : m_pendingRecords) {
		out << record.time << "," << (record.label.isEmpty() ? "None" : record.label);

		for (float val : record.data) {
			out << "," << QString::number(val, 'g', 10); // �C�@������I���b�P�@����
		}
		out << "\n"; // �o���O�������A����
	}

	file.close();

	// 3. �x�s������M�ŰO����A�קK�U���x�s�ɭ��Ƽg�J�¸��
	m_pendingRecords.clear();
	qDebug() << "All data saved to CSV and memory cleared.";
}