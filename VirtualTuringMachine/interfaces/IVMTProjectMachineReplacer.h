#if !defined(EA_2DE1D8E7_CC65_4196_B305_92B0066B2169__INCLUDED_)
#define EA_2DE1D8E7_CC65_4196_B305_92B0066B2169__INCLUDED_

class VMTComplexMachine;
class IVMTProjectMachineReplacer
{

public:
    virtual void ReplaceInnerMachine(VMTComplexMachine* machine) =0;

};
#endif // !defined(EA_2DE1D8E7_CC65_4196_B305_92B0066B2169__INCLUDED_)
