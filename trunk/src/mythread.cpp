#include "QDebug"
#include "mythread.h"

MyThread::MyThread()  { }
MyThread::~MyThread() { }


void MyThread::setData(GPContext *lcontext, Camera *lcamera)
{
    context = lcontext;
    camera = lcamera;

    running = true;
}

int MyThread::result_check(int retval, QString message) {
    if (retval < GP_OK) {
        QString log;
        if (message.size() == 0)
            log += QString(gp_result_as_string(retval));
        else
            log.sprintf("%s: %s\n",message.toStdString().c_str(),gp_result_as_string(retval));
        qDebug() << log;
    }
    return retval;
}


void MyThread::run()
{
    while (running)
    {
        CameraFile *cf;
        result_check( gp_file_new(&cf),"gp_file_new" );
        result_check( gp_camera_capture_preview( camera, cf, context ),"gp_camera_capture_preview");

        //qDebug() << "Emit Signal";
        emit previewAvailable(cf);

    }
}

void MyThread::mysleep(unsigned long time)
{
    sleep(time);
}
