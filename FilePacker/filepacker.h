#ifndef FILEPACKER_H
#define FILEPACKER_H

#include <QString>

class FilePacker
{
public:
    void Pack(QStringList fileList, QString outputFile);
    void Unpack(QString packedFile);
};

#endif // FILEPACKER_H
