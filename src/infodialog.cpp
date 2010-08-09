#include "QMessageBox"
#include "QDebug"

#include "mythread.h"
#include "infodialog.h"
#include "imagedisplaydialog.h"
#include "ui_infodialog.h"


#include <gphoto2/gphoto2-version.h>

static int _lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child)
{
    int ret;
    ret = gp_widget_get_child_by_name (widget, key, child);
    if (ret < GP_OK)
        ret = gp_widget_get_child_by_label (widget, key, child);
    return ret;

}








InfoDialog::InfoDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::InfoDialog)
{
    ui->setupUi(this);

    const char ** version = gp_library_version(GP_VERSION_SHORT);
    ui->lb_version_local->setText( QString(version[0]) );


    context = NULL;
    camera = NULL;
}

InfoDialog::~InfoDialog()
{
    gp_camera_exit(camera,context);

    delete ui;
    if (context != NULL)
        free(context);

    if (camera != NULL)
        gp_camera_free (camera);
}

void InfoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void InfoDialog::logMessage(const QString message)
{
    ui->log->appendPlainText(message);
    QApplication::processEvents();
}

int InfoDialog::result_check(int retval, QString message) {
    if (retval < GP_OK) {
        QString log;
        if (message.size() == 0)
            log += QString(gp_result_as_string(retval));
        else
            log.sprintf("%s: %s\n",message.toStdString().c_str(),gp_result_as_string(retval));
        logMessage(log);
    }
    return retval;
}


void InfoDialog::rereadCameraInfo()
{
    // Parameters and Variables
    CameraStorageInformation    *storage;

    int ret = 0;
    int tmp = 0;

    // Creating Context and Camera
    sdialog.clearItems();
    logMessage(QString("Initializing camera"));
    context = gp_context_new();
    gp_camera_new (&camera);
    ret = result_check(gp_camera_init (camera, context),"gp_camera_init");

    if (ret < GP_OK) {
        logMessage(QString("No camera connected!"));
        gp_camera_free (camera);
        free(context);
        camera = NULL;
        context = NULL;
        return;
    }


    // summary
    CameraText sum;
    result_check(gp_camera_get_summary(camera,&sum,context),"gp_camera_get_summary");
    logMessage(QString(sum.text));


    // manual
    CameraText manual;
    result_check(gp_camera_get_manual(camera,&manual,context),"gp_camera_get_manual");
    logMessage(QString(manual.text));


    // about
    CameraText about;
    result_check(gp_camera_get_about(camera,&about,context),"gp_camera_get_about");
    logMessage(QString(about.text));


    // Reading Storage information
    logMessage(QString("Loading Storage configuration"));
    ret = result_check(gp_camera_get_storageinfo( camera, &storage, &tmp, context),"gp_camera_get_storageinfo");

    ui->lb_images_left->setText( QString::number( storage->freeimages ));
    ui->lb_space_total->setText( QString::number( storage->capacitykbytes ));
    ui->lb_space_left->setText( QString::number( storage->freekbytes ));

    free(storage);


    // Camera Settings
    CameraWidget *config = NULL;
    logMessage(QString("Read configuration"));
    ret = result_check(gp_camera_get_config (camera, &config, context),"gp_camera_get_config");
    canon_enable_capture(camera, context, config);

    populateWithConfigs(config);

    gp_widget_free (config);


    // Abilities
    CameraAbilities abilities;
    logMessage(QString("Checking camera abilities"));
    ret = result_check(gp_camera_get_abilities (camera, &abilities),"gp_camera_get_abilities");

    if (ret >= GP_OK)
    {
        // rewrite model
        ui->lb_model->setText( QString(abilities.model));
        switch (abilities.status)
        {
            case GP_DRIVER_STATUS_PRODUCTION: ui->lb_quality->setText(tr("production")); break;
            case GP_DRIVER_STATUS_TESTING: ui->lb_quality->setText(tr("testing")); break;
            case GP_DRIVER_STATUS_EXPERIMENTAL: ui->lb_quality->setText(tr("experimental")); break;
            case GP_DRIVER_STATUS_DEPRECATED: ui->lb_quality->setText(tr("deprecated")); break;
            default: ui->lb_quality->setText(tr("unknown"));
        }
        //printf("Library  : %s\n", abilities.library);
        //printf("  General: "); printfBinary(abilities.operations);

        ui->lb_capture_audio->setEnabled(false); ui->pb_capture_audio->setEnabled(true);
        ui->lb_capture_image->setEnabled(false); ui->pb_capture_image->setEnabled(true);
        ui->lb_capture_movie->setEnabled(false); ui->pb_capture_movie->setEnabled(true);
        ui->lb_capture_preview->setEnabled(false); ui->pb_capture_preview->setEnabled(true);
        ui->lb_driver_configuration->setEnabled(false); ui->pb_driver_configuration->setEnabled(true);
        ui->pb_read_settings->setEnabled(true);

        int a = abilities.operations;
        if ( a != 0 )
        {
            ui->lb_capture_image->setEnabled( (a & GP_OPERATION_CAPTURE_IMAGE) > 0 );
            ui->lb_capture_movie->setEnabled( (a & GP_OPERATION_CAPTURE_VIDEO) > 0);
            ui->lb_capture_audio->setEnabled(  (a & GP_OPERATION_CAPTURE_AUDIO) > 0);
            ui->lb_capture_preview->setEnabled( (a & GP_OPERATION_CAPTURE_PREVIEW) > 0);
            ui->lb_driver_configuration->setEnabled( (a & GP_OPERATION_CONFIG) > 0);
        }
        else
        {
            QString log;
            log.sprintf("Abilities: %s\n", gp_result_as_string(ret));

            logMessage( log );
        }
    }
    logMessage(QString("Initialization complete and every information is read"));
}



void InfoDialog::canon_enable_capture(Camera *camera, GPContext *context, CameraWidget *config)
{
    int onoff = 1;
    CameraWidgetType type;
    CameraWidget *child;
    int ret;

    printf("CANON ENABLE CAPTURE Config is at %p\n", config);
    ret = _lookup_widget (config, "capture", &child);
    if (ret < GP_OK) {
        fprintf (stderr, "lookup widget failed: %d\n", ret);
        goto out;
    }

    ret = gp_widget_get_type (child, &type);
    if (ret < GP_OK) {
        fprintf (stderr, "widget get type failed: %d\n", ret);
        goto out;
    }
    switch (type) {
    case GP_WIDGET_TOGGLE:
        break;
    default:
        fprintf (stderr, "widget has bad type %d\n", type);
        ret = GP_ERROR_BAD_PARAMETERS;
        goto out;
    }
    /* Now set the toggle to the wanted value */
    ret = gp_widget_set_value (child, &onoff);
    if (ret < GP_OK) {
        fprintf (stderr, "toggling Canon capture to %d failed with %d\n", onoff, ret);
        goto out;
    }
    /* OK */
    ret = gp_camera_set_config (camera, config, context);
    if (ret < GP_OK) {
        fprintf (stderr, "camera_set_config failed: %d\n", ret);
        return;
    }

    out:
    child=NULL;
    return;
}


void InfoDialog::populateWithConfigs(CameraWidget *cc)
{
    int i, childrenTotal;
    const char *name;
    const char *val;
    CameraWidget *child;
    CameraWidgetType widgettype;

    result_check(gp_widget_get_type(cc, &widgettype));
    switch (widgettype) {
    case GP_WIDGET_RADIO:
        {
            result_check(gp_widget_get_name(cc, &name));
            result_check(gp_widget_get_value(cc, &val));

            if (val == NULL)
                break;

            int count;
            count = gp_widget_count_choices(cc);

            QString n = QString(name);
            QString v = QString(val);
            sdialog.addItem(n,v);
            //QString log;
            //logMessage(log.sprintf("%s=%s", name, val));

            /*
            string sname = string(name);
            string sval  = string(val);
            printf("%s=%s\n", name, val);
            if ( sname == "iso" )
                iso = val;
            if ( sname == "aperture" )
                aperture = val;
            if ( sname == "shutterspeed" )
                shutter = val;
            if ( sname == "capturetarget" )
                target = val;
                */
            break;
        }
    case GP_WIDGET_TEXT:
        {
            result_check(gp_widget_get_name(cc, &name));
            result_check(gp_widget_get_value(cc, &val));

            //QString log;
            //logMessage(log.sprintf("%s=%s", name, val));
            QString n = QString(name);
            QString v = QString(val);
            sdialog.addItem(n,v);

            /*
             string sname = string(name);
             string sval  = string(val);

             printf("%s=%s\n", name, val);
             if ( sname == "batterylevel" )
                battery = sval;
             if ( sname == "ownername" )
                 ownername = sval;
             if ( sname == "serialnumber" )
                 serialnumber = sval;
             if ( sname == "model" )
                 model = sval;
             if ( sname == "ptpversion" )
                 ptpversion = sval;
            */
            break;
        }
    case GP_WIDGET_RANGE:
        result_check(gp_widget_get_name(cc, &name));
        break;
    case GP_WIDGET_TOGGLE:
        result_check(gp_widget_get_name(cc, &name));
        break;
    case GP_WIDGET_DATE:
        int date;
        result_check(gp_widget_get_name(cc, &name));
        result_check(gp_widget_get_value(cc, &date));
        printf("Date %s=%d\n", name, date);
        break;
    case GP_WIDGET_WINDOW:
    case GP_WIDGET_SECTION:
        childrenTotal = result_check(gp_widget_count_children(cc));
        for (i = 0; i < childrenTotal; i ++) {
            result_check(gp_widget_get_child(cc, i, &child));
            populateWithConfigs(child);
        }
        break;
        default:
        break;
    }
}


void InfoDialog::on_pb_rescan_clicked()
{
    this->setEnabled(false);
    QApplication::processEvents();
    this->rereadCameraInfo();
    this->setEnabled(true);
}

void InfoDialog::on_pb_capture_image_clicked()
{
    this->setEnabled(false);
    QApplication::processEvents();

    CameraFilePath cfp;
    CameraFile *cf;

    // Taking Image
    result_check( gp_camera_capture( camera, GP_CAPTURE_IMAGE, &cfp, context ),"gp_camera_capture");

    // Downloading Image
    result_check( gp_file_new(&cf),"gp_file_new" );
    result_check( gp_camera_file_get(camera, cfp.folder, cfp.name, GP_FILE_TYPE_NORMAL, cf, context),"gp_camera_file_get");

    QString a = QString("Image Capture - ");
    setImage(cf, a);
    dialog.exec();

    this->setEnabled(true);
}

void InfoDialog::processPreview(CameraFile *cf)
{
    //qDebug() << "Slot processPreview accessed";
    QString a = QString("Preview - ");
    setImage( cf, a, false);
}

void InfoDialog::setImage(CameraFile *cf, QString &method, bool out)
{
    unsigned long mysize = 0;
    const char *data = NULL;

    result_check(gp_file_get_data_and_size(cf, &data, &mysize),"gp_file_get_data_and_size");

    if (mysize == 0 )
        return;

    QPixmap pm;
    pm.loadFromData( (uchar*) data,  (uint) mysize);
    dialog.setPixmap(pm);

    if ( out )
    {
        // Info about the image
        QString logmsg = QString("Size of the Image ") + QString::number(mysize) + QString(" bytes with ");

        dialog.setWindowTitle( method + ui->lb_model->text());
        logmsg += method + QString::number(pm.width()) + QString("x") + QString::number(pm.height()) ;
        logMessage( logmsg);
    }
    gp_file_free(cf);
}

void InfoDialog::notImplementedError()
{
    QMessageBox box;
    box.setText("Not implemented yet");
    box.setIcon(QMessageBox::Information);
    box.exec();
}

void InfoDialog::on_pb_read_settings_clicked()
{
    dialog.exec();
}

void InfoDialog::on_pb_capture_audio_clicked()
{
    notImplementedError();
}

void InfoDialog::on_pb_capture_movie_clicked()
{
    notImplementedError();
}




void InfoDialog::on_pb_capture_preview_clicked()
{
    this->setEnabled(false);
    QApplication::processEvents();

    dialog.setWindowTitle( "Preview - " + ui->lb_model->text());

    MyThread thread;
    connect(&thread, SIGNAL(previewAvailable(CameraFile*)),
            this,    SLOT(processPreview(CameraFile*)));
    thread.setData(context,camera);
    qDebug() << "Starting Thread";
    thread.start();
    qDebug() << "Thread startet";
    dialog.exec();
    qDebug() << "Dialog returned";
    thread.running = false;

    thread.mysleep(1);

    thread.quit();

    this->setEnabled(true);
}
