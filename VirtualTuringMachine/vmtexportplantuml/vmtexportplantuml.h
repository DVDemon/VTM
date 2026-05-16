#ifndef VMTEXPORTPLANTUML_H
#define VMTEXPORTPLANTUML_H

#include <QString>

class VMTComplexMachine;

class VmtExportPlantUml
{
public:
    static QString exportStateMachine(VMTComplexMachine& machine);
};

#endif // VMTEXPORTPLANTUML_H
