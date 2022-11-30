# VideoSinkTest
Testing if Qt's QVideoSink can be used to edit every frame in a video.

The answer: yes.

In this repo, I have an extended QMediaPlayer's video sink
call a frame-processing slot on each new frame (via the `videoFrameChanged` signal)
that edits the frame and sends it to a different QVideoSink.

Even if processing takes a long time, every frame of video will be processed,
in order. They do not all _display_ in the final QML VideoOutput, unfortunately;
especially on Android, where _none_ of the frames except the first and last display.
Still, one can show, by saving the resulting frames to disk and via `qDebug()` statements,
that each one is still being processed.

## Credits
The general idea/structure for this code, as well as a couple of small snippets,
come from two main sources:

 * [An answer](https://stackoverflow.com/a/69432938) by StackOverflow user [eyllanesc](https://stackoverflow.com/users/6622587/eyllanesc) on the post "How to use QVideoSink in QML in Qt6" by user [Igor Mironchik](https://stackoverflow.com/users/1624232/igor-mironchik).
 * A GitHub repo, [https://github.com/igormironchik/qt6_qcamera](https://github.com/igormironchik/qt6_qcamera), which seems to have been made by the asker of said question (since the usernames and the subject matter are both so similar).
 