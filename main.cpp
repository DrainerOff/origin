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

}

bool IsFileCorrect(data picture_data)
{
    return 0;
}

