#if !defined(EA_F87A7617_E180_442b_A3D0_541F8C64FC9B__INCLUDED_)
#define EA_F87A7617_E180_442b_A3D0_541F8C64FC9B__INCLUDED_

#include <QString>

class VMTAlphabit
{
private:
    QString _signs;
public:
	VMTAlphabit();
    VMTAlphabit(const VMTAlphabit& other);
    void AddSign(char sign);


    char GetLambda();
    char GetSignAt(int index);
    int GetSignsCount();
    bool IsEqual(const VMTAlphabit &other);
	bool IsLambda(char sign);
    bool IsSign(char sign);

    void ReplaceString(const QString& other);
	const QString& ToString();

    bool operator !=(const VMTAlphabit &other);
    virtual ~VMTAlphabit();

};
#endif // !defined(EA_F87A7617_E180_442b_A3D0_541F8C64FC9B__INCLUDED_)
