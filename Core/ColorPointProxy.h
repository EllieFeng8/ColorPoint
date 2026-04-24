#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <sstream>
#include <iomanip>
#include <QVariantList>
#include <string>
#include <QDir>
#include <QGuiApplication>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
class ColorPointProxy : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // ===== Array =====
    Q_PROPERTY(QVariantList chartData READ getChartData  NOTIFY chartDataChanged)
    Q_PROPERTY(QVariantList nirList   READ getNirList  NOTIFY nirListChanged)
    Q_PROPERTY(QVariantList whiteScanList   READ getWhiteScanList  NOTIFY whiteScanListChanged)

    // ===== Buttons / States =====
    Q_PROPERTY(bool clearBtn   READ getClearBtn   WRITE setClearBtn   NOTIFY clearBtnChanged)
    Q_PROPERTY(bool updateBtn  READ getUpdateBtn  WRITE setUpdateBtn  NOTIFY updateBtnChanged)
    Q_PROPERTY(bool connectBtn READ getConnectBtn WRITE setConnectBtn NOTIFY connectBtnChanged)
    Q_PROPERTY(bool connectedLight  READ getConnectedLight  WRITE setConnectedLight NOTIFY connectedLightChanged)
    Q_PROPERTY(bool savedLight READ getSavedLight WRITE setSavedLight NOTIFY savedLightChanged)
    Q_PROPERTY(bool confirmBtn READ getConfirmBtn WRITE setConfirmBtn NOTIFY confirmBtnChanged)
    Q_PROPERTY(bool autoBtn    READ getAutoBtn    WRITE setAutoBtn    NOTIFY autoBtnChanged)
    Q_PROPERTY(bool scanBtn    READ getScanBtn    WRITE setScanBtn    NOTIFY scanBtnChanged)
    Q_PROPERTY(bool whiteBtn   READ getWhiteBtn   WRITE setWhiteBtn   NOTIFY whiteBtnChanged)
    Q_PROPERTY(bool saveLabelBtn    READ getSaveLabelBtn    WRITE setSaveLabelBtn    NOTIFY saveLabelBtnChanged)
    Q_PROPERTY(bool saveFileNameBtn    READ getSaveFileNameBtn    WRITE setSaveFileNameBtn    NOTIFY saveFileNameBtnChanged)
    Q_PROPERTY(bool clearNirListBtn    READ getClearNirListBtn    WRITE setClearNirListBtn    NOTIFY clearNirListBtnChanged)
    Q_PROPERTY(bool powerOnResetBtn    READ getPowerOnResetBtn    WRITE setPowerOnResetBtn    NOTIFY powerOnResetBtnChanged)

    // ===== Settings =====
    Q_PROPERTY(int  integrationTime READ getIntegrationTime WRITE setIntegrationTime NOTIFY integrationTimeChanged)
    Q_PROPERTY(int  avgTime   READ getAvgTime   WRITE setAvgTime   NOTIFY avgTimeChanged)
    Q_PROPERTY(int  height   READ getHeight   WRITE setHeight   NOTIFY heightChanged)
    Q_PROPERTY(int  heightSet   READ getHeightSet  WRITE setHeightSet  NOTIFY heightSetChanged)
    Q_PROPERTY(bool  resetBtn   READ getResetBtn  WRITE setResetBtn NOTIFY resetBtnChanged)
    Q_PROPERTY(bool  autoSetHeightBtn   READ getAutoSetHeightBtn  WRITE setAutoSetHeightBtn NOTIFY autoSetHeightBtnChanged)

    // ===== Text =====
    Q_PROPERTY(QString label    READ getLabel    WRITE setLabel    NOTIFY labelChanged)
    Q_PROPERTY(QString whiteLabel    READ getWhiteLabel    WRITE setWhiteLabel    NOTIFY whiteLabelChanged)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)

    //======inference======
    Q_PROPERTY(bool inferConnectBtn READ getInferConnectBtn WRITE setInferConnectBtn NOTIFY inferConnectBtnChanged)
    Q_PROPERTY(bool inferConnectedLight  READ getInferConnectedLight  WRITE setInferConnectedLight NOTIFY inferConnectedLightChanged)

    Q_PROPERTY(int  inferIntegrationTime READ getInferIntegrationTime WRITE setInferIntegrationTime NOTIFY inferIntegrationTimeChanged)
    Q_PROPERTY(int  inferAvgTime   READ getInferAvgTime   WRITE setInferAvgTime   NOTIFY inferAvgTimeChanged)
    Q_PROPERTY(int  inferHeight   READ getInferHeight WRITE setInferHeight   NOTIFY inferHeightChanged)
    Q_PROPERTY(int  inferCurrentHeight   READ getInferCurrentHeight WRITE setInferCurrentHeight   NOTIFY inferCurrentHeightChanged)
    Q_PROPERTY(int  inferMeasurePeriod   READ getInferMeasurePeriod  WRITE setInferMeasurePeriod    NOTIFY inferMeasurePeriodChanged)

    Q_PROPERTY(bool inferScanBtn    READ getInferScanBtn    WRITE setInferScanBtn    NOTIFY inferScanBtnChanged)
    Q_PROPERTY(bool inferAutoScanBtn    READ getInferAutoScanBtn    WRITE setInferAutoScanBtn    NOTIFY inferAutoScanBtnChanged)
    Q_PROPERTY(bool inferWhiteBtn   READ getInferWhiteBtn   WRITE setInferWhiteBtn   NOTIFY inferWhiteBtnChanged)
    Q_PROPERTY(bool inferSaveBtn   READ getInferSaveBtn   WRITE setInferSaveBtn   NOTIFY inferSaveBtnChanged)
    Q_PROPERTY(bool inferHeightSetBtn   READ getInferHeightSetBtn   WRITE setInferHeightSetBtn   NOTIFY inferHeightSetBtnChanged)
    Q_PROPERTY(bool inferPeriodBtn   READ getInferPeriodBtn   WRITE setInferPeriodBtn   NOTIFY inferPeriodBtnChanged)
    Q_PROPERTY(QString inferLabel    READ getInferLabel    WRITE setInferLabel    NOTIFY inferLabelChanged)

    // =====inference Array =====
    Q_PROPERTY(QVariantList inferChartData READ getInferChartData  NOTIFY inferChartDataChanged)
    Q_PROPERTY(QVariantList inferNirList   READ getInferNirList  NOTIFY inferNirListChanged)
    Q_PROPERTY(QVariantList inferWhiteScanList   READ getInferWhiteScanList  NOTIFY inferWhiteScanListChanged)
    Q_PROPERTY(QVariantList inferPredictList   READ getInferPredictList  NOTIFY inferPredictListChanged)
    Q_PROPERTY(QVariantList inferModelSetList   READ getInferModelSetList  NOTIFY inferModelSetListChanged)
    Q_PROPERTY(QVariantList inferWhiteScanList   READ getInferWhiteScanList  NOTIFY inferWhiteScanListChanged)

public:

    explicit ColorPointProxy(QObject *parent = nullptr) : QObject(parent)
    {

        // QVariantMap item1;
        // QVariantMap item2;
        // item1["name"] = "target 1";
        // item1["data"] = "12.2";
        // item2["name"] = "target 2";
        // item2["data"] = "0000000";

        // m_inferPredictList.append(item1);
        // m_inferModelSetList.append(item2);


    }
    //保存路徑
    Q_INVOKABLE QUrl  lastFolderPath() const { return QUrl::fromLocalFile(m_lastFolderPath); }
    //保存文字名字
    Q_INVOKABLE QUrl  lastFolderName() const { return m_lastFolderName; }

    Q_INVOKABLE void saveCsvFile(const QString &folderPath,
                            const QString &csvContent)
    {
        saveCsvFileInternal(folderPath, csvContent, false);
    }

    Q_INVOKABLE void saveInferCsvFile(const QString &folderPath,
                            const QString &csvContent)
    {
        saveCsvFileInternal(folderPath, csvContent, true);
    }

    Q_INVOKABLE void saveInstrumentConfigFile(const QString &filePath)
    {
        saveInstrumentConfigFile(filePath, false);
    }

    Q_INVOKABLE void saveCurrentInstrumentConfigFile()
    {
        const QString jsonPath = QCoreApplication::applicationDirPath() + "/instrument_config.json";
        saveInstrumentConfigFile(jsonPath);
    }

    Q_INVOKABLE QVariantList getChartData() const { return m_chartData ; }

    Q_INVOKABLE void setChartData(const std::vector<float>& x,
                                    const std::vector<float>& y)
    {
        m_chartData.clear();
        m_chartData.reserve(static_cast<int>(x.size()));
        for (size_t i = 0; i < x.size(); ++i) {
            QVariantMap item;
            item.insert("x", x[i]);
            item.insert("y", y[i]);
            m_chartData.append(item);
        }
        //m_chartData = value;
        emit chartDataChanged();
    }
    Q_INVOKABLE QVariantList getNirList() const { return m_nirList   ; }

    Q_INVOKABLE void setNirList(const std::vector<QString>& datalist)
    {
        // 1. 清理並預留空間，減少記憶體重分配
        m_nirList.clear();
        m_nirList.reserve(static_cast<int>(datalist.size()));
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        QString label = getLabel();
        // 3. 填充數據
        for (const QString& data : datalist) {
            QVariantMap item;
            item.insert("Time", currentTime);
            item.insert("Label", label);
            item.insert("Wavelength", 0);
            item.insert("", data);
            m_nirList.append(item);
        }
        // 4. 通知界面更新
        emit nirListChanged();
    }

    Q_INVOKABLE QVariantList getWhiteScanList() const { return m_whiteScanList   ; }

    Q_INVOKABLE void setWhiteScanList(const std::vector<QString>& datalist)
    {
        // 1. 清理並預留空間，減少記憶體重分配
        m_whiteScanList.clear();
        m_whiteScanList.reserve(static_cast<int>(datalist.size()));
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        QString label = "white";

        // 3. 填充數據
        for (const QString& data : datalist) {
            QVariantMap item;
            item.insert("time", currentTime);
            item.insert("label", label);
            item.insert("Wavelength", 0);
            item.insert("listData", data);

            m_whiteScanList.append(item);
        }
        // 4. 通知界面更新
        emit whiteScanListChanged();
    }
    Q_INVOKABLE bool getClearBtn() const { return m_clearBtn; }
    Q_INVOKABLE void setClearBtn(bool value)
    {
        m_clearBtn = value;
        emit clearBtnChanged(m_clearBtn);
    }
    Q_INVOKABLE bool getUpdateBtn() const { return m_updateBtn; }
    Q_INVOKABLE void setUpdateBtn(bool value)
    {
        m_updateBtn = value;
        emit updateBtnChanged(m_updateBtn);
    }
    Q_INVOKABLE bool getConnectBtn() const { return m_connectBtn ; }
    Q_INVOKABLE void setConnectBtn(bool value)
    {
        m_connectBtn = value;
        qDebug() << value << "connectBtnChanged" << m_connectBtn;
        emit connectBtnChanged(m_connectBtn);
    }
    Q_INVOKABLE bool getConnectedLight() const { return m_connectedLight   ; }
    Q_INVOKABLE void setConnectedLight(bool value)
    {
        m_connectedLight = value;
        emit connectedLightChanged(m_connectedLight);
    }
    Q_INVOKABLE bool getSavedLight() const { return m_savedLight ; }
    Q_INVOKABLE void setSavedLight(bool value)
    {
        m_savedLight = value;
        emit savedLightChanged(m_savedLight);
    }
    Q_INVOKABLE bool getConfirmBtn() const { return m_confirmBtn; }
    Q_INVOKABLE void setConfirmBtn(bool value)
    {
        m_confirmBtn = value;
        emit confirmBtnChanged(m_confirmBtn);
    }
    Q_INVOKABLE bool getAutoBtn() const { return m_autoBtn ; }
    Q_INVOKABLE void setAutoBtn(bool value)
    {
        m_autoBtn = value;
        emit autoBtnChanged(m_autoBtn);
    }

    Q_INVOKABLE bool getScanBtn() const { return m_scanBtn   ; }
    Q_INVOKABLE void setScanBtn(bool value)
    {
        m_scanBtn = value;
        qDebug() << value << "scanBtnChanged" << m_scanBtn;
        emit scanBtnChanged(m_scanBtn);
    }
    Q_INVOKABLE bool getWhiteBtn() const { return m_whiteBtn ; }
    Q_INVOKABLE void setWhiteBtn(bool value)
    {
        m_whiteBtn = value;
        emit whiteBtnChanged(m_whiteBtn);
    }

    Q_INVOKABLE bool getSaveLabelBtn() const { return m_saveLabelBtn   ; }
    Q_INVOKABLE void setSaveLabelBtn(bool value)
    {
        m_saveLabelBtn = value;
        emit saveLabelBtnChanged(m_saveLabelBtn);
    }
    Q_INVOKABLE bool getSaveFileNameBtn() const { return m_saveFileNameBtn   ; }
    Q_INVOKABLE void setSaveFileNameBtn(bool value)
    {
        m_saveFileNameBtn = value;
        emit saveFileNameBtnChanged(m_saveFileNameBtn);
    }
    Q_INVOKABLE bool getClearNirListBtn() const { return m_clearNirListBtn   ; }
    Q_INVOKABLE void setClearNirListBtn(bool value)
    {
        m_clearNirListBtn = value;
        emit clearNirListBtnChanged(m_clearNirListBtn);
    }
    Q_INVOKABLE bool getPowerOnResetBtn() const { return m_powerOnResetBtn   ; }
    Q_INVOKABLE void setPowerOnResetBtn(bool value)
    {
        m_powerOnResetBtn = value;
        emit powerOnResetBtnChanged(m_powerOnResetBtn);
    }
    Q_INVOKABLE int getIntegrationTime() const { return m_integrationTime ; }
    Q_INVOKABLE void setIntegrationTime(int value)
    {
        m_integrationTime = value;
        emit integrationTimeChanged(m_integrationTime);
    }
    Q_INVOKABLE int getAvgTime() const { return m_avgTime   ; }
    Q_INVOKABLE void setAvgTime(int value)
    {
        m_avgTime = value;
        emit avgTimeChanged(m_avgTime);
    }
    Q_INVOKABLE int getHeight() const { return m_height   ; }
    Q_INVOKABLE void setHeight(int value)
    {
        m_height = value;
        emit heightChanged(m_height);
    }
    Q_INVOKABLE int getHeightSet() const { return m_heightSet   ; }
    Q_INVOKABLE void setHeightSet(int value)
    {
        m_heightSet = value;
        emit heightSetChanged(m_heightSet);
    }
    Q_INVOKABLE bool getResetBtn() const { return m_resetBtn  ; }
    Q_INVOKABLE void setResetBtn(bool value)
    {
        m_resetBtn = value;
        emit resetBtnChanged(m_resetBtn);
    }

    Q_INVOKABLE bool getAutoSetHeightBtn() const { return m_autoSetHeightBtn  ; }
    Q_INVOKABLE void setAutoSetHeightBtn(bool value)
    {
        m_autoSetHeightBtn = value;
        emit autoSetHeightBtnChanged(m_autoSetHeightBtn);
    }

    Q_INVOKABLE QString getLabel() const { return m_label ; }
    Q_INVOKABLE void setLabel(const QString &value)
    {
        m_label = value;
        emit labelChanged(m_label);
    }

    Q_INVOKABLE QString getWhiteLabel() const { return m_whiteLabel ; }
    Q_INVOKABLE void setWhiteLabel(const QString &value)
    {
        m_whiteLabel = value;
        emit whiteLabelChanged(m_whiteLabel);
    }
    Q_INVOKABLE QString getFileName() const { return m_fileName   ; }
    Q_INVOKABLE void setFileName(const QString &value)
    {
        m_fileName = value;
        emit fileNameChanged(m_fileName);
    }


    //=====inference======
    Q_INVOKABLE bool getInferConnectBtn() const { return m_inferConnectBtn ; }
    Q_INVOKABLE void setInferConnectBtn(bool value)
    {
        m_inferConnectBtn = value;
        qDebug() << value << "InferConnectBtnconnectBtnChanged" << m_inferConnectBtn;
        emit inferConnectBtnChanged(m_inferConnectBtn);
    }
    Q_INVOKABLE bool getInferConnectedLight() const { return m_inferConnectedLight   ; }
    Q_INVOKABLE void setInferConnectedLight(bool value)
    {
        m_connectedLight = value;
        emit inferConnectedLightChanged(m_inferConnectedLight);
    }
    Q_INVOKABLE int getInferIntegrationTime() const { return m_inferIntegrationTime ; }
    Q_INVOKABLE void setInferIntegrationTime(int value)
    {
        m_inferIntegrationTime = value;
        emit inferIntegrationTimeChanged(m_inferIntegrationTime);
    }
    Q_INVOKABLE int getInferAvgTime() const { return m_inferAvgTime   ; }
    Q_INVOKABLE void setInferAvgTime(int value)
    {
        m_inferAvgTime = value;
        emit inferAvgTimeChanged(m_inferAvgTime);
    }
    Q_INVOKABLE int getInferHeight() const { return m_inferHeight  ; }
    Q_INVOKABLE void setInferHeight(int value)
    {
        m_inferHeight = value;
        emit inferHeightChanged(m_inferHeight);
    }
    Q_INVOKABLE int getInferCurrentHeight() const { return m_inferCurrentHeight  ; }
    Q_INVOKABLE void setInferCurrentHeight(int value)
    {
        m_inferCurrentHeight = value;
        emit inferCurrentHeightChanged(m_inferCurrentHeight);
    }

    Q_INVOKABLE int getInferMeasurePeriod () const { return m_inferMeasurePeriod   ; }
    Q_INVOKABLE void setInferMeasurePeriod (int value)
    {
        m_inferMeasurePeriod  = value;
        emit inferMeasurePeriodChanged(m_inferMeasurePeriod );
    }

    Q_INVOKABLE bool getInferScanBtn() const { return m_inferScanBtn   ; }
    Q_INVOKABLE void setInferScanBtn(bool value)
    {
        m_inferScanBtn = value;
        qDebug() << value << "scanBtnChanged" << m_inferScanBtn;
        emit inferScanBtnChanged(m_inferScanBtn);
    }
    Q_INVOKABLE bool getInferAutoScanBtn() const { return m_inferAutoScanBtn   ; }
    Q_INVOKABLE void setInferAutoScanBtn(bool value)
    {
        m_inferAutoScanBtn = value;
        qDebug() << value << "AutoscanBtnChanged" << m_inferAutoScanBtn;
        emit inferAutoScanBtnChanged(m_inferAutoScanBtn);
    }
    Q_INVOKABLE bool getInferSaveBtn() const { return m_inferSaveBtn   ; }
    Q_INVOKABLE void setInferSaveBtn(bool value)
    {
        m_inferSaveBtn = value;
        qDebug() << value << "SaveBtnChanged" << m_inferSaveBtn;
        emit inferSaveBtnChanged(m_inferSaveBtn);
    }
    Q_INVOKABLE bool getInferHeightSetBtn() const { return m_inferHeightSetBtn   ; }
    Q_INVOKABLE void setInferHeightSetBtn(bool value)
    {
        m_inferHeightSetBtn = value;
        qDebug() << value << "HeightSetBtnChanged" << m_inferHeightSetBtn;
        emit inferHeightSetBtnChanged(m_inferHeightSetBtn);
    }
    Q_INVOKABLE bool getInferPeriodBtn() const { return m_inferPeriodBtn   ; }
    Q_INVOKABLE void setInferPeriodBtn(bool value)
    {
        m_inferPeriodBtn = value;
        qDebug() << value << "PeriodBtnChanged" << m_inferPeriodBtn;
        emit inferPeriodBtnChanged(m_inferPeriodBtn);
    }
    Q_INVOKABLE bool getInferWhiteBtn() const { return m_inferWhiteBtn ; }
    Q_INVOKABLE void setInferWhiteBtn(bool value)
    {
        m_inferWhiteBtn = value;
        emit inferWhiteBtnChanged(m_inferWhiteBtn);
    }

    Q_INVOKABLE QString getInferLabel() const { return m_inferLabel ; }
    Q_INVOKABLE void setInferLabel(const QString &value)
    {
        m_inferLabel = value;
        emit inferLabelChanged(m_inferLabel);
    }

    Q_INVOKABLE QVariantList getInferChartData() const { return m_inferChartData ; }

    Q_INVOKABLE void setInferChartData(const std::vector<float>& x,
                                    const std::vector<float>& y)
    {
        m_inferChartData.clear();
        m_inferChartData.reserve(static_cast<int>(x.size()));
        for (size_t i = 0; i < x.size(); ++i) {
            QVariantMap item;
            item.insert("x", x[i]);
            item.insert("y", y[i]);
            m_inferChartData.append(item);
        }
        //m_chartData = value;
        emit inferChartDataChanged();
    }
    Q_INVOKABLE QVariantList getInferNirList() const { return m_inferNirList   ; }

    Q_INVOKABLE void setInferNirList(const std::vector<QString>& datalist)
    {
        // 1. 清理並預留空間，減少記憶體重分配
        m_inferNirList.clear();
        m_inferNirList.reserve(static_cast<int>(datalist.size()));
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        QString label = getLabel();

        // 3. 填充數據
        for (const QString& data : datalist) {
            QVariantMap item;
            item.insert("time", currentTime);
            item.insert("label", label);
            item.insert("listData", data);

            m_inferNirList.append(item);
        }
        // 4. 通知界面更新
        emit inferNirListChanged();
    }

    Q_INVOKABLE QVariantList getInferWhiteScanList() const { return m_inferWhiteScanList   ; }

    Q_INVOKABLE void setInferWhiteScanList(const std::vector<QString>& datalist)
    {
        // 1. 清理並預留空間，減少記憶體重分配
        m_inferWhiteScanList.clear();
        m_inferWhiteScanList.reserve(static_cast<int>(datalist.size()));
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        QString label = "white";

        // 3. 填充數據
        for (const QString& data : datalist) {
            QVariantMap item;
            item.insert("time", currentTime);
            item.insert("label", label);
            item.insert("listData", data);

            m_inferWhiteScanList.append(item);
        }
        // 4. 通知界面更新
        emit inferWhiteScanListChanged();
    }

    Q_INVOKABLE QVariantList getInferPredictList() const
    {
        qDebug() << u8"m_inferPredictList" << m_inferPredictList;
        return m_inferPredictList   ;
    }
    Q_INVOKABLE void setInferPredictList(const std::vector<QString>& dataList)
    {
        for (size_t i = 0; i < dataList.size(); ++i) {
            QVariantMap item;
            // item.insert("name", dataList[i]);
            m_inferPredictList.append(item);
        }
        qDebug() << u8"m_inferPredictList" << m_inferPredictList;
        emit inferPredictListChanged();
    }

    Q_INVOKABLE QVariantList getInferModelSetList() const { return m_inferModelSetList   ; }
    Q_INVOKABLE void setInferModelSetList(const std::vector<QString>& dataList)
    {
        for (size_t i = 0; i < dataList.size(); ++i) {
            QVariantMap item;
            // item.insert("name", dataList[i]);
            m_inferModelSetList.append(item);
        }
        emit inferModelSetListChanged();
    }
    //異常提醒
    Q_INVOKABLE void raiseAbnormal() {
        abnormalRaised();
    }
    signals:

    void abnormalRaised();
    void chartDataChanged();
    void nirListChanged();
    void whiteScanListChanged();

    void clearBtnChanged(bool);
    void updateBtnChanged(bool);
    void connectBtnChanged(bool);
    void connectedLightChanged(bool);
    void savedLightChanged(bool);
    void confirmBtnChanged(bool);
    void autoBtnChanged(bool);
    void scanBtnChanged(bool);
    void whiteBtnChanged(bool);
    void saveLabelBtnChanged(bool);
    void saveFileNameBtnChanged(bool);
    void clearNirListBtnChanged(bool);
    void powerOnResetBtnChanged(bool);

    void integrationTimeChanged(int);
    void avgTimeChanged(int);
    void heightChanged(int);
    void heightSetChanged(int);
    void resetBtnChanged(bool);
    void autoSetHeightBtnChanged(bool);

    void labelChanged( QString text);
    void whiteLabelChanged( QString text);
    void fileNameChanged( QString text);

    //====inference======
    void inferConnectBtnChanged(bool);
    void inferConnectedLightChanged(bool);
    void inferIntegrationTimeChanged(int);
    void inferAvgTimeChanged(int);
    void inferHeightChanged(int);
    void inferCurrentHeightChanged(int);
    void inferMeasurePeriodChanged(int);
    void inferScanBtnChanged(bool);
    void inferAutoScanBtnChanged(bool);
    void inferWhiteBtnChanged(bool);
    void inferSaveBtnChanged(bool);
    void inferHeightSetBtnChanged(bool);
    void inferPeriodBtnChanged(bool);
    void inferLabelChanged(QString text);
    void inferChartDataChanged();
    void inferNirListChanged();
    void inferWhiteScanListChanged();
    void inferPredictListChanged();
    void inferModelSetListChanged();

    // void inferLabelChanged();


private:

    void saveCsvFileInternal(const QString &folderPath,
                             const QString &csvContent,
                             bool useInferConfig)
    {
        QString filePath = folderPath;
        if (filePath.startsWith("file:/"))
            filePath = QUrl(filePath).toLocalFile();

        qWarning() << "save file path:" << filePath;

        if (filePath.isEmpty()) {
            qWarning() << "Invalid file path";
            return;
        }

        if (!filePath.endsWith(".csv", Qt::CaseInsensitive))
            filePath += ".csv";

        QFileInfo fi(filePath);
        QDir dir = fi.dir();
        if (!dir.exists() && !dir.mkpath(".")) {
            qWarning() << "Failed to create dir:" << dir.absolutePath();
            return;
        }

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << u8"⚠ Failed to write CSV:" << filePath;
            return;
        }

        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << csvContent;
        file.close();

        qDebug() << u8"✔ CSV saved:" << filePath;

        const QString jsonPath = dir.absoluteFilePath("instrument_config.json");
        saveInstrumentConfigFile(jsonPath, useInferConfig);
    }

    void saveInstrumentConfigFile(const QString &filePath, bool useInferConfig)
    {
        QFile jsonFile(filePath);
        if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << u8"⚠ Failed to write JSON:" << filePath;
            return;
        }

        QJsonObject jsonObject;
        if (useInferConfig) {
            jsonObject.insert("height_cm", static_cast<double>(getInferHeight()));
            jsonObject.insert("integration_time_us", static_cast<double>(getInferIntegrationTime()));
            jsonObject.insert("average_count", getInferAvgTime());
        } else {
            jsonObject.insert("height_cm", static_cast<double>(getHeight()));
            jsonObject.insert("integration_time_us", static_cast<double>(getIntegrationTime()));
            jsonObject.insert("average_count", getAvgTime());
        }
        jsonObject.insert("white_ref", buildWhiteRefObject(useInferConfig));

        const QJsonDocument jsonDocument(jsonObject);
        jsonFile.write(jsonDocument.toJson(QJsonDocument::Indented));
        jsonFile.close();

        qDebug() << u8"✔ JSON saved:" << filePath;
    }

    QJsonObject buildWhiteRefObject(bool useInferConfig) const
    {
        QJsonArray rawSamples;
        rawSamples.append(QJsonObject{{"height_cm", 110.0}, {"intensity", 1000.0}});
        rawSamples.append(QJsonObject{{"height_cm", 111.0}, {"intensity", 1500.0}});
        rawSamples.append(QJsonObject{{"height_cm", 112.0}, {"intensity", 2000.0}});
        rawSamples.append(QJsonObject{{"height_cm", 113.0}, {"intensity", 2500.0}});
        rawSamples.append(QJsonObject{{"height_cm", 114.0}, {"intensity", 3000.0}});

        QJsonObject whiteRefObject;
        if (useInferConfig) {
            whiteRefObject.insert("height_cm", static_cast<double>(getInferHeight()));
            whiteRefObject.insert("integration_time_us", static_cast<double>(getInferIntegrationTime()));
            whiteRefObject.insert("average_count", getInferAvgTime());
        } else {
            whiteRefObject.insert("height_cm", static_cast<double>(getHeight()));
            whiteRefObject.insert("integration_time_us", static_cast<double>(getIntegrationTime()));
            whiteRefObject.insert("average_count", getAvgTime());
        }
        whiteRefObject.insert("raw_samples", rawSamples);
        return whiteRefObject;
    }

    QVariantList m_chartData={};
    QVariantList m_nirList;
    QVariantList m_whiteScanList;
	QString m_lastFolderPath;
	QString m_lastFolderName;
    bool m_clearBtn = false;
    bool m_updateBtn = false;
    bool m_connectBtn = false;
    bool m_connectedLight = false;
    bool m_savedLight = false;
    bool m_confirmBtn = false;
    bool m_autoBtn = false;
    bool m_scanBtn = false;
    bool m_whiteBtn = false;
    bool m_saveLabelBtn = false;
    bool m_saveFileNameBtn = false;
    bool m_clearNirListBtn = false;
    bool m_powerOnResetBtn = false;

    int m_integrationTime = 10;
    int m_avgTime   = 1;
    int m_height   = 0;
    int m_heightSet   = 120;
    bool m_resetBtn = false;
    bool m_autoSetHeightBtn = true;

    QString m_label ="";
    QString m_whiteLabel ="White";
    QString m_fileName="";

    bool m_inferConnectBtn = false;
    bool m_inferConnectedLight = false;
    int m_inferIntegrationTime = 10;
    int m_inferAvgTime = 1;
    int m_inferHeight = 0;
    int m_inferCurrentHeight = 0;
    int m_inferMeasurePeriod = 1;
    bool m_inferScanBtn = false;
    bool m_inferAutoScanBtn = false;
    bool m_inferWhiteBtn = false;
    bool m_inferSaveBtn = false;
    bool m_inferHeightSetBtn = false;
    bool m_inferPeriodBtn = false;
    QString m_inferLabel = "";
    QVariantList m_inferChartData={};
    QVariantList m_inferNirList;
    QVariantList m_inferWhiteScanList;
    QVariantList m_inferPredictList;
    QVariantList m_inferModelSetList;
};


