#ifndef HELPER_H
#define HELPER_H

namespace helper {
struct Time {
    Time(int value) {
        sec = value / 1000;
        min = sec / 60;
        if (sec >= 60) {
            sec -= min * 60;
        }
    }
    int min = 0;
    int sec = 0;
};
}

#endif // HELPER_H
