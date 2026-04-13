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

    // ===== Settings =====
    Q_PROPERTY(int  integrationTime READ getIntegrationTime WRITE setIntegrationTime NOTIFY integrationTimeChanged)
    Q_PROPERTY(int  avgTime   READ getAvgTime   WRITE setAvgTime   NOTIFY avgTimeChanged)

    // ===== Text =====
    Q_PROPERTY(QString label    READ getLabel    WRITE setLabel    NOTIFY labelChanged)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)

    //======inference======
    Q_PROPERTY(bool inferConnectBtn READ getInferConnectBtn WRITE setInferConnectBtn NOTIFY inferConnectBtnChanged)
    Q_PROPERTY(bool inferConnectedLight  READ getInferConnectedLight  WRITE setInferConnectedLight NOTIFY inferConnectedLightChanged)

    Q_PROPERTY(int  inferIntegrationTime READ getInferIntegrationTime WRITE setInferIntegrationTime NOTIFY inferIntegrationTimeChanged)
    Q_PROPERTY(int  inferAvgTime   READ getInferAvgTime   WRITE setInferAvgTime   NOTIFY inferAvgTimeChanged)

    Q_PROPERTY(bool inferScanBtn    READ getInferScanBtn    WRITE setInferScanBtn    NOTIFY inferScanBtnChanged)
    Q_PROPERTY(bool inferAutoScanBtn    READ getInferAutoScanBtn    WRITE setInferAutoScanBtn    NOTIFY inferAutoScanBtnChanged)
    Q_PROPERTY(bool inferWhiteBtn   READ getInferWhiteBtn   WRITE setInferWhiteBtn   NOTIFY inferWhiteBtnChanged)
    Q_PROPERTY(QString inferLabel    READ getInferLabel    WRITE setInferLabel    NOTIFY inferLabelChanged)

    // =====inference Array =====
    Q_PROPERTY(QVariantList inferChartData READ getInferChartData  NOTIFY inferChartDataChanged)
    Q_PROPERTY(QVariantList inferNirList   READ getInferNirList  NOTIFY inferNirListChanged)
    Q_PROPERTY(QVariantList inferWhiteScanList   READ getInferWhiteScanList  NOTIFY inferWhiteScanListChanged)

public:

    explicit ColorPointProxy(QObject *parent = nullptr) : QObject(parent) {}
    //保存路徑
    Q_INVOKABLE QUrl  lastFolderPath() const { return QUrl::fromLocalFile(m_lastFolderPath); }
    //保存文字名字
    Q_INVOKABLE QUrl  lastFolderName() const { return m_lastFolderName; }

    Q_INVOKABLE void saveCsvFile(const QString &folderPath,
                            const QString &csvContent)
    {
        // ================================
        // 1. 基底路徑：執行檔所在路徑 + 相對資料夾
        // ================================
        QString filePath = folderPath;
        // QML 可能傳 file:///... 或 file:/...
        if (filePath.startsWith("file:/"))
            filePath = QUrl(filePath).toLocalFile();

        qWarning() << "save file path:" << filePath;

        if (filePath.isEmpty()) {
            qWarning() << "Invalid file path";
            return;
        }
        // 使用者若沒打 .csv，這裡補上
        if (!filePath.endsWith(".csv", Qt::CaseInsensitive))
            filePath += ".csv";

        // ✅ 只建立「父資料夾」，不要對 filePath mkpath
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
            item.insert("time", currentTime);
            item.insert("label", label);
            item.insert("listData", data);

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
            item.insert("listData", data);

            m_whiteScanList.append(item);
        }
        // 4. 通知界面更新
        emit whiteScanListChanged();
    }
    Q_INVOKABLE bool getClearBtn() const { return m_clearBtn; }
    Q_INVOKABLE void setClearBtn(bool value)
    {
        if (m_clearBtn != value)
            m_clearBtn = value;
        emit clearBtnChanged(m_clearBtn);
    }
    Q_INVOKABLE bool getUpdateBtn() const { return m_updateBtn; }
    Q_INVOKABLE void setUpdateBtn(bool value)
    {
        if (m_updateBtn != value)
            m_updateBtn = value;
        emit updateBtnChanged(m_updateBtn);
    }
    Q_INVOKABLE bool getConnectBtn() const { return m_connectBtn ; }
    Q_INVOKABLE void setConnectBtn(bool value)
    {
        if (m_connectBtn != value)
            m_connectBtn = value;
        qDebug() << value << "connectBtnChanged" << m_connectBtn;
        emit connectBtnChanged(m_connectBtn);
    }
    Q_INVOKABLE bool getConnectedLight() const { return m_connectedLight   ; }
    Q_INVOKABLE void setConnectedLight(bool value)
    {
        if (m_connectedLight != value)
            m_connectedLight = value;
        emit connectedLightChanged(m_connectedLight);
    }
    Q_INVOKABLE bool getSavedLight() const { return m_savedLight ; }
    Q_INVOKABLE void setSavedLight(bool value)
    {
        if (m_savedLight != value)
            m_savedLight = value;
        emit savedLightChanged(m_savedLight);
    }
    Q_INVOKABLE bool getConfirmBtn() const { return m_confirmBtn; }
    Q_INVOKABLE void setConfirmBtn(bool value)
    {
        if (m_confirmBtn != value)
            m_confirmBtn = value;
        emit confirmBtnChanged(m_confirmBtn);
    }
    Q_INVOKABLE bool getAutoBtn() const { return m_autoBtn ; }
    Q_INVOKABLE void setAutoBtn(bool value)
    {
        if (m_autoBtn != value)
            m_autoBtn = value;
        emit autoBtnChanged(m_autoBtn);
    }

    Q_INVOKABLE bool getScanBtn() const { return m_scanBtn   ; }
    Q_INVOKABLE void setScanBtn(bool value)
    {
        if (m_scanBtn != value)
            m_scanBtn = value;
        qDebug() << value << "scanBtnChanged" << m_scanBtn;
        emit scanBtnChanged(m_scanBtn);
    }
    Q_INVOKABLE bool getWhiteBtn() const { return m_whiteBtn ; }
    Q_INVOKABLE void setWhiteBtn(bool value)
    {
        if (m_whiteBtn != value)
            m_whiteBtn = value;
        emit whiteBtnChanged(m_whiteBtn);
    }

    Q_INVOKABLE bool getSaveLabelBtn() const { return m_saveLabelBtn   ; }
    Q_INVOKABLE void setSaveLabelBtn(bool value)
    {
        if (m_saveLabelBtn != value)
            m_saveLabelBtn = value;
        emit saveLabelBtnChanged(m_saveLabelBtn);
    }
    Q_INVOKABLE bool getSaveFileNameBtn() const { return m_saveFileNameBtn   ; }
    Q_INVOKABLE void setSaveFileNameBtn(bool value)
    {
        if (m_saveFileNameBtn != value)
            m_saveFileNameBtn = value;
        emit saveFileNameBtnChanged(m_saveFileNameBtn);
    }
    Q_INVOKABLE int getIntegrationTime() const { return m_integrationTime ; }
    Q_INVOKABLE void setIntegrationTime(int value)
    {
        if (m_integrationTime != value)
            m_integrationTime = value;
        emit integrationTimeChanged(m_integrationTime);
    }
    Q_INVOKABLE int getAvgTime() const { return m_avgTime   ; }
    Q_INVOKABLE void setAvgTime(int value)
    {
        if (m_avgTime != value)
            m_avgTime = value;
        emit avgTimeChanged(m_avgTime);
    }
    Q_INVOKABLE QString getLabel() const { return m_label ; }
    Q_INVOKABLE void setLabel(const QString &value)
    {
        if (m_label != value)
            m_label = value;
        emit labelChanged(m_label);
    }
    Q_INVOKABLE QString getFileName() const { return m_fileName   ; }
    Q_INVOKABLE void setFileName(const QString &value)
    {
        if (m_fileName != value)
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
        if (m_inferAvgTime != value)
            m_inferAvgTime = value;
        emit inferAvgTimeChanged(m_inferAvgTime);
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
    signals:
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

    void integrationTimeChanged(int);
    void avgTimeChanged(int);

    void labelChanged( QString text);
    void fileNameChanged( QString text);

    //====inference======
    void inferConnectBtnChanged(bool);
    void inferConnectedLightChanged(bool);
    void inferIntegrationTimeChanged(int);
    void inferAvgTimeChanged(int);
    void inferScanBtnChanged(bool);
    void inferAutoScanBtnChanged(bool);
    void inferWhiteBtnChanged(bool);
    void inferLabelChanged(QString text);
    void inferChartDataChanged();
    void inferNirListChanged();
    void inferWhiteScanListChanged();
    // void inferLabelChanged();


private:

    QVariantList m_chartData;
    QVariantList m_nirList;
    QVariantList m_whiteScanList;
	QString m_lastFolderPath;
	QString m_lastFolderName;
    bool m_clearBtn = false;
    bool m_updateBtn = false;
    bool m_connectBtn = false;
    bool m_connectedLight = true;
    bool m_savedLight = false;
    bool m_confirmBtn = false;
    bool m_autoBtn = false;
    bool m_scanBtn = false;
    bool m_whiteBtn = false;
    bool m_saveLabelBtn = false;
    bool m_saveFileNameBtn = false;

    int m_integrationTime = 100;
    int m_avgTime   = 101;

    QString m_label ="123ab";
    QString m_fileName="aa123";

    bool m_inferConnectBtn = false;
    bool m_inferConnectedLight = false;
    int m_inferIntegrationTime = 0;
    int m_inferAvgTime = 0;
    bool m_inferScanBtn = false;
    bool m_inferAutoScanBtn = false;
    bool m_inferWhiteBtn = false;
    QString m_inferLabel = "123";
    QVariantList m_inferChartData;
    QVariantList m_inferNirList;
    QVariantList m_inferWhiteScanList;
};


