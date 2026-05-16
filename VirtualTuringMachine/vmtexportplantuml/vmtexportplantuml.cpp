#include "vmtexportplantuml.h"

#include "VMTTransitionConditions.h"
#include "interfaces/IVMTTransition.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "vmtmachines/VMTComplexMachineInner.h"
#include "vmtmachines/VMTMachineAlpha.h"

#include <QHash>
#include <QTextStream>

namespace {

QString escapePlantumlLabel(const QString& label)
{
    QString escaped = label;
    escaped.replace(QLatin1String("\\"), QLatin1String("\\\\"));
    escaped.replace(QLatin1String("\""), QLatin1String("\\\""));
    escaped.replace(QLatin1String("\n"), QLatin1String(" "));
    escaped.replace(QLatin1String("\r"), QLatin1String(" "));
    return escaped;
}

QString makeUniqueAlias(const QString& base, QHash<QString, int>& usage)
{
    QString alias = base;
    if (alias.isEmpty()) {
        alias = QStringLiteral("state");
    }
    if (alias.at(0).isDigit()) {
        alias.prepend(QStringLiteral("s_"));
    }

    const int count = ++usage[alias];
    if (count > 1) {
        alias += QStringLiteral("_%1").arg(count);
    }
    return alias;
}

QString sanitizeAliasBase(const QString& raw)
{
    QString alias;
    alias.reserve(raw.size());
    for (const QChar ch : raw) {
        if (ch.isLetterOrNumber()) {
            alias.append(ch);
        } else if (ch == QLatin1Char('_')) {
            if (!alias.isEmpty() && alias.back() != QLatin1Char('_')) {
                alias.append(ch);
            }
        } else if (!alias.isEmpty() && alias.back() != QLatin1Char('_')) {
            alias.append(QLatin1Char('_'));
        }
    }
    while (alias.endsWith(QLatin1Char('_'))) {
        alias.chop(1);
    }
    return alias;
}

QString machineDisplayName(const std::shared_ptr<IVMTMachine>& machine)
{
    if (!machine) {
        return QStringLiteral("unknown");
    }

    switch (machine->GetID()) {
    case IVMTMachine::MachineType::MT_START:
        return QStringLiteral("start");
    case IVMTMachine::MachineType::MT_FINISH:
        return QStringLiteral("finish");
    case IVMTMachine::MachineType::MT_WRITE: {
        if (auto alpha = std::dynamic_pointer_cast<VMTMachineAlpha>(machine)) {
            return QStringLiteral("λ(%1)").arg(alpha->GetSign());
        }
        return QStringLiteral("write");
    }
    default:
        break;
    }

    const QString name = machine->GetName();
    if (!name.isEmpty()) {
        return name;
    }
    return QStringLiteral("machine");
}

class PlantUmlBuilder
{
public:
    explicit PlantUmlBuilder(QTextStream& out)
        : _out(out)
    {
    }

    void exportComplexMachine(VMTComplexMachine& machine)
    {
        _out << "@startuml\n";
        _out << "!theme plain\n";
        _out << "hide empty description\n";
        _out << "skinparam shadowing false\n";
        _out << "skinparam state {\n";
        _out << "  BackgroundColor White\n";
        _out << "  BorderColor #212121\n";
        _out << "}\n\n";
        _out << "title " << escapePlantumlLabel(machine.GetName()) << "\n\n";

        if (machine.GetInnerObject()) {
            exportScope(*machine.GetInnerObject(), QString());
        }

        _out << "@enduml\n";
    }

private:
    void exportScope(VMTComplexMachineInner& scope, const QString& indent)
    {
        QHash<QString, int> aliasUsage;
        QHash<const IVMTMachine*, QString> machineAliases;

        for (const std::shared_ptr<IVMTMachine>& machine : scope.GetMachineCollection()) {
            if (!machine) {
                continue;
            }
            if (machine->GetID() == IVMTMachine::MachineType::MT_START
                || machine->GetID() == IVMTMachine::MachineType::MT_FINISH) {
                continue;
            }

            if (machine->GetID() == IVMTMachine::MachineType::MT_COMPLEX) {
                auto complex = std::dynamic_pointer_cast<VMTComplexMachine>(machine);
                if (!complex) {
                    continue;
                }

                const QString display = machineDisplayName(machine);
                const QString alias = makeUniqueAlias(sanitizeAliasBase(display), aliasUsage);
                machineAliases.insert(machine.get(), alias);

                _out << indent << "state \"" << escapePlantumlLabel(display) << "\" as " << alias << " {\n";
                if (complex->GetInnerObject()) {
                    exportScope(*complex->GetInnerObject(), indent + QStringLiteral("  "));
                }
                _out << indent << "}\n";
                continue;
            }

            const QString display = machineDisplayName(machine);
            const QString alias = makeUniqueAlias(sanitizeAliasBase(display), aliasUsage);
            machineAliases.insert(machine.get(), alias);
            _out << indent << "state \"" << escapePlantumlLabel(display) << "\" as " << alias << "\n";
        }

        if (!scope.GetTransitionCollection().empty()) {
            _out << "\n";
        }

        for (const std::shared_ptr<IVMTTransition>& transition : scope.GetTransitionCollection()) {
            if (!transition) {
                continue;
            }

            auto startMachine = transition->GetStartMachine().lock();
            auto finishMachine = transition->GetFinishMachine().lock();
            if (!startMachine || !finishMachine) {
                continue;
            }

            const QString fromRef = stateRef(startMachine, machineAliases);
            const QString toRef = stateRef(finishMachine, machineAliases);
            if (fromRef.isEmpty() || toRef.isEmpty()) {
                continue;
            }

            _out << indent << fromRef << " --> " << toRef;

            VMTTransitionConditions& conditions = transition->GetConditions();
            QString label = conditions.GetAsString();
            if (conditions.IsAllEnabled()) {
                label = QStringLiteral("*");
            } else if (label.isEmpty()) {
                label = QStringLiteral("ε");
            }

            if (!label.isEmpty()) {
                _out << " : " << escapePlantumlLabel(label);
            }
            _out << "\n";
        }
    }

    QString stateRef(const std::shared_ptr<IVMTMachine>& machine,
                     const QHash<const IVMTMachine*, QString>& aliases) const
    {
        if (!machine) {
            return QString();
        }

        if (machine->GetID() == IVMTMachine::MachineType::MT_START
            || machine->GetID() == IVMTMachine::MachineType::MT_FINISH) {
            return QStringLiteral("[*]");
        }

        return aliases.value(machine.get());
    }

    QTextStream& _out;
};

} // namespace

QString VmtExportPlantUml::exportStateMachine(VMTComplexMachine& machine)
{
    QString result;
    QTextStream stream(&result);
    PlantUmlBuilder builder(stream);
    builder.exportComplexMachine(machine);
    return result;
}
