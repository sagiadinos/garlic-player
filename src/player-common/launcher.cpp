#include "launcher.h"

Launcher::Launcher(QObject *parent) : QObject(parent)
{

}
void Launcher::sendConfig(QString path)
{
#if defined  Q_OS_ANDROID
    QAndroidJniObject java_file_path = QAndroidJniObject::fromString(path + "configuration.xml");
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity",
                                              "applyConfig",
                                              "(Ljava/lang/String;)V",
                                              java_file_path.object<jstring>()
                                              );
#endif

}

void Launcher::rebootOS(QString task_id)
{
#if defined  Q_OS_ANDROID
    QAndroidJniObject java_task_id = QAndroidJniObject::fromString(task_id);
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity",
                                              "rebootOS",
                                              "(Ljava/lang/String;)V",
                                              java_task_id.object<jstring>()
                                               );
#endif
}

void Launcher::installSoftware(QString file_path)
{
#if defined  Q_OS_ANDROID
   QAndroidJniObject java_file_path = QAndroidJniObject::fromString(file_path);
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity",
                                              "installSoftware",
                                              "(Ljava/lang/String;)V",
                                              java_file_path.object<jstring>());
#endif
}

void Launcher::toggleScreenActivity(bool is_on)
{

#if defined  Q_OS_ANDROID
    if (is_on)
    {
        QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity", "setScreenOn");
    }
    else
    {
        QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity", "setScreenOff");
    }
#elif defined Q_OS_LINUX
    QProcess process;
    QString program = "xset";
    QStringList arguments;
    if (is_on)
    {
        arguments << "dpms " << "force" << "on";
    }
    else
    {
        arguments << "dpms " << "force" << "off";
    }
    process.start(program, arguments);
#elif defined Q_OS_OSX
#elif defined Q_OS_WIN32
#endif


//    is_screen_on = is_on;
}

void Launcher::sendClosePlayerCorrect()
{
#if defined  Q_OS_ANDROID
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity", "closePlayerCorrect");
#endif
}
