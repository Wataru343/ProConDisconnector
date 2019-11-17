#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>

static void device_attached_callback(JNIEnv *env, jobject obj, jint vid, jint pid, jstring device_name);

class callback_manager : public QObject
{
    Q_OBJECT
public:
    explicit callback_manager(QObject *parent = nullptr): QObject(parent) {
        QAndroidJniEnvironment env;
        jclass activity = env->FindClass("com/usb/ProConDisconnectActivity");
        JNINativeMethod pmethods[] {
            {"device_attached", "(IILjava/lang/String;)V", reinterpret_cast<void*>(device_attached_callback)},
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
    void device_attached(int vid, int pid, const QString &device_name);

public slots:


};

static void device_attached_callback(JNIEnv *env, jobject obj, jint vid, jint pid, jstring device_name) {
    (void)obj;
    jboolean is_copy = false;
    QString str(env->GetStringUTFChars(device_name, &is_copy));
    emit callback_manager::instance()->device_attached(vid, pid, str);
}


#endif // CALLBACKS_HPP
