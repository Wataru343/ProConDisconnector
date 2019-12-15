#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QDebug>

static void device_attached_callback(JNIEnv *env, jobject obj, jint vid, jint pid, jstring device_name);
static void device_detached_callback(JNIEnv *env, jobject obj, jint vid, jint pid, jstring device_name);

class callback_manager : public QObject
{
    Q_OBJECT
public:
    explicit callback_manager(QObject *parent = nullptr): QObject(parent) {
        QAndroidJniEnvironment env;
        jclass activity = env->FindClass("com/usb/ProConDisconnectActivity");
        JNINativeMethod pmethods[] {
            {"device_attached", "(IILjava/lang/String;)V", reinterpret_cast<void*>(device_attached_callback)},
            {"device_detached", "(IILjava/lang/String;)V", reinterpret_cast<void*>(device_detached_callback)},
        };
        env->RegisterNatives(activity, pmethods, std::size(pmethods));
    }
    ~callback_manager() = default;

public:
    callback_manager(const callback_manager&) = delete;
    callback_manager& operator=(const callback_manager&) = delete;
    callback_manager(callback_manager&&) = delete;
    callback_manager& operator=(callback_manager&&) = delete;

    static callback_manager *instance() {
        static callback_manager manager;
        return &manager;
    }

signals:
    void device_attached(const QString &device_name, int vid, int pid);
    void device_detached(const QString &device_name, int vid, int pid);

public slots:


};

static void device_attached_callback(JNIEnv *env, jobject obj, jint vid, jint pid, jstring device_name) {
    (void)obj;
    jboolean is_copy = false;
    QString str(env->GetStringUTFChars(device_name, &is_copy));

    emit callback_manager::instance()->device_attached(str, vid, pid);
}

static void device_detached_callback(JNIEnv *env, jobject obj, jint vid, jint pid, jstring device_name) {
    (void)obj;
    jboolean is_copy = false;
    QString str(env->GetStringUTFChars(device_name, &is_copy));
    emit callback_manager::instance()->device_detached(str, vid, pid);
}


#endif // CALLBACKS_HPP
