#pragma once

#include <QJsonArray>
#include <QList>
#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QVector>

class SpecPredict : public QObject
{
    Q_OBJECT

public:
    struct InstrumentCondition {
        double heightCm = 0.0;
        double integrationTimeUs = 0.0;
        int averageCount = 0;
    };

    struct ModelInfo {
        QString trainedModelId;
        QString modelName;
        QString itemName;
        QString component;
    };

    struct PredictionParameters {
        QString name;
        QString groupId;
        InstrumentCondition sampleCondition;
        InstrumentCondition whiteRefCondition;
        QList<ModelInfo> models;

        bool isValid() const;
    };

    struct PredictionField {
        QString key;
        QString value;
    };

    struct PredictionResultItem {
        QString trainedModelId;
        QString modelName;
        QString itemName;
        QString component;
        bool hasPredictedValue = false;
        double predictedValue = 0.0;
        QString predictedClass;

        QString displayValue() const;
        QString resultType() const;
    };

    struct PredictionEntry {
        QString label;
        QList<PredictionResultItem> results;
    };

    struct PredictionResultData {
        QString groupId;
        QString groupName;
        QList<PredictionEntry> predictions;
        QJsonObject rawObject;

        bool isValid() const;
        int totalResultCount() const;
    };

    static SpecPredict &instance();

    void setBaseUrl(const QUrl &baseUrl);
    QUrl baseUrl() const;

    void fetchCurrentParameters();
    void predict(const QVector<double> &white, const QVector<double> &spectra);

    PredictionParameters currentParameters() const;
    QJsonObject lastPredictionResult() const;
    PredictionResultData lastParsedPredictionResult() const;

signals:
    void currentParametersUpdated();
    void predictionFinished();
    void requestFailed(const QString &operation, const QString &errorMessage);

public:
    explicit SpecPredict(QObject *parent = nullptr);

    Q_DISABLE_COPY_MOVE(SpecPredict)

    static InstrumentCondition parseInstrumentCondition(const QJsonObject &json);
    static ModelInfo parseModelInfo(const QJsonObject &json);
    static PredictionResultItem parsePredictionResultItem(const QJsonObject &json,
                                                          const QList<ModelInfo> &models);
    static PredictionEntry parsePredictionEntry(const QJsonObject &json,
                                                const QList<ModelInfo> &models);
    PredictionResultData parsePredictionResult(const QJsonObject &json) const;
    QUrl buildApiUrl(const QString &path) const;
    void failRequest(const QString &operation, const QString &message);
    static QJsonArray vectorToJsonArray(const QVector<double> &values);

    QNetworkAccessManager networkManager_;
    QUrl baseUrl_;
    PredictionParameters currentParameters_;
    QJsonObject lastPredictionResult_;
    PredictionResultData lastParsedPredictionResult_;
};
