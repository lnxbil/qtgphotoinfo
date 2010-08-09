#ifndef IMAGEDISPLAYDIALOG_H
#define IMAGEDISPLAYDIALOG_H

#include <QDialog>
#include <QTime>
#include <QString>

namespace Ui {
    class ImageDisplayDialog;
}

class QPixmap;

class ImageDisplayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageDisplayDialog(QWidget *parent = 0);
    ~ImageDisplayDialog();

    void setPixmap( const QPixmap &image);
    void reset();

private:
    Ui::ImageDisplayDialog *ui;
    bool started;

    // Measurement
    QTime myTimer, overallTimer;
    QString oldtitle, fpsstr;
    unsigned long images, cimages;

};

#endif // IMAGEDISPLAYDIALOG_H
