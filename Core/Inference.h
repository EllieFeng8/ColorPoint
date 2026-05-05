#pragma once

#include <QObject>
#include <vector>
#include "specpredict.h"
class Inference : public QObject
{
    Q_OBJECT
public:
    struct ModelInfo {
        QString trainedModelId;
        QString modelName;
        QString itemName;
        QString component;
    };
public slots:

    void init() {
    SpecPredict::instance().setBaseUrl(QUrl("http://127.0.0.1"));
}
    void load()
    {

        auto mocels = m_Predict.parsePredictionEntry(json, models);
    }
    void Infer(QVector<double> white)
    {
        m_Predict.predict(white, spectra);
    }
private:
    SpecPredict& m_Predict = SpecPredict::instance();
    QList<SpecPredict::ModelInfo> models;
    QJsonObject json;
    QVector<double>spectra;
};