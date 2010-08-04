#include "imagedisplaydialog.h"
#include "ui_imagedisplaydialog.h"

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


void ImageDisplayDialog::setPixmap(const QPixmap &image)
{
    QSize s = image.size();
    int maxval = 768;

    if ( s.width() > s.height())
        ui->label->setPixmap( image.scaledToWidth(maxval) );
    else
        ui->label->setPixmap (image.scaledToHeight(maxval) );

}
