#include "configuration.h"

#include <qdir.h>
#include <qmessagebox.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

Configuration Configuration::instance;

namespace {
QString configurationFilePath() {
    QString directory =
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if (directory.isEmpty()) {
        directory = QDir::homePath();
    }
    QDir().mkpath(directory);
    qDebug() << "Config path: "
             << QDir(directory).filePath(QStringLiteral("config.xml")) << '\n';
    return QDir(directory).filePath(QStringLiteral("config.xml"));
}
}  // namespace

Configuration::Configuration() : _dark_theme(false) {}

const std::vector<QString>& Configuration::GetRecentProjects() {
    return this->_recent_projects;
}

const std::vector<std::shared_ptr<Exercise>>& Configuration::GetExercises() {
    return _exercises;
}

void Configuration::AddRecentProject(QString& name) {
    bool found = false;

    for (auto& n : _recent_projects)
        if (n == name) found = true;

    if (!found) this->_recent_projects.insert(_recent_projects.begin(), name);

    if (_recent_projects.size() > 10) _recent_projects.pop_back();
}

const QString& Configuration::GetDefaultLocation() { return _default_location; }

void Configuration::LoadExercises() {
    QFile file("://config/exercises.xml");

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QXmlStreamReader reader(file.readAll());
        std::shared_ptr<Exercise> ex;
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == QString("exercise")) {
                    ex = std::make_shared<Exercise>();
                    foreach (const QXmlStreamAttribute& attr,
                             reader.attributes()) {
                        if (attr.name().toString() == QLatin1String("id")) {
                            ex->SetIndex(attr.value().toInt());
                        } else if (attr.name().toString() ==
                                   QLatin1String("text_en")) {
                            ex->SetTextEn(attr.value().toString());
                        } else if (attr.name().toString() ==
                                   QLatin1String("text_ru")) {
                            ex->SetTextRu(attr.value().toString());
                        } else if (attr.name().toString() ==
                                   QLatin1String("completed")) {
                            ex->SetCompleted(attr.value().toInt());
                        } else if (attr.name().toString() ==
                                   QLatin1String("alphabit")) {
                            ex->SetAlphabit(attr.value().toString());
                        }
                    }
                    _exercises.push_back(ex);
                } else if (reader.name() == QString("test")) {
                    QString input;
                    QString output;

                    foreach (const QXmlStreamAttribute& attr,
                             reader.attributes()) {
                        if (attr.name().toString() == QLatin1String("input")) {
                            input = attr.value().toString();
                        } else if (attr.name().toString() ==
                                   QLatin1String("output")) {
                            output = attr.value().toString();
                        }
                    }
                    std::shared_ptr<Exercise> ex = _exercises.back();
                    ex->AddTest(input, output);
                }
            }
        }
        file.close();
    }
}

void Configuration::Init(QApplication* app) {
    LoadExercises();
    _app = app;
    _default_location = QDir::homePath();
    QFile file(configurationFilePath());

    if (file.exists() && !file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox box;
        box.setText(QString{"Error: can't open config file: "} +
                    configurationFilePath());
        box.exec();
    } else {
        QXmlStreamReader reader(file.readAll());
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();

            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == QString("project")) {
                    QString name;
                    name.append(reader.readElementText());
                    if (QFile(name).exists()) {
                        _recent_projects.push_back(name);
                    }
                } else if (reader.name() == QString("darkTheme")) {
                    _dark_theme = reader.readElementText().trimmed() ==
                                  QLatin1String("1");
                }
            }
        }
        file.close();
    }
}

void Configuration::Save() {
    QFile file(configurationFilePath());

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox box;
        box.setText(QString{"Error: can't open config file: "} +
                    configurationFilePath());
        box.exec();
    } else {
        QXmlStreamWriter stream(&file);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();
        stream.writeStartElement("config");
        stream.writeStartElement("projects");
        for (const QString& name : _recent_projects) {
            stream.writeTextElement("project", name);
        }
        stream.writeEndElement();
        stream.writeTextElement("darkTheme", _dark_theme ? QStringLiteral("1")
                                                         : QStringLiteral("0"));
        stream.writeEndElement();
        stream.writeEndDocument();
        file.close();
    }
}

bool Configuration::IsDarkTheme() const { return _dark_theme; }

void Configuration::SetDarkTheme(bool enabled) { _dark_theme = enabled; }

Configuration& Configuration::GetInstance() { return instance; }
