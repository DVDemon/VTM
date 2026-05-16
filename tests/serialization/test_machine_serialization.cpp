#include <QtTest>
#include <QApplication>
#include <QTemporaryDir>
#include <QFile>

#include <algorithm>
#include <memory>
#include <vector>

#include "vmtproject.h"
#include "VMTSerializer.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "vmtmachines/VMTMachineStub.h"
#include "interfaces/IVMTMachine.h"
#include "interfaces/IVMTTransition.h"
#include "interfaces/IVMTEnvironment.h"
#include "uicanvas.h"

namespace {

class TestEnvironment : public IVMTEnvironment
{
public:
    explicit TestEnvironment(const std::shared_ptr<VMTComplexMachine>& machine)
        : _machine(machine)
        , _canvas(QBrush(Qt::white),
                  QPen(Qt::gray),
                  QPen(Qt::black),
                  QSize(20, 20),
                  12)
    {}

    void EnableActionHint(const QString&, QPixmap&) override {}
    void DisableActionHint() override {}
    void DisableAlphabit() override {}
    void DisableCalculator() override {}
    void EnableAlphabit(IVMTAlphabitSource*, const QString&, QPixmap&) override {}
    void EnableCalculator(std::shared_ptr<IVMTMachine>) override {}
    UICanvas& GetGraphics() override { return _canvas; }
    std::weak_ptr<VMTComplexMachine> GetMachine() override { return _machine; }
    void SetMachine(std::shared_ptr<VMTComplexMachine> machine) override { _machine = machine; }
    void Repaint(const QRect&) override {}
    void MoveInScreen(QPoint&&) override {}
    void Move(QPoint&&) override {}
    void EnableAnimation(bool) override {}

private:
    std::shared_ptr<VMTComplexMachine> _machine;
    UICanvas _canvas;
};


struct NodeSnapshot {
    int type = 0;
    QRect bounds;
    long power = 1;

    bool operator<(const NodeSnapshot& other) const
    {
        if (type != other.type)
            return type < other.type;
        if (bounds.x() != other.bounds.x())
            return bounds.x() < other.bounds.x();
        if (bounds.y() != other.bounds.y())
            return bounds.y() < other.bounds.y();
        if (bounds.width() != other.bounds.width())
            return bounds.width() < other.bounds.width();
        if (bounds.height() != other.bounds.height())
            return bounds.height() < other.bounds.height();
        return power < other.power;
    }
};

struct TransitionSnapshot {
    int startType = 0;
    int finishType = 0;

    bool operator<(const TransitionSnapshot& other) const
    {
        if (startType != other.startType)
            return startType < other.startType;
        return finishType < other.finishType;
    }
};

struct DiagramSnapshot {
    QString name;
    std::vector<NodeSnapshot> nodes;
    std::vector<TransitionSnapshot> transitions;
};

void resetProject()
{
    VMTProject& project = VMTProject::GetInstance();
    project.GetMachines().clear();
    project.GetCurrentMachineName().clear();
    project.GetName() = QStringLiteral("serialization_test");
    project.GetLocation() = QDir::tempPath();
    project.GetAlphabit()->ReplaceString(QStringLiteral("01"));
    project.GetUndoManager()->Clear();
}

void placeNode(const std::shared_ptr<IVMTMachine>& node, int x, int y)
{
    node->Move(QPoint(x, y), nullptr);
    node->Update(nullptr);
}

DiagramSnapshot captureDiagram(const std::shared_ptr<VMTComplexMachine>& machine)
{
    DiagramSnapshot snapshot;
    snapshot.name = machine->GetName();

    for (const std::shared_ptr<IVMTMachine>& node : machine->GetMachineCollection()) {
        NodeSnapshot nodeSnapshot;
        nodeSnapshot.type = static_cast<int>(node->GetID());
        nodeSnapshot.bounds = node->GetBounds();
        nodeSnapshot.power = node->GetPower();
        snapshot.nodes.push_back(nodeSnapshot);
    }
    std::sort(snapshot.nodes.begin(), snapshot.nodes.end());

    for (const std::shared_ptr<IVMTTransition>& transition :
         machine->GetTransitionCollection()) {
        TransitionSnapshot transitionSnapshot;
        if (auto start = transition->GetStartMachine().lock())
            transitionSnapshot.startType = static_cast<int>(start->GetID());
        if (auto finish = transition->GetFinishMachine().lock())
            transitionSnapshot.finishType = static_cast<int>(finish->GetID());
        snapshot.transitions.push_back(transitionSnapshot);
    }
    std::sort(snapshot.transitions.begin(), snapshot.transitions.end());

    return snapshot;
}

std::shared_ptr<VMTComplexMachine> buildSampleDiagram()
{
    auto machine = std::make_shared<VMTComplexMachine>(
        QStringLiteral("Main machine"),
        VMTProject::GetInstance().GetAlphabit());

    auto start = VMTMachineStub::CreateMachineByID(IVMTMachine::MT_START, machine);
    auto left = VMTMachineStub::CreateMachineByID(IVMTMachine::MT_LEFT, machine);
    auto right = VMTMachineStub::CreateMachineByID(IVMTMachine::MT_RIGHT, machine);
    auto finish = VMTMachineStub::CreateMachineByID(IVMTMachine::MT_FINISH, machine);

    if (!start || !left || !right || !finish)
        return {};

    placeNode(start, 100, 100);
    placeNode(left, 220, 100);
    placeNode(right, 340, 100);
    placeNode(finish, 460, 100);

    machine->AddMachine(start);
    machine->AddMachine(left);
    machine->AddMachine(right);
    machine->AddMachine(finish);

    TestEnvironment environment(machine);
    VMTComplexMachine::ConnectMachines(start, left, &environment, machine);
    VMTComplexMachine::ConnectMachines(left, right, &environment, machine);
    VMTComplexMachine::ConnectMachines(right, finish, &environment, machine);
    machine->Update(&environment);

    return machine;
}

QString saveProjectToTempFile(QTemporaryDir& tempDir)
{
    QString path = tempDir.filePath(QStringLiteral("diagram.jdtp"));
    VMTSerializer serializer(path);
    serializer.Serialize(&VMTProject::GetInstance());
    return path;
}

void loadProjectFromFile(QString path)
{
    VMTProject& project = VMTProject::GetInstance();
    project.GetMachines().clear();
    VMTSerializer serializer(path);
    serializer.Deserialize(&project);
}

} // namespace

class MachineSerializationTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        resetProject();
    }

    void cleanup()
    {
        resetProject();
    }

    void roundTrip_preservesDiagramNodesAndTransitions()
    {
        resetProject();

        const std::shared_ptr<VMTComplexMachine> machine = buildSampleDiagram();
        QVERIFY(machine);

        const DiagramSnapshot original = captureDiagram(machine);
        VMTProject::GetInstance().GetMachines().push_back(machine);

        QTemporaryDir tempDir;
        QVERIFY2(tempDir.isValid(), qPrintable(tempDir.errorString()));

        const QString path = saveProjectToTempFile(tempDir);
        QVERIFY(QFile::exists(path));
        loadProjectFromFile(QString(path));

        QCOMPARE(VMTProject::GetInstance().GetMachines().size(), 1);

        const DiagramSnapshot loaded =
            captureDiagram(VMTProject::GetInstance().GetMachines().front());

        QCOMPARE(loaded.name, original.name);
        QCOMPARE(loaded.nodes.size(), original.nodes.size());
        QCOMPARE(loaded.transitions.size(), original.transitions.size());

        for (size_t i = 0; i < original.nodes.size(); ++i) {
            QCOMPARE(loaded.nodes[i].type, original.nodes[i].type);
            QCOMPARE(loaded.nodes[i].bounds, original.nodes[i].bounds);
            QCOMPARE(loaded.nodes[i].power, original.nodes[i].power);
        }

        for (size_t i = 0; i < original.transitions.size(); ++i) {
            QCOMPARE(loaded.transitions[i].startType, original.transitions[i].startType);
            QCOMPARE(loaded.transitions[i].finishType, original.transitions[i].finishType);
        }
    }
};

QTEST_MAIN(MachineSerializationTest)
#include "test_machine_serialization.moc"
