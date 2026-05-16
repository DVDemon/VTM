#ifndef IVMTUNDOOBSERVER_H
#define IVMTUNDOOBSERVER_H

class IVMTUndoObserver{
public:
    virtual void EnableUndoButton(bool enable) = 0;
    virtual ~IVMTUndoObserver() = default;
};

#endif // IVMTUNDOOBSERVER_H
