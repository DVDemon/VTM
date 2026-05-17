#include "VMTJsonSerializer.h"

#include "VMTAlphabit.h"
#include "VMTTransitionImpl.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "vmtmachines/VMTComplexMachineInner.h"
#include "vmtmachines/VMTMachineAlpha.h"
#include "vmtmachines/VMTMachineStub.h"
#include "vmtproject.h"

#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QSet>
#include <QUuid>
#include <QDebug>
#include <functional>

namespace {

const int SCHEMA_VERSION = 1;

QString newId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString machineTypeToString(IVMTMachine::MachineType type)
{
    switch (type) {
    case IVMTMachine::MT_START: return QStringLiteral("start");
    case IVMTMachine::MT_FINISH: return QStringLiteral("finish");
    case IVMTMachine::MT_LEFT: return QStringLiteral("left");
    case IVMTMachine::MT_RIGHT: return QStringLiteral("right");
    case IVMTMachine::MT_LEFT_WORD: return QStringLiteral("leftWord");
    case IVMTMachine::MT_RIGHT_WORD: return QStringLiteral("rightWord");
    case IVMTMachine::MT_WRITE: return QStringLiteral("write");
    case IVMTMachine::MT_COPY: return QStringLiteral("copy");
    case IVMTMachine::MT_COMPLEX: return QStringLiteral("complex");
    default: return QStringLiteral("start");
    }
}

IVMTMachine::MachineType machineTypeFromString(const QString& type)
{
    if (type == QLatin1String("finish")) return IVMTMachine::MT_FINISH;
    if (type == QLatin1String("left")) return IVMTMachine::MT_LEFT;
    if (type == QLatin1String("right")) return IVMTMachine::MT_RIGHT;
    if (type == QLatin1String("leftWord")) return IVMTMachine::MT_LEFT_WORD;
    if (type == QLatin1String("rightWord")) return IVMTMachine::MT_RIGHT_WORD;
    if (type == QLatin1String("write")) return IVMTMachine::MT_WRITE;
    if (type == QLatin1String("copy")) return IVMTMachine::MT_COPY;
    if (type == QLatin1String("complex")) return IVMTMachine::MT_COMPLEX;
    return IVMTMachine::MT_START;
}

QJsonObject pointToJson(const QPoint& p)
{
    QJsonObject o;
    o.insert(QStringLiteral("x"), p.x());
    o.insert(QStringLiteral("y"), p.y());
    return o;
}

QPoint pointFromJson(const QJsonObject& o)
{
    return QPoint(o.value(QStringLiteral("x")).toInt(), o.value(QStringLiteral("y")).toInt());
}

std::vector<QPoint> routingFromJson(const QJsonArray& arr)
{
    std::vector<QPoint> points;
    points.reserve(arr.size());
    for (const QJsonValue& pv : arr) {
        points.push_back(pointFromJson(pv.toObject()));
    }
    return points;
}

void linkTransition(const std::shared_ptr<VMTTransitionImpl>& transition,
                    const std::shared_ptr<IVMTMachine>& start,
                    const std::shared_ptr<IVMTMachine>& finish,
                    const QJsonObject& transitionJson)
{
    transition->AttachStartMachine(start);
    transition->AttachFinishMachine(finish);
    start->AddOutgoingTransition(transition);
    finish->AddIncomingTransition(transition);

    const std::vector<QPoint> route =
        routingFromJson(transitionJson.value(QStringLiteral("routingPoints")).toArray());
    if (route.size() >= 2) {
        transition->SetCommittedRoutingPolyline(route);
    } else {
        transition->RebuildRouteFromMachines();
    }
}

std::shared_ptr<VMTAlphabit> alphabitFromString(const QString& alphabet)
{
    auto a = std::make_shared<VMTAlphabit>();
    a->ReplaceString(alphabet);
    return a;
}

QJsonArray enabledSignsToJson(VMTTransitionConditions& conditions,
                              const std::shared_ptr<VMTAlphabit>& alphabit)
{
    QJsonArray arr;
    const QString all = alphabit->ToString();
    for (const QChar& ch : all) {
        if (conditions.IsEnabled(ch.toLatin1())) {
            arr.append(QString(ch));
        }
    }
    return arr;
}

void applyEnabledSigns(VMTTransitionConditions& conditions,
                       const QJsonArray& signs,
                       IVMTEnvironment* env)
{
    QString alphabit = conditions.GetAlphabit()->ToString();
    for (const QChar& ch : alphabit) {
        conditions.SetEnable(ch.toLatin1(), false, env);
    }
    for (const QJsonValue& v : signs) {
        const QString s = v.toString();
        if (!s.isEmpty()) {
            conditions.SetEnable(s[0].toLatin1(), true, env);
        }
    }
}

QJsonObject machineToJson(IVMTMachine* machine,
                          const QMap<IVMTMachine*, QString>& machineIds,
                          const QMap<std::shared_ptr<VMTComplexMachineInner>, QString>& innerIds)
{
    QJsonObject o;
    o.insert(QStringLiteral("id"), machineIds.value(machine));
    o.insert(QStringLiteral("type"), machineTypeToString(machine->GetID()));
    o.insert(QStringLiteral("center"), pointToJson(machine->GetCenter()));
    o.insert(QStringLiteral("size"), pointToJson(machine->GetSize()));
    o.insert(QStringLiteral("power"), static_cast<int>(machine->GetPower()));
    o.insert(QStringLiteral("parentBodyId"), QJsonValue::Null);

    if (machine->GetID() == IVMTMachine::MT_WRITE) {
        if (auto* alpha = dynamic_cast<VMTMachineAlpha*>(machine)) {
            o.insert(QStringLiteral("sign"), QString(alpha->GetSign()));
        }
    } else if (machine->GetID() == IVMTMachine::MT_COMPLEX) {
        if (auto* cm = dynamic_cast<VMTComplexMachine*>(machine)) {
            o.insert(QStringLiteral("name"), cm->GetName());
            const auto inner = cm->GetInnerObject();
            o.insert(QStringLiteral("innerId"), innerIds.value(inner));
        }
    }
    return o;
}

QJsonObject bodyToJson(const QString& bodyId,
                       const QString& bodyName,
                       VMTComplexMachineInner* inner,
                       const std::shared_ptr<VMTAlphabit>& projectAlphabit,
                       QMap<IVMTMachine*, QString>& machineIds,
                       const QMap<std::shared_ptr<VMTComplexMachineInner>, QString>& innerIds)
{
    QJsonObject body;
    body.insert(QStringLiteral("id"), bodyId);
    body.insert(QStringLiteral("name"), bodyName);
    body.insert(QStringLiteral("alphabet"),
                inner->GetAlphabit() ? inner->GetAlphabit()->ToString() : projectAlphabit->ToString());

    QJsonArray machines;
    for (const auto& m : inner->GetMachineCollection()) {
        if (!machineIds.contains(m.get())) {
            machineIds.insert(m.get(), newId());
        }
        machines.append(machineToJson(m.get(), machineIds, innerIds));
    }
    body.insert(QStringLiteral("machines"), machines);

    QJsonArray transitions;
    for (const auto& t : inner->GetTransitionCollection()) {
        if (!t) continue;
        auto start = t->GetStartMachine().lock();
        auto finish = t->GetFinishMachine().lock();
        if (!start || !finish) continue;

        QJsonObject tr;
        tr.insert(QStringLiteral("id"), newId());
        tr.insert(QStringLiteral("startMachineId"), machineIds.value(start.get()));
        tr.insert(QStringLiteral("finishMachineId"), machineIds.value(finish.get()));

        QJsonArray route;
        for (const QPoint& p : t->GetCommittedRoutingPolyline()) {
            route.append(pointToJson(p));
        }
        tr.insert(QStringLiteral("routingPoints"), route);
        QJsonObject cond;
        const std::shared_ptr<VMTAlphabit> bodyAlphabit =
            inner->GetAlphabit() ? inner->GetAlphabit() : projectAlphabit;
        cond.insert(QStringLiteral("enabledSigns"),
                    enabledSignsToJson(t->GetConditions(), bodyAlphabit));
        tr.insert(QStringLiteral("conditions"), cond);
        transitions.append(tr);
    }
    body.insert(QStringLiteral("transitions"), transitions);
    return body;
}

} // namespace

VMTJsonSerializer::VMTJsonSerializer(const QString& fileName)
    : _file_name(fileName)
{
}

bool VMTJsonSerializer::isJsonPath(const QString& path)
{
    return path.endsWith(QLatin1String(".json"), Qt::CaseInsensitive)
           || path.endsWith(QLatin1String(".vmt.json"), Qt::CaseInsensitive);
}

bool VMTJsonSerializer::serialize(VMTProject* project) const
{
    if (!project) return false;

    QMap<std::shared_ptr<VMTComplexMachineInner>, QString> innerIds;
    QMap<IVMTMachine*, QString> machineIds;
    QJsonArray bodiesArr;
    QJsonArray rootIds;

    QMap<std::shared_ptr<VMTComplexMachineInner>, QString> innerNames;

    const auto registerInner = [&](const std::shared_ptr<VMTComplexMachineInner>& inner,
                                   const QString& defaultName) -> QString {
        if (!inner) return QString();
        if (!innerIds.contains(inner)) {
            innerIds.insert(inner, newId());
            innerNames.insert(inner, defaultName);
        }
        return innerIds.value(inner);
    };

    std::function<void(const std::shared_ptr<VMTComplexMachineInner>&, const QString&)>
        scanInner;
    scanInner = [&](const std::shared_ptr<VMTComplexMachineInner>& inner,
                    const QString& defaultName) {
        if (!inner) return;
        registerInner(inner, defaultName);
        for (const auto& m : inner->GetMachineCollection()) {
            if (!m || m->GetID() != IVMTMachine::MT_COMPLEX) continue;
            const auto cm = std::dynamic_pointer_cast<VMTComplexMachine>(m);
            if (cm && cm->GetInnerObject()) {
                scanInner(cm->GetInnerObject(), cm->GetName());
            }
        }
    };

    QSet<VMTComplexMachineInner*> exported;
    std::function<void(const std::shared_ptr<VMTComplexMachineInner>&, const QString&)>
        emitBody;
    emitBody = [&](const std::shared_ptr<VMTComplexMachineInner>& inner,
                   const QString& defaultName) {
        if (!inner || exported.contains(inner.get())) return;
        exported.insert(inner.get());
        const QString bodyId = innerIds.value(inner);
        const QString bodyName = innerNames.value(inner, defaultName);
        bodiesArr.append(bodyToJson(bodyId, bodyName, inner.get(),
                                    project->GetAlphabit(), machineIds, innerIds));
        for (const auto& m : inner->GetMachineCollection()) {
            if (!m || m->GetID() != IVMTMachine::MT_COMPLEX) continue;
            const auto cm = std::dynamic_pointer_cast<VMTComplexMachine>(m);
            if (cm && cm->GetInnerObject()) {
                emitBody(cm->GetInnerObject(), cm->GetName());
            }
        }
    };

    for (const auto& top : project->GetMachines()) {
        if (!top) continue;
        const auto inner = top->GetInnerObject();
        scanInner(inner, top->GetName());
        rootIds.append(innerIds.value(inner));
    }
    for (const auto& top : project->GetMachines()) {
        if (top && top->GetInnerObject()) {
            emitBody(top->GetInnerObject(), top->GetName());
        }
    }

    QJsonObject root;
    root.insert(QStringLiteral("schemaVersion"), SCHEMA_VERSION);
    root.insert(QStringLiteral("id"), newId());
    root.insert(QStringLiteral("name"), project->GetName());
    root.insert(QStringLiteral("alphabet"), project->GetAlphabit()->ToString());
    root.insert(QStringLiteral("bodies"), bodiesArr);
    root.insert(QStringLiteral("rootBodyIds"), rootIds);
    const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    root.insert(QStringLiteral("createdAt"), now);
    root.insert(QStringLiteral("updatedAt"), now);

    QFile file(_file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "VMTJsonSerializer: cannot write" << _file_name;
        return false;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool VMTJsonSerializer::deserialize(VMTProject* project) const
{
    if (!project) return false;

    QFile file(_file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "VMTJsonSerializer: cannot read" << _file_name;
        return false;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return false;
    const QJsonObject root = doc.object();

    const QString alphabet = root.value(QStringLiteral("alphabet")).toString(QLatin1String(" 01"));
    project->SetAlphabit(alphabitFromString(alphabet));
    project->GetName() = root.value(QStringLiteral("name")).toString(project->GetName());

    QMap<QString, std::shared_ptr<VMTComplexMachineInner>> bodiesById;
    QMap<QString, QJsonObject> bodyJsonById;

    for (const QJsonValue& bv : root.value(QStringLiteral("bodies")).toArray()) {
        const QJsonObject b = bv.toObject();
        const QString id = b.value(QStringLiteral("id")).toString();
        const QString bodyName = b.value(QStringLiteral("name")).toString(QStringLiteral("Machine"));
        const QString bodyAlphabet = b.value(QStringLiteral("alphabet")).toString(alphabet);
        auto inner = std::make_shared<VMTComplexMachineInner>(
            bodyName, alphabitFromString(bodyAlphabet));
        bodiesById.insert(id, inner);
        bodyJsonById.insert(id, b);
    }

    for (auto it = bodyJsonById.begin(); it != bodyJsonById.end(); ++it) {
        const QString bodyId = it.key();
        const QJsonObject b = it.value();
        auto inner = bodiesById.value(bodyId);
        if (!inner) continue;

        const QString bodyName = b.value(QStringLiteral("name")).toString();
        const std::shared_ptr<VMTAlphabit> bodyAlphabit = inner->GetAlphabit();
        auto shell = std::make_shared<VMTComplexMachine>(bodyName, bodyAlphabit);
        shell->SetInnerObject(inner, shell);

        QMap<QString, std::shared_ptr<IVMTMachine>> idMap;
        for (const QJsonValue& mv : b.value(QStringLiteral("machines")).toArray()) {
            const QJsonObject mo = mv.toObject();
            const QString id = mo.value(QStringLiteral("id")).toString();
            const QString type = mo.value(QStringLiteral("type")).toString();
            const IVMTMachine::MachineType mt = machineTypeFromString(type);

            std::shared_ptr<IVMTMachine> machine;
            if (mt == IVMTMachine::MT_WRITE) {
                const QString sign = mo.value(QStringLiteral("sign")).toString();
                const char ch = sign.isEmpty()
                    ? bodyAlphabit->GetLambda()
                    : sign[0].toLatin1();
                machine = std::make_shared<VMTMachineAlpha>(ch, shell);
            } else if (mt == IVMTMachine::MT_COMPLEX) {
                const QString cmName = mo.value(QStringLiteral("name")).toString(bodyName);
                auto cm = std::make_shared<VMTComplexMachine>(cmName, bodyAlphabit);
                const QString innerId = mo.value(QStringLiteral("innerId")).toString();
                if (bodiesById.contains(innerId)) {
                    cm->SetInnerObject(bodiesById.value(innerId), shell);
                }
                machine = cm;
            } else {
                machine = VMTMachineStub::CreateMachineByID(mt, shell);
            }
            if (!machine) continue;

            machine->Move(pointFromJson(mo.value(QStringLiteral("center")).toObject()), nullptr);
            machine->GetSize() = pointFromJson(mo.value(QStringLiteral("size")).toObject());
            machine->SetPower(mo.value(QStringLiteral("power")).toInt(1));

            shell->AddMachine(machine);
            idMap.insert(id, machine);
        }

        for (const QJsonValue& tv : b.value(QStringLiteral("transitions")).toArray()) {
            const QJsonObject to = tv.toObject();
            const QString startId = to.value(QStringLiteral("startMachineId")).toString();
            const QString finishId = to.value(QStringLiteral("finishMachineId")).toString();
            if (!idMap.contains(startId) || !idMap.contains(finishId)) continue;

            const auto startMachine = idMap.value(startId);
            const auto finishMachine = idMap.value(finishId);
            auto transition = std::make_shared<VMTTransitionImpl>(shell);
            linkTransition(transition, startMachine, finishMachine, to);

            const QJsonObject cond = to.value(QStringLiteral("conditions")).toObject();
            applyEnabledSigns(transition->GetConditions(),
                              cond.value(QStringLiteral("enabledSigns")).toArray(),
                              nullptr);

            shell->AddTransition(transition);
        }
    }

    project->GetMachines().clear();
    const QJsonArray rootIds = root.value(QStringLiteral("rootBodyIds")).toArray();
    if (rootIds.isEmpty() && !bodiesById.isEmpty()) {
        for (auto it = bodiesById.begin(); it != bodiesById.end(); ++it) {
            auto cm = std::make_shared<VMTComplexMachine>(
                it.key(), bodiesById.value(it.key())->GetAlphabit());
            cm->SetInnerObject(it.value(), cm);
            project->GetMachines().push_back(cm);
        }
    } else {
        for (const QJsonValue& rv : rootIds) {
            const QString bodyId = rv.toString();
            const QJsonObject b = bodyJsonById.value(bodyId);
            const QString name = b.value(QStringLiteral("name")).toString(QStringLiteral("Machine"));
            auto cm = std::make_shared<VMTComplexMachine>(
                name,
                bodiesById.contains(bodyId) ? bodiesById.value(bodyId)->GetAlphabit()
                                           : project->GetAlphabit());
            if (bodiesById.contains(bodyId)) {
                cm->SetInnerObject(bodiesById.value(bodyId), cm);
            }
            project->GetMachines().push_back(cm);
        }
    }

    if (!project->GetMachines().empty()) {
        project->GetCurrentMachineName() = project->GetMachines().front()->GetName();
    }

    return !project->GetMachines().empty();
}
