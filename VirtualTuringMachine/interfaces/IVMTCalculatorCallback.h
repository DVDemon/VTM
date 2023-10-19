#if !defined(EA_6B80DBEC_F1C7_4381_B9D1_3CCD36BEDBEF__INCLUDED_)
#define EA_6B80DBEC_F1C7_4381_B9D1_3CCD36BEDBEF__INCLUDED_

class IVMTCalculatorCallback
{

public:
	IVMTCalculatorCallback() {

	}

	virtual ~IVMTCalculatorCallback() {

	}

	virtual long GetCalculatorValue() =0;
	virtual void SetCalculatorValue(long value) =0;

};
#endif // !defined(EA_6B80DBEC_F1C7_4381_B9D1_3CCD36BEDBEF__INCLUDED_)
