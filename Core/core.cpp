#include "core.h"
#include <cmath>
#include <cstdint>
#include <QFileDialog>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include "SpecPredict.h"

Core::Core(QObject *parent)
    : QObject(parent) {
}

Core::~Core() {
    if (threadSpec && threadSpec->isRunning()) {
        if (m_Spec) {
            // 停止定時器並關閉設備
            QMetaObject::invokeMethod(m_Spec, [this] { m_Spec->stop(); }, Qt::BlockingQueuedConnection);

            QMetaObject::invokeMethod(m_Spec, [this] {
                m_Spec->CloseSpectrometer(0);
            }, Qt::BlockingQueuedConnection);

            m_Spec->deleteLater();
        }
        threadSpec->quit();
        if (!threadSpec->wait(3000)) {
            threadSpec->terminate();
            threadSpec->wait();
        }
    }

    if (threadSusi && threadSusi->isRunning()) {
        if (m_susi) {
            // 停止定時器並關閉設備
            QMetaObject::invokeMethod(m_susi, [this] { m_susi->stop(); }, Qt::BlockingQueuedConnection);

            QMetaObject::invokeMethod(m_susi, [this] {
                m_susi->uninit();
            }, Qt::BlockingQueuedConnection);

            m_susi->deleteLater();
        }


        threadSusi->quit();


        if (!threadSusi->wait(3000)) {
            threadSusi->terminate();
            threadSusi->wait();
        }
    }

    if (threadModbus && threadModbus->isRunning()) {
        if (m_modbus) {
            // 停止定時器並關閉設備
            QMetaObject::invokeMethod(m_modbus, [this] { m_modbus->stop(); }, Qt::BlockingQueuedConnection);

            m_modbus->deleteLater();
        }


        threadModbus->quit();


        if (!threadModbus->wait(3000)) {
            threadModbus->terminate();
            threadModbus->wait();
        }
    }
}

Core &Core::instance() {
    static Core inst;
    return inst;
}

void Core::init() {
    m_proxy = new ColorPointProxy(this);

    threadSpec = new QThread(this);
    m_Spec = new Spectrometer();
    m_Spec->moveToThread(threadSpec);
    threadSpec->start();

    threadSusi = new QThread(this);
    m_susi = new susi_control();
    m_susi->moveToThread(threadSusi);
    threadSusi->start();

    threadModbus = new QThread(this);
    m_modbus = new ModbusHandler();
    m_modbus->moveToThread(threadModbus);
    threadModbus->start();

    Core_connect();
    SpecPredict::instance().setBaseUrl(QUrl("http://192.168.1.200"));
}

void Core::Core_connect() {
    QObject::connect(m_proxy, &ColorPointProxy::inferHeightSetBtnChanged, this, [this](bool v) {
        setHeight(m_proxy->getInferHeight());
    }, Qt::QueuedConnection);

    QObject::connect(m_proxy, &ColorPointProxy::initInferenceSignal, this, [=]() {
        SpecPredict::instance().fetchCurrentParameters();
    }, Qt::QueuedConnection);

    QObject::connect(&SpecPredict::instance(), &SpecPredict::predictionFinished, this, [this]() {
        SpecPredict::PredictionResultData result = SpecPredict::instance().lastParsedPredictionResult();
        qDebug() << "get Result" << result.groupName;
        if (m_Spec->inference_pendingRecords.size() > 0) {
            m_Spec->inference_pendingRecords.back().prediction = result;
        }
        std::vector<QString> resultList;

        if (!result.predictions.isEmpty()) {
            const auto &prediction = result.predictions.first();
            for (const auto &item: prediction.results) {
                const QString value = item.hasPredictedValue
                                          ? QString::number(item.predictedValue)
                                          : item.predictedClass;
                resultList.emplace_back(item.itemName + " : " + value);
            }
        }

        m_proxy->setInferPredictList(resultList);
    });
    QObject::connect(&SpecPredict::instance(), &SpecPredict::currentParametersUpdated, this, [this]() {
        std::vector<QString> modelSetList;

        const auto &models = SpecPredict::instance().currentParameters().models;
        for (const auto &model: models) {
            modelSetList.emplace_back(model.modelName);
        }

        m_proxy->setInferModelSetList(modelSetList);
        m_proxy->setInferHeight(SpecPredict::instance().currentParameters().sampleCondition.heightCm);
        m_proxy->setInferAvgTime(SpecPredict::instance().currentParameters().sampleCondition.averageCount);
        m_proxy->setInferIntegrationTime(
            SpecPredict::instance().currentParameters().sampleCondition.integrationTimeUs / 1000);

        SpecPredict::instance().testPredict();
    });

    QObject::connect(m_proxy, &ColorPointProxy::inferMeasurePeriodChanged, m_Spec, &Spectrometer::setMeasurePeriod,
                     Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::labelChanged, m_Spec, &Spectrometer::SetLable, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::clearNirListBtnChanged, m_Spec, &Spectrometer::clearlist,
                     Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::clearBtnChanged, m_Spec, &Spectrometer::clearlist,
                     Qt::QueuedConnection);

    QObject::connect(m_proxy, &ColorPointProxy::autoBtnChanged, m_Spec, &Spectrometer::autoset, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::inferAutoScanBtnChanged, this, [this](bool v) {
        if (v) {
            if (m_Spec) {
                m_Spec->SetIntegrationTime(
                    SpecPredict::instance().currentParameters().sampleCondition.integrationTimeUs / 1000);
                m_Spec->SetAVG(SpecPredict::instance().currentParameters().sampleCondition.averageCount);
                QMetaObject::invokeMethod(m_Spec, [this]() {
                    m_Spec->StartContinuousAcq(500);
                });
            }
        } else {
            if (m_Spec) {
                QMetaObject::invokeMethod(m_Spec, [this]() {
                    m_Spec->StopContinuousAcq();
                });
            }
        }
    }, Qt::QueuedConnection);
    //QObject::connect(m_proxy, &ColorPointProxy::saveFileNameBtnChan, &Spectrometer::saveToCSV, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::saveFileNameBtnChanged, this, &Core::saveData, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::inferSaveBtnChanged, this, &Core::save_inferenceData,
                     Qt::QueuedConnection);

    //QObject::connect(m_proxy, &ColorPointProxy::autoSetHeightBtnChanged, this, [this](bool v)
    //    {
    //        m_AutoHeight = v;
    //    }, Qt::QueuedConnection);

    QObject::connect(m_proxy, &ColorPointProxy::connectBtnChanged, m_Spec, &Spectrometer::OpenSpectrometer,
                     Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::integrationTimeChanged, m_Spec, &Spectrometer::SetIntegrationTime,
                     Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::avgTimeChanged, m_Spec, &Spectrometer::SetAVG, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::scanBtnChanged, m_Spec, &Spectrometer::Scan, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::inferScanBtnChanged, m_Spec, &Spectrometer::inference_Scan,
                     Qt::QueuedConnection);
    //QObject::connect(m_proxy, &ColorPointProxy::, , &Spectrometer::OpenSpectrometer, Qt::QueuedConnection);

    QObject::connect(m_proxy, &ColorPointProxy::heightSetChanged, this, &Core::setHeight, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::resetBtnChanged, this, &Core::onResetBtnChanged, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::powerOnResetBtnChanged, this, &Core::onResetBtnChanged,
                     Qt::QueuedConnection);

    QObject::connect(m_proxy, &ColorPointProxy::whiteBtnChanged, this, &Core::whiteScan, Qt::QueuedConnection);
    QObject::connect(m_proxy, &ColorPointProxy::inferWhiteBtnChanged, this, &Core::inference_whiteScan,
                     Qt::QueuedConnection);

    QObject::connect(threadSusi, &QThread::started, m_susi, &susi_control::init);
    QObject::connect(threadModbus, &QThread::started, m_modbus, &ModbusHandler::init);
    QObject::connect(m_Spec, &Spectrometer::DataIntensity, this, &Core::onDataAcquires);
    QObject::connect(m_Spec, &Spectrometer::Inference_Data, this, &Core::onInference_Data);

    QObject::connect(m_Spec, &Spectrometer::isOpen, this, [this]() {
        m_proxy->setConnectedLight(true);
    });
    QObject::connect(m_Spec, &Spectrometer::currentTime, this, [this](int time) {
        m_proxy->setIntegrationTime(time / 1000);
    });
    QObject::connect(m_Spec, &Spectrometer::_intensity, this, &Core::onWhiteData);
    QObject::connect(m_modbus, &ModbusHandler::dataReceived, this, &Core::onModbusDataReceived);
    QObject::connect(m_susi, &susi_control::count, this, &Core::Height);
}

void Core::OpenSpectrometer(bool v) {
    //    qDebug() << "thy Search & Open ";
    //    m_Spec->SearchSpectrometer();
    //    m_Spec->OpenSpectrometer();
}

void Core::setIntegrationTime(int v) {
    //    m_Spec->SetIntegrationTime(0, v);
    //    m_IntegrationTime = v;
}

void Core::setAVG(int v) {
    //    Avg = v;
    //    m_Spec->SetAVG(v);
}

void Core::onDataAcquires(vector<float> values, int time) {
    m_proxy->setChartData(m_Spec->wavelength, values);
    //m_proxy->setIntegrationTime(time/1000);
}

void Core::onInference_Data(vector<float> values, int time) {
    m_proxy->setInferChartData(m_Spec->wavelength, values);
    //m_proxy->setIntegrationTime(time / 1000);
}

void Core::onModbusDataReceived(int address, QVector<quint16> data) {
    if (!data.isEmpty()) {
        int status = data[0]; // 1:正轉, 2:反轉, 3:停止
        //m_currentDirection = data[0];
        // 1. 將方向狀態傳遞給 susi_control 執行緒
        if (m_susi) {
            // 使用 QMetaObject::invokeMethod 確保跨執行緒呼叫安全
            QMetaObject::invokeMethod(m_susi, [this, status]() {
                m_susi->setDirection(status);
            });
        }
    }
}

void Core::Height(int h) {
    //if (m_height != h)
    //{
    m_height = h;
    //    if (!m_Spec->m_pendingRecords.empty())
    //    {
    //        qWarning() << "don't move";
    //    }
    //}
    if (m_proxy) {
        if (h > 120) {
            m_proxy->setHeight(120);
            m_proxy->setInferCurrentHeight(120);
        } else {
            m_proxy->setHeight(h);
            m_proxy->setInferCurrentHeight(h);
        }
    } // qDebug() << m_height;

    if (!m_AutoHeight || m_targetHeight == 0) {
        if (m_currentDirection != 3) {
            //m_susi->setGPIO2(0);
            //m_susi->setGPIO3(0);
            m_currentDirection = 3; // 重設狀態
            qDebug() << "!m_AutoHeight || m_targetHeight == 0";
        }
        return;
    }

    // 計算差距
    double diff = h - m_targetHeight;

    //1. 到達目標範圍 (容許誤差 0.25)
    if (qAbs(diff) <= 0.25) {
        m_currentDirection = 3; // 標記目前已停止
        //this->controlInverter(3);
        QMetaObject::invokeMethod(m_susi, [this]() {
            m_susi->setGPIO2(0);
            m_susi->setGPIO3(0);
        });
        m_AutoHeight = false;
        qDebug() << "Target Reached. Stopped.";
        return;
    }
    // 2. 目前太高 -> 需要下降 (假設 2 是反轉/下降)
    else if (h > m_targetHeight) {
        m_currentDirection = 2;

        //this->controlInverter(2);
        if (m_susi) {
            QMetaObject::invokeMethod(m_susi, [this]() {
                m_susi->setGPIO2(0); //確認上升停止
                m_susi->setGPIO3(1); //開始下降
            });
        }
        qDebug() << "Moving Down...";
    }
    // 3. 目前太低 -> 需要上升 (假設 1 是正轉/上升)
    else if (h < m_targetHeight) {
        m_currentDirection = 1;

        //this->controlInverter(1);
        if (m_susi) {
            QMetaObject::invokeMethod(m_susi, [this]() {
                m_susi->setGPIO3(0); //確認下降停止
                m_susi->setGPIO2(1); //開始上升
            });
        }
        qDebug() << "Moving Up...";
    }
}


void Core::setHeight(int h) {
    m_targetHeight = h;
    m_AutoHeight = true; // 開啟
}

void Core::controlInverter(int command) {
    //485
    //if (m_modbus) {
    //    //qDebug() << m_targetHeight;
    //    // 跨執行緒安全呼叫 ModbusHandler 的 writeInverterControl
    //    QMetaObject::invokeMethod(m_modbus, [this, command]() {
    //        m_modbus->writeInverterControl(command);
    //        });
    //}
}

void Core::onResetBtnChanged(bool v) {
    if (m_susi) {
        // 使用 QMetaObject::invokeMethod 確保跨執行緒呼叫安全
        QMetaObject::invokeMethod(m_susi, [this]() {
            m_susi->reset();
        });
    }
}

void Core::whiteScan(bool v) {
    w_height = m_height;
    QMetaObject::invokeMethod(m_Spec, [this]() {
        m_Spec->WhiteScan(w_height, 100000, 1);
    });
}

void Core::inference_whiteScan(bool v) {
    w_height = m_height;
    QMetaObject::invokeMethod(m_Spec, [this]() {
        m_Spec->SetIntegrationTime(
            SpecPredict::instance().currentParameters().whiteRefCondition.integrationTimeUs / 1000);
        m_Spec->SetAVG(SpecPredict::instance().currentParameters().whiteRefCondition.averageCount);
        m_Spec->inference_WhiteScan(w_height, 100000, 1);
    });
}

void Core::saveData(bool v) {
    //if (m_Spec->m_pendingRecords.empty()) {
    //    qDebug() << "No data to save.";
    //    return;
    //}
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/tmp.csv";
    QString csvPath = QFileDialog::getSaveFileName(
        nullptr,
        "select Path",
        initialPath,
        "CSV Files (*.csv);;All Files (*)"
    );

    if (csvPath.isEmpty()) {
        return; // 使用者取消選擇
    }
    QFile file(csvPath);
    bool fileExists = file.exists();

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing";
        return;
    }

    QTextStream out(&file);

    // 1. 寫入表頭 (僅在檔案剛建立時)
    if (!fileExists || file.size() == 0) {
        out << "Time,Label,Wavelength";
        for (float w: m_Spec->wavelength) {
            out << "," << QString::number(w, 'f', 1); // 輸出波長作為欄位名
        }
        out << "\n";
    }

    // 2. 遍歷所有暫存記錄，確保每一筆掃描佔用一行
    for (const auto &record: m_Spec->m_pendingRecords) {
        out << record.time << "," << (record.label.isEmpty() ? "None" : record.label) << ",0";

        for (float val: record.data) {
            out << "," << QString::number(val, 'g', 10); // 每一筆資料點接在同一行後方
        }
        out << "\n"; // 這筆記錄結束，換行
    }

    file.close();
    QString jsonPath = csvPath;
    jsonPath.replace(".csv", ".json");
    saveConfigJson(jsonPath);

    // 3. 儲存完畢後清空記憶體，避免下次儲存時重複寫入舊資料
    //m_Spec->m_pendingRecords.clear();
    qDebug() << "All data saved to CSV and memory cleared.";
}

void Core::save_inferenceData(bool v) {
    //if (m_Spec->m_pendingRecords.empty()) {
    //    qDebug() << "No data to save.";
    //    return;
    //}
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/predict_tmp.csv";
    QString csvPath = QFileDialog::getSaveFileName(
        nullptr,
        "select Path",
        initialPath,
        "CSV Files (*.csv);;All Files (*)"
    );

    if (csvPath.isEmpty()) {
        return; // 使用者取消選擇
    }
    QFile file(csvPath);
    bool fileExists = file.exists();

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing";
        return;
    }

    QTextStream out(&file);
    QStringList predictionHeaders;
    for (const auto &record: m_Spec->inference_pendingRecords) {
        for (const auto &prediction: record.prediction.predictions) {
            for (const auto &result: prediction.results) {
                if (!result.itemName.isEmpty() && !predictionHeaders.contains(result.itemName)) {
                    predictionHeaders.append(result.itemName);
                }
            }
        }
    }

    // 1. 寫入表頭 (僅在檔案剛建立時)
    if (!fileExists || file.size() == 0) {
        out << "Time,Label";
        for (const QString &header: predictionHeaders) {
            out << "," << header;
        }
        for (float w: m_Spec->wavelength) {
            out << "," << QString::number(w, 'f', 1); // 輸出波長作為欄位名
        }
        out << "\n";
    }

    // 2. 遍歷所有暫存記錄，確保每一筆掃描佔用一行
    for (const auto &record: m_Spec->inference_pendingRecords) {
        out << record.time << "," << (record.label.isEmpty() ? "None" : record.label);

        for (const QString &header: predictionHeaders) {
            QString predictionValue;
            bool foundPrediction = false;
            for (const auto &prediction: record.prediction.predictions) {
                for (const auto &result: prediction.results) {
                    if (result.itemName == header) {
                        predictionValue = result.hasPredictedValue
                                              ? QString::number(result.predictedValue, 'g', 15)
                                              : result.predictedClass;
                        foundPrediction = true;
                        break;
                    }
                }
                if (foundPrediction) {
                    break;
                }
            }
            out << "," << predictionValue;
        }
        for (float val: record.data) {
            out << "," << QString::number(val, 'g', 10); // 每一筆資料點接在同一行後方
        }
        out << "\n"; // 這筆記錄結束，換行
    }

    file.close();
    QString jsonPath = csvPath;
    jsonPath.replace(".csv", ".json");
    saveConfigJson(jsonPath);

    // 3. 儲存完畢後清空記憶體，避免下次儲存時重複寫入舊資料
    //m_Spec->inference_pendingRecords.clear();
    qDebug() << "All data saved to CSV and memory cleared.";
}

void Core::saveConfigJson(QString filePath) {
    if (filePath.isEmpty()) return;

    // 1. 建立最內層的 raw_samples 陣列
    QJsonArray rawSamplesArray;
    // 這裡示範如何從你的資料源（假設是 m_Spec 內的暫存資料）建立陣列
    // 如果你有實際的樣本清單，可以在這裡跑迴圈
    /*
    for (const auto& sample : m_Spec->samples) {
        QJsonObject sampleObj;
        sampleObj.insert("height_cm", sample.height);
        sampleObj.insert("intensity", sample.intensity);
        rawSamplesArray.append(sampleObj);
    }
    */

    // 為了符合你的範例格式，手動加入幾筆測試資料
    QJsonObject s1;
    s1.insert("height_cm", 110);
    s1.insert("intensity", 1000);
    QJsonObject s2;
    s2.insert("height_cm", 111);
    s2.insert("intensity", 1500);
    rawSamplesArray.append(s1);
    rawSamplesArray.append(s2);

    // 2. 建立 white_ref 物件
    QJsonObject whiteRefObj;
    whiteRefObj.insert("average_count", m_Spec->Avg); // 從光譜儀實體取得
    whiteRefObj.insert("height_cm", w_height); // 目前高度
    whiteRefObj.insert("integration_time_us", m_Spec->Time);
    whiteRefObj.insert("raw_samples", rawSamplesArray);

    // 3. 建立最外層的 Root 物件
    QJsonObject root;
    root.insert("average_count", m_Spec->Avg);
    root.insert("height_cm", m_height);
    root.insert("integration_time_us", m_Spec->Time);
    root.insert("white_ref", whiteRefObj);

    // 4. 寫入檔案
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(root);
        file.write(doc.toJson(QJsonDocument::Indented)); // 使用縮排格式，方便人類閱讀
        file.close();
        qDebug() << "JSON OK";
    }
}

void Core::onWhiteData(double currentHeight, float currentIntensity) {
    // 1. 取得檔案路徑 (通常位於桌面或程式目錄)
    QString jsonPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/data_log.json";

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "can not open .json";
        return;
    }

    // 2. 解析 JSON 內容
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray samples = root["white_ref"].toObject()["raw_samples"].toArray();

    // 3. 尋找高度最接近的樣本
    double minDiff = 9999.0;
    QJsonObject bestMatch;

    for (int i = 0; i < samples.size(); ++i) {
        QJsonObject sample = samples[i].toObject();
        double sampleHeight = sample["height_cm"].toDouble();
        double diff = qAbs(sampleHeight - currentHeight);

        if (diff < minDiff) {
            minDiff = diff;
            bestMatch = sample;
        }
    }

    // 4. 進行數值比對與邏輯處理
    if (!bestMatch.isEmpty() && minDiff < 0.5) {
        // 容許誤差 0.5cm
        double refIntensity = bestMatch["intensity"].toDouble();
        double intensityDiff = qAbs(refIntensity - currentIntensity);


        qDebug() << "currentHeight:" << currentHeight << " (refHeight:" << bestMatch["height_cm"].toDouble() << ")";
        qDebug() << "currentIntensity:" << currentIntensity << " / refIntensity:" << refIntensity;

        // 如果強度偏差過大，可以觸發警告
        if (intensityDiff > (refIntensity * 0.3)) {
            // 偏差超過 30%
            qDebug() << "w_intensity error！";
            m_proxy->raiseAbnormal();
        }
    } else {
        qDebug() << "error。";
    }
}
