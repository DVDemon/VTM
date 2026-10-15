#include <QByteArray>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QTextStream>

#include "VMTJsonSerializer.h"
#include "vmtdebuger.h"
#include "vmtproject.h"

namespace {
class HeadlessEnvironment final : public IVMTEnvironment {
   public:
    void EnableActionHint(const QString&, QPixmap&) override {}
    void DisableActionHint() override {}
    void DisableAlphabit() override {}
    void DisableCalculator() override {}
    void EnableAlphabit(IVMTAlphabitSource*, const QString&,
                        QPixmap&) override {}
    void EnableCalculator(std::shared_ptr<IVMTMachine>) override {}
    UICanvas& GetGraphics() override { return _canvas; }
    std::weak_ptr<VMTComplexMachine> GetMachine() override { return _machine; }
    void SetMachine(std::shared_ptr<VMTComplexMachine> machine) override {
        _machine = machine;
    }
    void Repaint(const QRect&) override {}
    void MoveInScreen(QPoint&&) override {}
    void Move(QPoint&&) override {}
    void EnableAnimation(bool) override {}

   private:
    UICanvas _canvas{QBrush(), QPen(), QPen(), QSize(1, 1), 1};
    std::weak_ptr<VMTComplexMachine> _machine;
};

QString outputWord(const std::shared_ptr<VMTLine>& line) {
    QChar labmda = line->GetAlphabit()->GetLambda();
    QString line_raw = line->ToString();
    if (line_raw.size() < 1) {
        return "";
    }
    const long left = 0;
    long right = line_raw.size() - 1;
    while (right > left && line_raw[right] == labmda) {
        --right;
    }

    QString result;
    result.reserve(static_cast<int>(right - left + 1));
    for (long position = left; position <= right; ++position) {
        result.append(line->GetValueAt(position));
    }
    return result;
}

bool run(const QString& path, const QString& machineName, const QString& input,
         int maxSteps, QString& output, QString& error) {
    VMTProject& project = VMTProject::GetInstance();
    bool loaded = false;
    if (VMTJsonSerializer::isJsonPath(path)) {
        loaded = VMTJsonSerializer(path).deserialize(&project);
    } else {
        throw std::logic_error("Legacy .jdtp format support was dropped");
    }
    if (!loaded) {
        error = QStringLiteral("cannot load diagram: ") + path;
        return false;
    }

    QString selected = machineName;
    std::shared_ptr<VMTComplexMachine> machine;
    if (selected.isEmpty()) {
        machine = project.GetCurrentMachine();
    } else {
        machine = project.GetMachine(selected);
    }
    if (!machine) {
        error = QStringLiteral("machine not found: ") +
                (selected.isEmpty() ? QStringLiteral("<default>") : selected);
        return false;
    }
    bool hasStart = false;
    for (const auto& component : machine->GetMachineCollection()) {
        if (component->GetID() == IVMTMachine::MT_START) {
            hasStart = true;
            break;
        }
    }
    if (!hasStart) {
        error =
            QStringLiteral("machine has no start state: ") + machine->GetName();
        return false;
    }

    VMTDebuger debugger(project.GetAlphabit(), machine);
    for (long position = 0; position < input.size(); ++position) {
        debugger.GetLine()->SetValueAt(position + 1,
                                       input[position].toLatin1());
    }
    debugger.GetLine()->SetActivePosition(input.size() + 1);

    HeadlessEnvironment environment;
    debugger.ToStart(&environment);
    int steps = 0;
    while (!debugger.IsFinish() && steps < maxSteps) {
        debugger.Step(&environment);

        QTextStream(stderr) << debugger.GetLine()->GetActivePosition() << " | "
                            << debugger.GetLine()->ToString() << Qt::endl;
        if (debugger.GetLine()->GetActivePosition() < 0) {
            error = QStringLiteral(
                "Machine working cell out of left line border (< 0). Which is "
                "forbidden");
            return false;
        }
        ++steps;
    }
    if (!debugger.IsFinish()) {
        error = QStringLiteral("maximum step count exceeded: ") +
                QString::number(maxSteps);
        return false;
    }
    output = outputWord(debugger.GetLine());
    return true;
}
}  // namespace

int main(int argc, char* argv[]) {
    if (!qEnvironmentVariableIsSet("QT_QPA_PLATFORM")) {
        qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("offscreen"));
    }
    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("VTM-headless"));
    app.setApplicationVersion(QStringLiteral("1.0.2"));

    QCommandLineParser parser;
    parser.setApplicationDescription(
        QStringLiteral("Execute a Virtual Turing Machine diagram."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption diagramOption(
        {QStringLiteral("d"), QStringLiteral("diagram")},
        QStringLiteral("Diagram file (.vmt or .vmt.json)."),
        QStringLiteral("file"));
    QCommandLineOption machineOption(
        {QStringLiteral("m"), QStringLiteral("machine")},
        QStringLiteral("Machine name (defaults to the first one)."),
        QStringLiteral("name"));
    QCommandLineOption inputOption(
        {QStringLiteral("i"), QStringLiteral("input")},
        QStringLiteral("Input tape word (may be repeated)."),
        QStringLiteral("word"));
    QCommandLineOption maxStepsOption(
        QStringLiteral("max-steps"), QStringLiteral("Maximum execution steps."),
        QStringLiteral("count"), QStringLiteral("1000000"));
    parser.addOption(diagramOption);
    parser.addOption(machineOption);
    parser.addOption(inputOption);
    parser.addOption(maxStepsOption);
    parser.addPositionalArgument(QStringLiteral("word"),
                                 QStringLiteral("Input tape word."));
    parser.process(app);
    const QString path = parser.value(diagramOption);
    if (path.isEmpty()) {
        QTextStream(stderr)
            << "A diagram file is required (--diagram)." << Qt::endl;
        return 2;
    }
    bool ok = false;
    const int maxSteps = parser.value(maxStepsOption).toInt(&ok);
    if (!ok || maxSteps <= 0) {
        QTextStream(stderr)
            << "--max-steps must be a positive integer." << Qt::endl;
        return 2;
    }

    QStringList inputs = parser.values(inputOption);
    inputs.append(parser.positionalArguments());
    if (inputs.isEmpty()) inputs.append(QString());

    QTextStream out(stdout);
    QTextStream err(stderr);
    for (const QString& input : inputs) {
        QString result;
        QString error;
        if (!run(path, parser.value(machineOption), input, maxSteps, result,
                 error)) {
            err << error << Qt::endl;
            return 1;
        }
        out << result << Qt::endl;
    }
    return 0;
}
