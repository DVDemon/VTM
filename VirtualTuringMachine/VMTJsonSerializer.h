#ifndef VMTJSONSERIALIZER_H
#define VMTJSONSERIALIZER_H

#include <QString>

class VMTProject;

/** JSON interchange format compatible with vmt-web (.vmt.json). */
class VMTJsonSerializer
{
    QString _file_name;

public:
    explicit VMTJsonSerializer(const QString& fileName);

    bool serialize(VMTProject* project) const;
    bool deserialize(VMTProject* project) const;

    static bool isJsonPath(const QString& path);
};

#endif // VMTJSONSERIALIZER_H
