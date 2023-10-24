#include "configuration.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

Configuration Configuration::instance;

Configuration::Configuration()
{

}

const std::vector<QString>& Configuration::GetRecentProjects(){
   return this->_recent_projects;
}

void  Configuration::AddRecentProject(QString & name){
    bool found = false;

    for(auto n:_recent_projects)
        if(n==name) found = true;

    if(!found)
     this->_recent_projects.insert(_recent_projects.begin(),name);

    if(_recent_projects.size()>10) _recent_projects.pop_back();
}

const QString& Configuration::GetDefaultLocation(){
    return _default_location;
}

void   Configuration::Init(QApplication *app){
    _app = app;
    qDebug() << "Current path:" << _app->applicationDirPath();

    _default_location  = _app->applicationDirPath();
    QString filename = _app->applicationDirPath();
    filename.append("/config.xml");
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text))
     {
        //    qDebug(qPrintable(QString("Error: Cannot read file %1 %2").arg(filename).arg(file.errorString())));
     } else
    {
        QXmlStreamReader reader(file.readAll());

        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == QString("project")) {
                  QString name;
                  name.append(reader.readElementText());
                  _recent_projects.push_back(name);
              }
          }
        }
        file.close();
    }
}

void Configuration::Save(){
    QString filename = _app->applicationDirPath();
    filename.append("/config.xml");
    QFile file(filename);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        //qDebug(qPrintable(QString("Error: Cannot write file %1 %2").arg(filename).arg(file.errorString())));
    } else {
        QXmlStreamWriter stream(&file);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();
        stream.writeStartElement("projects");
        for(QString name: _recent_projects){
        stream.writeTextElement("project", name);
        }
        stream.writeEndElement();
        stream.writeEndDocument();
        file.close();
    }

}

Configuration& Configuration::GetInstance(){
    return instance;
}
