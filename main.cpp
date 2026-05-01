#include <cstdio>
#include <QApplication>
#include <QImageReader>
#include <QFile>
#include <QByteArray>

QApplication* app = nullptr;

int main(int argc, char** argv)
{
    app = new QApplication(argc, argv);

    if (argc < 2)
        return -1;

    QImageReader reader(argv[1]);
    FILE* f = fopen("./out.bin", "wb");
    FILE* f2 = fopen("./out.tim", "wb");
    if (f)
    {
        QImage img;
        int milisec = 0;
        fwrite(&milisec, 4, 1, f2);
        milisec = reader.nextImageDelay();
        while (reader.read(&img)) {
            if (!img.isNull()) {
                //file.write(QByteArray(img.scaled(QSize(80, 25)).convertedTo(QImage::Format_Grayscale8).constBits()).data(), 2000);
                fwrite((void*)img.scaled(QSize(80, 25)).convertedTo(QImage::Format_Grayscale8).constBits(), 2000, 1, f);
            }
            fwrite(&milisec, 4, 1, f2);
            milisec += reader.nextImageDelay();
        }
    }
    fclose(f);

    printf("Ok");
    return 0;
}