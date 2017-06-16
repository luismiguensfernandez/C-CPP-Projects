#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QByteArray>
#include <QStringList>

#include "filepacker.h"

void FilePacker::Pack(QStringList fileList, QString outputFile)
{
    QJsonDocument jsonDocument;
    QJsonObject mainObject;
    QJsonArray jsonArray;

    int i=0;
    for(QString fileName: fileList)
    {
        QJsonObject object;
        object["file name"]=fileName;
        QFile fileContent(fileName);
        fileContent.open(QIODevice::ReadOnly);
        QByteArray allContent = fileContent.readAll();
        object["content"]=QString(allContent.toBase64());
        jsonArray.insert(i,object);
        ++i;
        fileContent.close();
    }
    mainObject["filepacker"] = jsonArray;
    jsonDocument.setObject(mainObject);

    QFile outputFileObject(outputFile);
    outputFileObject.open(QIODevice::WriteOnly);
    outputFileObject.write(qCompress(jsonDocument.toBinaryData()));
    outputFileObject.close();
}

void FilePacker::Unpack(QString packedFile)
{
    QFile packedFileContent(packedFile);
    packedFileContent.open(QIODevice::ReadOnly);

    QJsonDocument jsonDocument;
    jsonDocument.fromBinaryData(qUncompress(packedFileContent.readAll()));
    packedFileContent.close();

    QJsonObject mainObject = jsonDocument.object();
    QJsonArray jsonArray = mainObject["filepacker"].toArray();

    for(int i = 0; i < jsonArray.size() ; ++i)
    {
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        QString fileName = jsonObject["file name"].toString();
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QByteArray byteArray = jsonObject["content"].toString().toLocal8Bit();
        file.write(QByteArray::fromBase64(byteArray));
        file.close();
    }
}
