#include "configuration.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QFileInfo>


Configuration Configuration::instance;

Configuration::Configuration()
{

}

const std::vector<QString>& Configuration::GetRecentProjects(){
    return this->_recent_projects;
}

const std::vector<std::shared_ptr<Exercise>>& Configuration::GetExercises(){
    return _exercises;
}

void  Configuration::AddRecentProject(QString & name){
    bool found = false;

    for(auto &n:_recent_projects)
        if(n==name) found = true;

    if(!found)
        this->_recent_projects.insert(_recent_projects.begin(),name);

    if(_recent_projects.size()>10) _recent_projects.pop_back();
}

const QString& Configuration::GetDefaultLocation(){
    return _default_location;
}

void   Configuration::LoadExercises(){
    //

    QFile file("://config/exercises.xml");

    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QXmlStreamReader reader(file.readAll());
        std::shared_ptr<Exercise> ex;
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == QString("exercise")) {
                    ex = std::make_shared<Exercise>();
                    foreach(const QXmlStreamAttribute &attr, reader.attributes()) {
                        if (attr.name().toString() == QLatin1String("id")) {
                            ex->SetIndex(attr.value().toInt());
                        } else
                            if (attr.name().toString() == QLatin1String("text_en")) {
                                ex->SetTextEn(attr.value().toString());
                            } else
                                if (attr.name().toString() == QLatin1String("text_ru")) {
                                    ex->SetTextRu(attr.value().toString());
                                } else
                                    if (attr.name().toString() == QLatin1String("completed")) {
                                        ex->SetCompleted(attr.value().toInt());
                                    } else
                                        if (attr.name().toString() == QLatin1String("alphabit")) {
                                            ex->SetAlphabit(attr.value().toString());
                                        }
                    }
                    _exercises.push_back(ex);
                } else
                    if (reader.name() == QString("test")) {
                        QString input;
                        QString output;

                        foreach(const QXmlStreamAttribute &attr, reader.attributes()) {
                            if (attr.name().toString() == QLatin1String("input")) {
                                input=attr.value().toString();
                            } else
                                if (attr.name().toString() == QLatin1String("output")) {
                                    output = attr.value().toString();
                                }
                        }
                        std::shared_ptr<Exercise> ex = _exercises.back();
                        ex->AddTest(input,output);
                    }

            }
        }
        file.close();
    }
}

void   Configuration::Init(QApplication *app){
    LoadExercises();
    _app = app;
    _default_location  = _app->applicationDirPath();
    QString filename = _default_location;
    filename.append("/config.xml");
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        //        QMessageBox box;
        //      box.setText(filename);
        //        box.exec();
        //qDebug(qPrintable(QString("Error: Cannot read file %1 %2").arg(filename).arg(file.errorString())));
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
        stream.writeStartElement("config");
        stream.writeStartElement("projects");
        for(const QString &name: _recent_projects){
            stream.writeTextElement("project",name);
        }
        stream.writeEndElement();
/*
        stream.writeStartElement("exercises");
        for(std::shared_ptr<Exercise> ex:GetExercises()){
            stream.writeStartElement("exercise");
            stream.writeAttribute("id",QString::number(ex->GetIndex()));
            stream.writeAttribute("text_en",ex->GetTextEn());
            stream.writeAttribute("text_ru",ex->GetTextRu());
            stream.writeAttribute("completed",QString::number(ex->GetCompleted()?1:0));
            stream.writeAttribute("alphabit",ex->GetAlphabit());
            for(auto p:ex->GetTests()){
                stream.writeStartElement("test");
                stream.writeAttribute("input",p.first);
                stream.writeAttribute("output",p.second);
                stream.writeEndElement();
            }
            stream.writeEndElement();
        }
        stream.writeEndElement();*/
        stream.writeEndElement();
        stream.writeEndDocument();
        file.close();
    }

}

Configuration& Configuration::GetInstance(){
    return instance;
}
