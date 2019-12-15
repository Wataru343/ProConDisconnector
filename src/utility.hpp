#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <chrono>
#include <QtCore/QEventLoop>
#include <QtCore/QString>
#include <QtCore/QTimer>

template <class It>
inline QString to_hex(It begin, It end, int width) {
    QString dst;
    for(auto it = begin; it != end; ++it){
        dst += QString("%1").arg(*it, width, 16, QChar('0')).toUpper();
    }

    return dst;
}

template <class Rep, class Period>
inline void wait(const std::chrono::duration<Rep, Period> &time) {
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(time);
    loop.exec();
}


#endif // UTILITY_HPP
