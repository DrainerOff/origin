/*!
* \file main.cpp
* \brief ImgSizer
*/

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
#include "conio.h"

using namespace std;

/// Структура хранящая размеры изображения
struct size
{
    int width = 0; ///< Ширина изображения
    int height = 0; ///< Высота изображения
};

/// Структура хранящая данные об изображении
struct data
{
    QString path; ///< Путь к папке, где находится изображение
    QString full_name; ///< Имя изображения с параметрами изображения
    QString size_w; ///< Ширина изображения
    QString size_h; ///< Высота изображения
    size size; ///< Размеры изображения
    QString mode; ///< Тип обработки изображения
    QString extension; ///< Расширение изображения
    QString name; ///< Имя изображения
};

data path_analayze(QString picture_path);

bool IsFileCorrect(data picture_data);

void changeIMG(data picture);

void testing();

/*!
 Основная функция программы
 \param argc[in] - Количество введенных параметров командной строки
 \param *argv[][in] - Введенные парметры командной строки
 \return 0 - Успешное выполнение
 */
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

    if(picture_path == "Test" || picture_path == "test")
    {
        testing();
        return 0;
    }


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

/*!
 Разбивает путь к изображению на составляющие
 \param picture_path[in] - Путь к изображению
 \return Структуру типа data
 */
data path_analayze(QString picture_path)
{
    data picture;

    picture.path = picture_path;


    picture.full_name = picture_path;
    picture.full_name = picture.full_name.remove(0,picture.full_name.lastIndexOf('\\')+1); // Имя файла

    // Поиск размеров
    picture.size_w = picture.full_name;
    picture.size_w = picture.size_w.remove(picture.size_w.lastIndexOf('('),picture.full_name.length()+1);
    if(picture.size_w.contains("("))
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
    int x =  picture.mode.size();
    picture.mode = picture.mode.remove(picture.mode.lastIndexOf(')'), x+1);

    // Поиск расширения файла
    picture.extension = picture.full_name;
    picture.extension = picture.extension.remove(0, picture.extension.lastIndexOf('.')+1);

    // Поиск имени изображения
    picture.path = picture.path.remove(picture_path.length() - picture.full_name.length(), picture_path.length()+1);
    picture.name =  picture.full_name;
    picture.name = picture.name.remove(0,picture.name.indexOf(")")+1);

    return picture;
}

/*!
 Проверяет можно ли обработать данное изображение
 \param picture_data[in] - Структура типа data, хранящая информацию об изображении
 \return 0 - Файл не подходит для обработки
 \return 1 - Файл подходит для обработки
 */
bool IsFileCorrect(data picture_data)
{
    bool decision = 0;

    QTextStream cout(stdout);
    QTextStream cin(stdin);

    picture_data.extension = picture_data.extension.toLower();
    picture_data.mode = picture_data.mode.toLower();

    // Проверка на наличие файла
    QString test_path = picture_data.path + picture_data.name;
    QDir test_dir_;
    if(test_dir_.exists(test_path))
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Файл с таким именем отсутствует") << endl;
        return 0;
    }


    // Проверка на поддерживаемое расширение файла
    if(picture_data.extension == "png" || picture_data.extension == "jpg" || picture_data.extension == "bmp")
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
    if(picture_data.size.width >= 10 && picture_data.size.width <= 10000 && picture_data.size.height >= 10 && picture_data.size.height <= 10000)
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Изображение выходит за диапазон допустимого размера 10x10 - 10000x10000") << endl;
        return 0;
    }

    // Проверка на правильность введенного типа обработки
    if(picture_data.mode == "prop" || picture_data.mode == "exact")
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Неправильно введен тип обработки изображения") << endl;
        return 0;
    }

    // Проверка на соотношение размера исходной картинки и требуемого
    QImage test_pic(picture_data.path + picture_data.name);
    if(picture_data.size.width <= test_pic.width() && picture_data.size.height <= test_pic.height())
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Указанный размер больше исходного изображения") << endl;
        return 0;
    }

    // Проверка на наличие этого изображения среди обработанных
    QString test_name = picture_data.path + "Resized\\" + picture_data.full_name;
    QDir test_dir;
    if(!test_dir.exists(test_name))
    {
        decision = 1;
    }
    else
    {
        cout << QString::fromUtf8("Данное изображение уже было обработано") << endl;
        return 0;
    }

    return decision;
}

/*!
 Функция обрезки изображения по заданным параметрам
 \param data[in] - Структура типа data, хранящая информацию об изображении
 */
void changeIMG(data picture)
{
    char extension_cpy[4] ;
    strcpy(extension_cpy, picture.extension.toStdString().c_str());

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
        scaled.save(link, extension_cpy, -1);
    }
    else if(picture.mode == "Exact" || picture.mode == "exact")
    {
        size real_picture_size;

        QImage scaled = original.scaled(picture.size.width, picture.size.height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        scaled.save(link, extension_cpy, -1);

        real_picture_size.width = scaled.width();
        real_picture_size.height = scaled.height();

        if(real_picture_size.height - picture.size.height > 0)
        {
            int height = (real_picture_size.height - picture.size.height)/2;

            QRect rect_first(0, height, picture.size.width, picture.size.height);
            QImage cropped = scaled.copy(rect_first);
            cropped.save(link, extension_cpy, -1);
        }

        else if(real_picture_size.width - picture.size.width > 0)
        {

            int width = (real_picture_size.width - picture.size.width)/2;

            QRect rect_first(width, 0, picture.size.width, picture.size.height);
            QImage cropped = scaled.copy(rect_first);
            cropped.save(link, extension_cpy, -1);
        }

    }
}

/*!
 Тестирующая функция
 */
void testing()
{
    data Default_prop, Default_exact, Jpg_quality,Max_size, Minimal_size, bigger_size;

    Default_prop.path = QCoreApplication::applicationDirPath() + "/";
    Default_prop.path.replace("/","\\");

    Default_prop.extension = "jpg";
    Default_prop.mode = "Prop";
    Default_prop.full_name = "100_100(Prop)Default prop.jpg";
    Default_prop.name = "Default prop.jpg";
    Default_prop.size.height = 100;
    Default_prop.size.width = 100;

    changeIMG(Default_prop);

    QString pth = QCoreApplication::applicationDirPath() + "/" + "\\Perfect\\100_100(Prop)Default prop.jpg";
    pth.replace("/","\\");

    QString pth2 = QCoreApplication::applicationDirPath() + "/" + "\\Resized\\100_100(Prop)Default prop.jpg";
    pth.replace("/","\\");

    QImage original(pth);

    QImage pic(pth2);

    if(original != pic)
    {
        cout << "Error in test1 - Default prop" << endl;
    }
    else
    {
        cout << "test1(Default prop)is ok" << endl;
    }

    Default_exact.path = QCoreApplication::applicationDirPath() + "/";
    Default_exact.path.replace("/","\\");

    Default_exact.extension = "png";
    Default_exact.mode = "Exact";
    Default_exact.full_name = "350_163(Exact)Default exact.png";
    Default_exact.name = "Default exact.png";
    Default_exact.size.height = 163;
    Default_exact.size.width = 350;

    changeIMG(Default_exact);

    QString pth3 = QCoreApplication::applicationDirPath() + "/" + "\\Perfect\\350_163(Exact)Default exact.png";
    pth3.replace("/","\\");

    QString pth4 = QCoreApplication::applicationDirPath() + "/" + "\\Resized\\350_163(Exact)Default exact.png";
    pth.replace("/","\\");

    QImage original2(pth3);
    QImage pic2(pth4);

    if(original2 != pic2)
    {
        cout << "Error in test2 - Default_exact" << endl;
    }
    else if(original2 == pic2)
    {
        cout << "test2(Default_exact) is ok" << endl;
    }

    Jpg_quality.path = QCoreApplication::applicationDirPath() + "/";
    Jpg_quality.path.replace("/","\\");

    Jpg_quality.extension = "jpg";
    Jpg_quality.mode = "Prop";
    Jpg_quality.full_name = "350_350(Prop)Jpg quality.jpg";
    Jpg_quality.name = "Jpg quality.jpg";
    Jpg_quality.size.height = 350;
    Jpg_quality.size.width = 350;

    changeIMG(Jpg_quality);

    QString pth5 = QCoreApplication::applicationDirPath() + "/" + "\\Perfect\\350_350(Prop)Jpg quality.jpg";
    pth3.replace("/","\\");

    QString pth6 = QCoreApplication::applicationDirPath() + "/" + "\\Resized\\350_350(Prop)Jpg quality.jpg";
    pth.replace("/","\\");

    QImage original3(pth5);
    QImage pic4(pth6);

    if(original3 != pic4)
    {
        cout << "Error in test3 - Jpg_quality" << endl;
    }
    else if(original3 == pic4)
    {
        cout << "test3(Jpg_quality) is ok" << endl;
    }

    Max_size.path = QCoreApplication::applicationDirPath() + "/";
    Max_size.path.replace("/","\\");

    Max_size.extension = "jpg";
    Max_size.mode = "Exact";
    Max_size.full_name = "1920_1080(Exact)Max size.jpg";
    Max_size.name = "Max size.jpg";
    Max_size.size.height = 1080;
    Max_size.size.width = 1920;

    changeIMG(Max_size);

    QString pth7 = QCoreApplication::applicationDirPath() + "/" + "\\Perfect\\1920_1080(Exact)Max size.jpg";
    pth3.replace("/","\\");

    QString pth8 = QCoreApplication::applicationDirPath() + "/" + "\\Resized\\1920_1080(Exact)Max size.jpg";
    pth.replace("/","\\");

    QImage original4(pth7);
    QImage pic5(pth8);

    if(original4 != pic5)
    {
        cout << "Error in test4 - Max size" << endl;
    }
    else if(original4 == pic5)
    {
        cout << "test4(Max size) is ok" << endl;
    }

    Minimal_size.path = QCoreApplication::applicationDirPath() + "/";
    Minimal_size.path.replace("/","\\");

    Minimal_size.extension = "png";
    Minimal_size.mode = "Exact";
    Minimal_size.full_name = "10_10(Exact)Minimal size.png";
    Minimal_size.name = "Minimal size.png";
    Minimal_size.size.height = 10;
    Minimal_size.size.width = 10;

    changeIMG(Minimal_size);

    QString pth9 = QCoreApplication::applicationDirPath() + "/" + "\\Perfect\\10_10(Exact)Minimal size.png";
    pth3.replace("/","\\");

    QString pth10 = QCoreApplication::applicationDirPath() + "/" + "\\Resized\\10_10(Exact)Minimal size.png";
    pth.replace("/","\\");

    QImage original5(pth9);
    QImage pic6(pth10);

    if(original5 != pic6)
    {
        cout << "Error in test5 - Minimal_size" << endl;
    }
    else if(original5 == pic6)
    {
        cout << "test5(Minimal_size) is ok" << endl;
    }
}






