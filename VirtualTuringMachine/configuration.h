#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QApplication>
#include <vector>
#include "exercise.h"
#include <memory>

class Configuration
{
private:
    static Configuration instance;
    QApplication *_app;
    std::vector<QString> _recent_projects;
    std::vector<std::shared_ptr<Exercise>> _exercises;

    QString _default_location;
    void LoadExercises();
public:
    Configuration();
    void Init(QApplication *app);
    void Save();
    const QString& GetDefaultLocation();
    const std::vector<QString>& GetRecentProjects();
    const std::vector<std::shared_ptr<Exercise>>& GetExercises();

    void  AddRecentProject(QString & name);
    static Configuration& GetInstance();
};

#endif // CONFIGURATION_H
