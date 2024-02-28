#include "testing.h"

namespace testing {
    void testPlaySoundURL(QMediaPlayer* player) {
        player->setMedia(QUrl("https://muzux.net/uploads/music/2023/07/mirele-skin-kotikov-mp3.mp3"));
        // player->setVolume(50);
        // player->play();
        // qDebug() << player->media().request().priority();
    }
}

