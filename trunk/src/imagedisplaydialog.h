#ifndef IMAGEDISPLAYDIALOG_H
#define IMAGEDISPLAYDIALOG_H

#include <QDialog>

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

private:
    Ui::ImageDisplayDialog *ui;
};

#endif // IMAGEDISPLAYDIALOG_H
