#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QApplication>
#include <vector>

class Configuration
{
private:
    static Configuration instance;
    QApplication *_app;
    std::vector<QString> _recent_projects;
    QString _default_location;
public:
    Configuration();
    void Init(QApplication *app);
    void Save();
    const QString& GetDefaultLocation();
    const std::vector<QString>& GetRecentProjects();
    void  AddRecentProject(QString & name);
    static Configuration& GetInstance();
};

#endif // CONFIGURATION_H
