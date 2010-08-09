#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <gphoto2/gphoto2-camera.h>

class MyThread : public QThread
{
    Q_OBJECT

public:
    MyThread();
    ~MyThread();

    void setData(GPContext *lcontext, Camera *lcamera);
    int result_check(int retval, QString message);

    void run();
    void mysleep(unsigned long time);
    bool running;

signals:
    void previewAvailable(CameraFile *cf);


private:
    GPContext *context;
    Camera *camera;
};

#endif // MYTHREAD_H
