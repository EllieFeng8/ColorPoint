#include "specpredict.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>

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

QVector<double> parseDoubleList(const char *values)
{
    QVector<double> result;
    const QStringList tokens = QString::fromLatin1(values).split(QLatin1Char(','), Qt::SkipEmptyParts);
    result.reserve(tokens.size());

    for (const QString &token : tokens) {
        result.append(token.trimmed().toDouble());
    }

    return result;
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

void SpecPredict::testPredict()
{
    static const char WhiteInput[] = R"(1.373571062, 1.45100402, 1.528436977, 1.605869935, 1.683302893,
1.76073585, 1.838168808, 1.915601766, 1.993034724, 2.070467681,
2.147900639, 2.225333597, 2.302766554, 2.354725766, 2.461016706,
2.54793395, 2.637007149, 2.696460804, 2.765226282, 2.825118036,
2.873475062, 2.899475722, 2.96855431, 2.990887551, 2.981138586,
2.995593693, 2.985753808, 2.969323339, 2.928186132, 2.906832757,
2.873341122, 2.838352981, 2.753809059, 2.680553131, 2.596715444,
2.51227044, 2.43886709, 2.339045357, 2.233095061, 2.130109729,
2.038151763, 1.933471471, 1.838415138, 1.737485061, 1.647634986,
1.538684069, 1.474422798, 1.408922871, 1.317021893, 1.238301847,
1.183750364, 1.112070229, 1.062505482, 1.051648528, 1.013849924,
0.991082694, 0.991209686, 0.994370409, 1.022817059, 1.053066304,
1.091607601, 1.136509466, 1.18884606, 1.241300532, 1.306926868,
1.374871443, 1.483959824, 1.559569217, 1.650097684, 1.762378856,
1.840522452, 1.958309529, 2.053894343, 2.140998399, 2.249523439,
2.356364428, 2.447410342, 2.542748364, 2.614134941, 2.721208956,
2.748006466, 2.815229088, 2.872260545, 2.903437046, 2.963450228,
2.987745908, 2.99460852, 2.994644818, 2.980414654, 2.9727017,
2.963263948, 2.919193721, 2.87314292, 2.829577707, 2.774890804,
2.696844369, 2.621304914, 2.549463243, 2.449623414, 2.352332524,
2.266090856, 2.158083483, 2.056623988, 1.972077299, 1.855872657,
1.733132169, 1.666480724, 1.565847238, 1.478073484, 1.401947902,
1.318354553, 1.251693189, 1.166499718, 1.033709314, 0.940413535,
0.847117757, 0.753821979, 0.660526201, 0.567230422, 0.473934644,
0.380638866, 0.287343088, 0.194047309, 0.100751531, 0.007455753,
-0.085840025, -0.179135804, -0.272431582, -0.36572736, -0.459023138,
-0.552318917, -0.645614695, -0.738910473, -0.832206251, -0.925502029,
-1.018797808, -1.112093586, -1.205389364, -1.298685142, -1.391980921,
-1.485276699)";

    static const char SpectraInput[] = R"(2.081282041, 2.161013121, 2.249580745, 2.33392075, 2.425482103,
2.488611108, 2.589284951, 2.649868674, 2.751714156, 2.828571141,
2.893230663, 2.98740442, 3.065790447, 3.127561499, 3.231590124,
3.322759007, 3.43381329, 3.482071431, 3.564116444, 3.616998763,
3.687108565, 3.691182297, 3.764949239, 3.809977311, 3.779962641,
3.815724697, 3.79821608, 3.770358254, 3.710224899, 3.705560451,
3.671316447, 3.644934668, 3.559650678, 3.459369265, 3.385482274,
3.295732905, 3.221109884, 3.109693626, 2.973125161, 2.881977969,
2.774630494, 2.673349321, 2.584550935, 2.470960103, 2.384276245,
2.260879898, 2.17645357, 2.094891617, 2.024303877, 1.937345264,
1.871968104, 1.788852318, 1.746043038, 1.739952629, 1.708543574,
1.679670232, 1.693999024, 1.670908181, 1.720187791, 1.743871357,
1.791144007, 1.827364266, 1.897848554, 1.945611116, 2.005334974,
2.083933775, 2.197136951, 2.284462688, 2.363604525, 2.493967087,
2.580934721, 2.701998618, 2.816321721, 2.902798921, 3.00986604,
3.125207195, 3.222376106, 3.324375121, 3.377653762, 3.506241211,
3.522779322, 3.62156687, 3.668547777, 3.700225602, 3.774655582,
3.795629715, 3.787384727, 3.798000443, 3.780294097, 3.758035679,
3.758587655, 3.717003268, 3.668940825, 3.60915087, 3.577599521,
3.482443642, 3.403875844, 3.331456761, 3.213881396, 3.123335781,
3.023807994, 2.904178722, 2.811476324, 2.731159374, 2.600479322,
2.472530498, 2.387538703, 2.270240478, 2.208202739, 2.133261692,
2.039654581, 1.950302441, 1.851930074, 1.71225067, 1.607309369,
1.524312107, 1.442681871, 1.326447663, 1.232126716, 1.144098655,
1.02754093, 0.953176396, 0.853981223, 0.733988364, 0.630763333,
0.544750254, 0.435541821, 0.328690717, 0.254307854, 0.154899139,
0.045528979, -0.067415586, -0.165843003, -0.25123625, -0.356068054,
-0.453241076, -0.538371317, -0.640019957, -0.732331125, -0.854779587,
-0.947066072)";

    predict(parseDoubleList(WhiteInput), parseDoubleList(SpectraInput));
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
