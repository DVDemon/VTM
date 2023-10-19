#if !defined(EA_7CBC34C2_863E_487a_972A_9FFC94D25030__INCLUDED_)
#define EA_7CBC34C2_863E_487a_972A_9FFC94D25030__INCLUDED_

class IVMTLineObserver
{

public:
	virtual void OnPositionChanged(long position) =0;
	virtual void OnValueChanged(char sign, long position) =0;
	virtual void StartObserve() =0;
	virtual void StopObserve() =0;

};
#endif // !defined(EA_7CBC34C2_863E_487a_972A_9FFC94D25030__INCLUDED_)
