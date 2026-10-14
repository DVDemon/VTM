#if !defined(EA_F87A7617_E180_442b_A3D0_541F8C64FC9B__INCLUDED_)
#define EA_F87A7617_E180_442b_A3D0_541F8C64FC9B__INCLUDED_

#include <QString>

class VMTAlphabit {
   private:
    QString _signs;

   public:
    VMTAlphabit();
    VMTAlphabit(const VMTAlphabit& other);
    void AddSign(char sign);

    QChar GetLambda() const;
    char GetSignAt(int index) const;
    int GetSignsCount() const;
    bool IsEqual(const VMTAlphabit& other) const;
    bool IsLambda(QChar sign) const;
    bool Contains(QChar sign) const;

    void ReplaceString(const QString& other);
    const QString& ToString();

    bool operator!=(const VMTAlphabit& other) const;
    virtual ~VMTAlphabit();
};
#endif  // !defined(EA_F87A7617_E180_442b_A3D0_541F8C64FC9B__INCLUDED_)
