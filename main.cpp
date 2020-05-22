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
    QString path;
    QString full_name;
    QString size_w;
    QString size_h;
    size size;
    QString mode;
    QString type;
    QString name;
};

data path_analayze(QString picture_path);

bool IsFileCorrect(data picture_data);

void changeIMG(data picture);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    setlocale(0, "");
    QString picture_path;

    QTextStream cin(stdin);

    if(argc > 1)
    {
        picture_path = argv[1];
    }
    else
        picture_path = cin.readLine(); // ввод пути


    if(!picture_path.contains("\\"))
    {

        picture_path = QCoreApplication::applicationDirPath() + "/" + picture_path;
        picture_path.replace("/","\\");

    }


    QString picture_path_copy = picture_path;

    data picture_data;

    picture_data = path_analayze(picture_path);

    bool error = IsFileCorrect(picture_data);

    if(error == 1)
        changeIMG(picture_data);

    return 0;
}

data path_analayze(QString picture_path)
{
    data picture;

    picture.path = picture_path;


    picture.full_name = picture_path;
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
    picture.path = picture.path.remove(picture_path.length() - picture.full_name.length(), picture_path.length()+1);
    picture.name =  picture.full_name;
    picture.name = picture.name.remove(0,picture.name.lastIndexOf(')')+1);

    return picture;
}

bool IsFileCorrect(data picture_data)
{
    bool decision = 0;

    QTextStream cout(stdout);
    QTextStream cin(stdin);

    picture_data.type = picture_data.type.toLower();
    picture_data.mode = picture_data.mode.toLower();

    // Проверка на поддерживаемое расширение файла
    if(picture_data.type == "png" || picture_data.type == "jpg" || picture_data.type == "bmp")
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Неверный формат файла") << endl;
        return 0;
    }

    // Проверка на поддерживаемый формат имени
    if((picture_data.full_name.contains("(") && picture_data.full_name.contains(")")) && picture_data.size.width != 0 && picture_data.size.height != 0 && picture_data.full_name.contains("_"))
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Неверное имя файла") << endl;
        return  0;
    }

    // Проверка на поддерживаемый размер изображения
    if(picture_data.size.width > 10 && picture_data.size.width < 10000 && picture_data.size.height > 10 && picture_data.size.height < 10000)
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Изображение выходит за диапазон допустимого размера 10x10 - 10000x10000") << endl;
        return 0;
    }

    // Проверка на правильность введнного типа обработки
    if(picture_data.mode == "prop" || picture_data.mode == "exact")
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Неправильно введен тип обработки изображения") << endl;
        return 0;
    }

    return decision;
}

void changeIMG(data picture)
{
    char type_cpy[4] ;
    strcpy(type_cpy, picture.type.toStdString().c_str());

    QString path(picture.path+"Resized");
    QDir dir;
    if(!dir.exists(path))
        dir.mkpath(path);

    QString link_to_picture = picture.path + picture.name;
    QString link = picture.path+"Resized"+"\\" + picture.full_name;

    QRect rect(0, 0, picture.size.width, picture.size.height);
    QImage original(link_to_picture);

    if(picture.mode == "Prop" || picture.mode == "prop")
    {
        QImage scaled = original.scaled(picture.size.width, picture.size.height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        scaled.save(link, type_cpy, -1);
    }
    else if(picture.mode == "Exact" || picture.mode == "exact")
    {
        size real_picture_size;

        QImage scaled = original.scaled(picture.size.width, picture.size.height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        scaled.save(link, type_cpy, -1);

        real_picture_size.width = scaled.width();
        real_picture_size.height = scaled.height();

        if(real_picture_size.height - picture.size.height > 0)
        {
            int height = (real_picture_size.height - picture.size.height)/2;

            QRect rect_first(0, height, picture.size.width, picture.size.height);
            QImage cropped = scaled.copy(rect_first);
            cropped.save(link, type_cpy, -1);
        }

        else if(real_picture_size.width - picture.size.width > 0)
        {

            int width = (real_picture_size.width - picture.size.width)/2;

            QRect rect_first(width, 0, picture.size.width, picture.size.height);
            QImage cropped = scaled.copy(rect_first);
            cropped.save(link, type_cpy, -1);
        }

    }

}
