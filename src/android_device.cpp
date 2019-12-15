#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>

#pragma GCC diagnostic ignored "-Wundefined-func-template"

namespace android_device {
void vibrate() {
    QAndroidJniEnvironment env;
    QtAndroid::androidActivity().callMethod<void>("vibrate", "()V");
}
}

#pragma GCC diagnostic warning "-Wundefined-func-template"
