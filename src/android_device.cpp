#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtCore/QDebug>
#pragma GCC diagnostic ignored "-Wundefined-func-template"

namespace android_device {
void vibrate() {
    QAndroidJniEnvironment env;
    QtAndroid::androidActivity().callMethod<void>("vibrate", "()V");
}

void search_device() {
    QAndroidJniEnvironment env;
    QtAndroid::androidActivity().callMethod<void>("search_device", "()V");
}

QString all_device() {
    QAndroidJniEnvironment env;
    QAndroidJniObject ret = QtAndroid::androidActivity().callObjectMethod("all_device", "()Ljava/lang/String;");

    return ret.toString();
}
}

#pragma GCC diagnostic warning "-Wundefined-func-template"
