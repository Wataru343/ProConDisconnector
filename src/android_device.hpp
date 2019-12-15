#ifndef ANDROID_DEVICE_HPP
#define ANDROID_DEVICE_HPP

#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>

namespace android_device {
void vibrate();

template <class T, class U>
void write_command(const T &jstr, const U &command) {
    QAndroidJniEnvironment env;

    jbyteArray arr = env->NewByteArray(std::size(command));
    jboolean is_copy = true;
    jbyte *buf = env->GetByteArrayElements(arr, &is_copy);

    std::copy(std::begin(command), std::end(command), buf);
    env->SetByteArrayRegion(arr, 0, std::size(command), buf);

    QtAndroid::androidActivity().callMethod<void>("write", "(Ljava/lang/String;[B)V", jstr.template object<jstring>(), arr);
    env->ReleaseByteArrayElements(arr, buf, 0);
}

template <class T>
auto read_data(const T &jstr) {
    QAndroidJniEnvironment env;

    std::vector<uint8_t> dst(64, 0);

    jbyteArray arr = env->NewByteArray(static_cast<jsize>(std::size(dst)));
    QtAndroid::androidActivity().callMethod<void>("read", "(Ljava/lang/String;[B)V", jstr.template object<jstring>(), arr);


    jboolean is_copy = false;
    jbyte *buf = env->GetByteArrayElements(arr, &is_copy);

    std::copy(buf, buf + std::size(dst), std::begin(dst));
    env->ReleaseByteArrayElements(arr, buf, 0);

    return dst;
}
}

#endif // ANDROID_DEVICE_HPP
