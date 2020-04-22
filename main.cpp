#include <QCoreApplication>
#include <iostream>
#include <QString>
#include <string.h>
#include <QTextStream>
#include <QFile>
#include <QtGui/QImage>
#include <QtGui/QPicture>
#include <QtGui/QPainter>
#include <QDir>

using namespace std;

struct size
{
    int width = 0;
    int height = 0;
};

struct data
{
    QString way;
    QString full_name;
    QString size_w;
    QString size_h;
    size size;
    QString mode;
    QString type;
    QString name;
};

data way_analayze(QString picture_way);

bool IsFileCorrect(data picture_data);

void changeIMG(data picture);

int main(int argc, char *argv[])
{
    return 0;
}

data way_analayze(QString picture_way)
{
    data picture;

    picture.way = picture_way;


    picture.full_name = picture_way;
    picture.full_name = picture.full_name.remove(0,picture.full_name.lastIndexOf('\\')+1); // Имя файла

    // Поиск размеров
    picture.size_w = picture.full_name;
    picture.size_w = picture.size_w.remove(picture.size_w.lastIndexOf('('),picture.full_name.length()+1);
    picture.size_h = picture.size_w;
    picture.size_w = picture.size_w.remove(picture.size_w.lastIndexOf('_'),sizeof(picture.size_w)+1);
    picture.size_h = picture.size_h.remove(0,picture.size_h.lastIndexOf('_')+1);

    // Разделение размеров на ширину и высоту
    picture.size.width = picture.size_w.toInt();
    picture.size.height = picture.size_h.toInt();

    // Поиск типа обработки
    picture.mode = picture.full_name;
    picture.mode = picture.mode.remove(0, picture.mode.lastIndexOf('(')+1);
    picture.mode = picture.mode.remove(picture.mode.lastIndexOf(')'), sizeof(picture.mode)+1);
    picture.mode = picture.mode.remove(picture.mode.lastIndexOf(')', sizeof(picture.mode)));

    // Поиск расширения файла
    picture.type = picture.full_name;
    picture.type = picture.type.remove(0, picture.type.lastIndexOf('.')+1);

    // Поиск имени изображения
    picture.way = picture.way.remove(picture_way.length() - picture.full_name.length(), picture_way.length()+1);
    picture.name =  picture.full_name;
    picture.name = picture.name.remove(0,picture.name.lastIndexOf(')')+1);

    return picture;
}

bool IsFileCorrect(data picture_data)
{
    return 0;
}

