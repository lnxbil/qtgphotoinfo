#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>
#include <gphoto2/gphoto2-camera.h>

#include "settingsdialog.h"

namespace Ui {
    class InfoDialog;
}

class InfoDialog : public QDialog {
    Q_OBJECT
public:
    InfoDialog(QWidget *parent = 0);
    ~InfoDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InfoDialog *ui;

    void rereadCameraInfo();
    void canon_enable_capture(Camera *camera, GPContext *context, CameraWidget *config);
    void populateWithConfigs(CameraWidget *cc);
    int result_check(int retval, QString message = "");

    void logMessage(const QString message);

    GPContext *context;
    Camera *camera;
    SettingsDialog dialog;

    void notImplementedError();
    void setImage(CameraFile *cf, QString &method);

private slots:
    void on_pb_capture_preview_clicked();
    void on_pb_capture_movie_clicked();
    void on_pb_capture_audio_clicked();
    void on_pb_read_settings_clicked();
    void on_pb_capture_image_clicked();
    void on_pb_rescan_clicked();
};

#endif // INFODIALOG_H
