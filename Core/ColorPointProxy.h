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
    Q_PROPERTY(bool saveBtn    READ getSaveBtn    WRITE setSaveBtn    NOTIFY saveBtnChanged)

    // ===== Settings =====
    Q_PROPERTY(int  integTime READ getIntegTime WRITE setIntegTime NOTIFY integTimeChanged)
    Q_PROPERTY(int  avgTime   READ getAvgTime   WRITE setAvgTime   NOTIFY avgTimeChanged)

    // ===== Text =====
    Q_PROPERTY(QString label    READ getLabel    WRITE setLabel    NOTIFY labelChanged)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)



public:

    explicit ColorPointProxy(QObject *parent = nullptr) : QObject(parent) {}
    //保存路徑
    Q_INVOKABLE QUrl  lastFolderPath() const { return QUrl::fromLocalFile(m_lastFolderPath); }
    //保存文字名字
    Q_INVOKABLE QUrl  lastFolderName() const { return m_lastFolderName; }

    Q_INVOKABLE void saveCsvFile(const QString &relativePath,
                            const QString &fileName,
                            const QString &csvContent)
    {
        // ================================
        // 1. 基底路徑：執行檔所在路徑 + 相對資料夾
        // ================================
        QDir appDir(QCoreApplication::applicationDirPath());
        QString folderPath = appDir.filePath(relativePath);

        // 確保資料夾存在
        QDir().mkpath(folderPath);

        // ================================
        // 2. 組合完整檔案路徑
        // ================================
        QString finalFileName = fileName;
        if (!finalFileName.endsWith(".csv", Qt::CaseInsensitive)) {
            finalFileName += ".csv";
        }
        QString fullFilePath = QDir(folderPath).filePath(finalFileName);

        // ================================
        // 3. 寫入 CSV（每次都新建 / 覆寫）
        // ================================
        QFile file(fullFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << u8"⚠ Failed to write CSV:" << fullFilePath;
            return;
        }

        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << csvContent;
        file.close();

        qDebug() << u8"✔ CSV saved:" << fullFilePath;
    }

    Q_INVOKABLE QVariantList getChartData() const { return m_chartData ; }
    // Q_INVOKABLE void setChartData(const QVariantList &value)
    // {
    //     if (m_chartData != value)
    //     {
    //         for(int i = 0; i < value.size(); ++i) {
    //             QStringList item;
    //             //item["name"] = listLeft[i];;
    //             m_chartData.append(item);
    //         }
    //     }
    //         //m_chartData = value;
    //     emit chartDataChanged();
    // }
    Q_INVOKABLE void setChartData(const std::vector<int>& x,
                                    const std::vector<int>& y)
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
    // Q_INVOKABLE void setNirList(const std::vector<QString>& label,
    //                                 const std::vector<QDateTime>& Time)
    // {
    //     m_nirList.clear();
    //     m_nirList.reserve(static_cast<int>(label.size()));
    //     for (size_t i = 0; i < label.size(); ++i) {
    //         QVariantMap item;
    //         item.insert("Time", Time[i]);
    //         item.insert("label", label[i]);
    //         m_nirList.append(item);
    //     }
    //     emit nirListChanged();
    // }
    Q_INVOKABLE void setNirList(const std::vector<QString>& labels,
                                const std::vector<QDateTime>& times)
    {
        // 1. 清理並預留空間，減少記憶體重分配
        m_nirList.clear();
        m_nirList.reserve(static_cast<int>(labels.size()));

        // 2. 填充數據
        for (size_t i = 0; i < labels.size(); ++i) {
            // 使用初始化列表創建 QVariantMap，代碼更簡潔高效
            m_nirList.append(QVariantMap{
                {"Time", times[i]},
                {"Label", labels[i]} // 修正了拼寫錯誤 Lable -> Label
            });
        }
        // 4. 通知界面更新
        emit nirListChanged();
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
    Q_INVOKABLE bool getSaveBtn() const { return m_saveBtn   ; }
    Q_INVOKABLE void setSaveBtn(bool value)
    {
        if (m_saveBtn != value)
            m_saveBtn = value;
        emit saveBtnChanged(m_saveBtn);
    }
    Q_INVOKABLE int getIntegTime() const { return m_integTime ; }
    Q_INVOKABLE void setIntegTime(int value)
    {
        if (m_integTime != value)
            m_integTime = value;
        emit integTimeChanged(m_integTime);
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

    signals:
    void chartDataChanged();
    void nirListChanged();

    void clearBtnChanged(bool);
    void updateBtnChanged(bool);
    void connectBtnChanged(bool);
    void connectedLightChanged(bool);
    void savedLightChanged(bool);
    void confirmBtnChanged(bool);
    void autoBtnChanged(bool);
    void scanBtnChanged(bool);
    void whiteBtnChanged(bool);
    void saveBtnChanged(bool);

    void integTimeChanged(int);
    void avgTimeChanged(int);

    void labelChanged( QString text);
    void fileNameChanged( QString text);


private:

    QVariantList m_chartData;
    QVariantList m_nirList;
	QString m_lastFolderPath;
	QString m_lastFolderName;
    bool m_clearBtn   = false;
    bool m_updateBtn  = false;
    bool m_connectBtn = false;
    bool m_connectedLight = true;
    bool m_savedLight    = false;
    bool m_confirmBtn = false;
    bool m_autoBtn    = false;
    bool m_scanBtn    = false;
    bool m_whiteBtn   = false;
    bool m_saveBtn    = false;

    int m_integTime = 100;
    int m_avgTime   = 101;

    QString m_label ="123ab";
    QString m_fileName="aa123";
};


