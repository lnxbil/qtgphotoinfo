#include "imagedisplaydialog.h"
#include "ui_imagedisplaydialog.h"

#include "QDebug"


//#define RESIZEME

ImageDisplayDialog::ImageDisplayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageDisplayDialog)
{
    ui->setupUi(this);
}

ImageDisplayDialog::~ImageDisplayDialog()
{
    delete ui;
}

void ImageDisplayDialog::reset()
{
    this->setWindowTitle(oldtitle);
    images = 0;
    started = false;
}


void ImageDisplayDialog::setPixmap(const QPixmap &image)
{
    if (! started)
    {
        started = true;
        myTimer.restart();
        overallTimer.restart();
        oldtitle = this->windowTitle();
        images = 0;
        cimages = 0;
    }

#ifdef RESIZEME
    QSize s = image.size();
    int maxval = 768;

    if ( s.width() > s.height())
        ui->label->setPixmap( image.scaledToWidth(maxval) );
    else
        ui->label->setPixmap (image.scaledToHeight(maxval) );
#else
    ui->label->setPixmap(image);
#endif

    images++;
    cimages++;

    if (images % 10 == 0)
    {
        QString fps;
        int time = myTimer.elapsed();
        int ctime = 1;
        if (time == 0)
            time = 1;

        if ( overallTimer.elapsed() > 2000 )
        {
            ctime = overallTimer.elapsed();
            fpsstr.sprintf("%5.2f", cimages * 1000.0 / ctime);
            cimages = 0;
            overallTimer.restart();
        }
        fps.sprintf(" - %s FPS - %d seconds running - %d images captured ", fpsstr.toStdString().c_str(), time / 1000, images);
        this->setWindowTitle(oldtitle + fps);
    }
}
