#include "specpredict.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace {

const SpecPredict::ModelInfo *findModelInfoByItemName(const QList<SpecPredict::ModelInfo> &models,
                                                      const QString &itemName)
{
    for (const SpecPredict::ModelInfo &model : models) {
        if (model.itemName == itemName) {
            return &model;
        }
    }
    return nullptr;
}

} // namespace

bool SpecPredict::PredictionParameters::isValid() const
{
    return !name.isEmpty() && !groupId.isEmpty();
}

QString SpecPredict::PredictionResultItem::displayValue() const
{
    if (hasPredictedValue) {
        return QString::number(predictedValue, 'g', 15);
    }
    return predictedClass;
}

QString SpecPredict::PredictionResultItem::resultType() const
{
    return hasPredictedValue ? QStringLiteral("predicted_value")
                             : QStringLiteral("predicted_class");
}

bool SpecPredict::PredictionResultData::isValid() const
{
    return !groupId.isEmpty() || !groupName.isEmpty() || !predictions.isEmpty() || !rawObject.isEmpty();
}

int SpecPredict::PredictionResultData::totalResultCount() const
{
    int total = 0;
    for (const PredictionEntry &entry : predictions) {
        total += entry.results.size();
    }
    return total;
}

SpecPredict &SpecPredict::instance()
{
    static SpecPredict instance;
    return instance;
}

SpecPredict::SpecPredict(QObject *parent)
    : QObject(parent)
{
}

void SpecPredict::setBaseUrl(const QUrl &baseUrl)
{
    baseUrl_ = baseUrl;
}

QUrl SpecPredict::baseUrl() const
{
    return baseUrl_;
}

void SpecPredict::fetchCurrentParameters()
{
    if (!baseUrl_.isValid() || baseUrl_.scheme().isEmpty() || baseUrl_.host().isEmpty()) {
        failRequest(QStringLiteral("fetchCurrentParameters"),
                    QStringLiteral("Base URL is not set. Call setBaseUrl(...) first."));
        return;
    }

    QNetworkRequest request(buildApiUrl(QStringLiteral("api/default-models")));
    QNetworkReply *reply = networkManager_.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            failRequest(QStringLiteral("fetchCurrentParameters"), reply->errorString());
            return;
        }

        const QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        if (!document.isArray()) {
            failRequest(QStringLiteral("fetchCurrentParameters"),
                        QStringLiteral("Response is not a JSON array."));
            return;
        }

        const QJsonArray models = document.array();
        if (models.isEmpty()) {
            failRequest(QStringLiteral("fetchCurrentParameters"),
                        QStringLiteral("No default models were returned."));
            return;
        }

        QJsonObject selectedModel = models.first().toObject();
        for (const QJsonValue &value : models) {
            const QJsonObject model = value.toObject();
            if (model.value(QStringLiteral("is_active")).toBool()) {
                selectedModel = model;
                break;
            }
        }

        const QJsonObject instrumentConfig =
            selectedModel.value(QStringLiteral("instrument_config")).toObject();
        currentParameters_.name = selectedModel.value(QStringLiteral("name")).toString();
        currentParameters_.groupId = selectedModel.value(QStringLiteral("_id")).toString();
        currentParameters_.sampleCondition = parseInstrumentCondition(instrumentConfig);
        currentParameters_.whiteRefCondition =
            parseInstrumentCondition(instrumentConfig.value(QStringLiteral("white_ref")).toObject());
        currentParameters_.models.clear();

        const QJsonArray modelArray = selectedModel.value(QStringLiteral("models")).toArray();
        currentParameters_.models.reserve(modelArray.size());
        for (const QJsonValue &modelValue : modelArray) {
            currentParameters_.models.append(parseModelInfo(modelValue.toObject()));
        }

        emit currentParametersUpdated();
    });
}

void SpecPredict::predict(const QVector<double> &white, const QVector<double> &spectra)
{
    if (!currentParameters_.isValid()) {
        failRequest(QStringLiteral("predict"),
                    QStringLiteral("Prediction parameters are not loaded. Call fetchCurrentParameters() first."));
        return;
    }

    if (!baseUrl_.isValid() || baseUrl_.scheme().isEmpty() || baseUrl_.host().isEmpty()) {
        failRequest(QStringLiteral("predict"),
                    QStringLiteral("Base URL is not set. Call setBaseUrl(...) first."));
        return;
    }

    QJsonObject payload;
    payload.insert(QStringLiteral("white"), vectorToJsonArray(white));

    QJsonObject sampleObject;
    sampleObject.insert(QStringLiteral("spectra"), vectorToJsonArray(spectra));

    QJsonArray samples;
    samples.append(sampleObject);
    payload.insert(QStringLiteral("samples"), samples);

    QNetworkRequest request(
        buildApiUrl(QStringLiteral("api/default-models/%1/predict").arg(currentParameters_.groupId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QNetworkReply *reply =
        networkManager_.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            failRequest(QStringLiteral("predict"), reply->errorString());
            return;
        }

        const QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        if (!document.isObject()) {
            failRequest(QStringLiteral("predict"),
                        QStringLiteral("Prediction response is not a JSON object."));
            return;
        }

        lastPredictionResult_ = document.object();
        lastParsedPredictionResult_ = parsePredictionResult(lastPredictionResult_);
        emit predictionFinished();
    });
}

SpecPredict::PredictionParameters SpecPredict::currentParameters() const
{
    return currentParameters_;
}

QJsonObject SpecPredict::lastPredictionResult() const
{
    return lastPredictionResult_;
}

SpecPredict::PredictionResultData SpecPredict::lastParsedPredictionResult() const
{
    return lastParsedPredictionResult_;
}

SpecPredict::InstrumentCondition SpecPredict::parseInstrumentCondition(const QJsonObject &json)
{
    InstrumentCondition condition;
    condition.heightCm = json.value(QStringLiteral("height_cm")).toDouble();
    condition.integrationTimeUs = json.value(QStringLiteral("integration_time_us")).toDouble();
    condition.averageCount = json.value(QStringLiteral("average_count")).toInt();
    return condition;
}

SpecPredict::ModelInfo SpecPredict::parseModelInfo(const QJsonObject &json)
{
    ModelInfo model;
    model.trainedModelId = json.value(QStringLiteral("trained_model_id")).toString();
    model.modelName = json.value(QStringLiteral("model_name")).toString();
    model.itemName = json.value(QStringLiteral("item_name")).toString();
    model.component = json.value(QStringLiteral("component")).toString();
    return model;
}

SpecPredict::PredictionResultItem SpecPredict::parsePredictionResultItem(
    const QJsonObject &json, const QList<ModelInfo> &models)
{
    PredictionResultItem item;
    item.itemName = json.value(QStringLiteral("item_name")).toString();

    if (const ModelInfo *model = findModelInfoByItemName(models, item.itemName)) {
        item.trainedModelId = model->trainedModelId;
        item.modelName = model->modelName;
        item.component = model->component;
    }

    const QJsonValue predictedValueNode = json.value(QStringLiteral("predicted_value"));
    if (!predictedValueNode.isUndefined() && !predictedValueNode.isNull()) {
        item.hasPredictedValue = true;
        item.predictedValue = predictedValueNode.toDouble();
    }

    const QJsonValue predictedClassNode = json.value(QStringLiteral("predicted_class"));
    if (!predictedClassNode.isUndefined() && !predictedClassNode.isNull()) {
        item.predictedClass = predictedClassNode.toString();
    }

    return item;
}

SpecPredict::PredictionEntry SpecPredict::parsePredictionEntry(const QJsonObject &json,
                                                               const QList<ModelInfo> &models)
{
    PredictionEntry entry;

    const QJsonValue labelNode = json.value(QStringLiteral("label"));
    if (!labelNode.isNull() && !labelNode.isUndefined()) {
        entry.label = labelNode.toString();
    }

    const QJsonArray results = json.value(QStringLiteral("results")).toArray();
    entry.results.reserve(results.size());
    for (const QJsonValue &resultValue : results) {
        if (resultValue.isObject()) {
            entry.results.append(parsePredictionResultItem(resultValue.toObject(), models));
        }
    }

    return entry;
}

SpecPredict::PredictionResultData SpecPredict::parsePredictionResult(const QJsonObject &json) const
{
    PredictionResultData result;
    result.groupId = json.value(QStringLiteral("group_id")).toString();
    result.groupName = json.value(QStringLiteral("group_name")).toString();
    result.rawObject = json;

    const QJsonArray predictions = json.value(QStringLiteral("predictions")).toArray();
    result.predictions.reserve(predictions.size());
    for (const QJsonValue &predictionValue : predictions) {
        if (predictionValue.isObject()) {
            result.predictions.append(
                parsePredictionEntry(predictionValue.toObject(), currentParameters_.models));
        }
    }

    return result;
}

QUrl SpecPredict::buildApiUrl(const QString &path) const
{
    QUrl url(baseUrl_);

    QString basePath = url.path();
    if (!basePath.endsWith(QLatin1Char('/'))) {
        basePath += QLatin1Char('/');
    }

    QString normalizedPath = path;
    while (normalizedPath.startsWith(QLatin1Char('/'))) {
        normalizedPath.remove(0, 1);
    }

    url.setPath(basePath + normalizedPath);
    return url;
}

void SpecPredict::failRequest(const QString &operation, const QString &message)
{
    emit requestFailed(operation, message);
}

QJsonArray SpecPredict::vectorToJsonArray(const QVector<double> &values)
{
    QJsonArray array;

    for (double value : values) {
        array.append(value);
    }

    return array;
}
