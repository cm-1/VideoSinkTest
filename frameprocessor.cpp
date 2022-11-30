/*
The general idea/structure for this code, as well as a couple of small snippets,
come from two main sources:

 * [An answer](https://stackoverflow.com/a/69432938) by StackOverflow user [eyllanesc](https://stackoverflow.com/users/6622587/eyllanesc) on the post "How to use QVideoSink in QML in Qt6" by user [Igor Mironchik](https://stackoverflow.com/users/1624232/igor-mironchik).
 * A GitHub repo, [https://github.com/igormironchik/qt6_qcamera](https://github.com/igormironchik/qt6_qcamera), which seems to have been made by the asker of said question (since the usernames and the subject matter are both so similar).
*/
#include "frameprocessor.h"

FrameProcessor::FrameProcessor(QObject *parent): QMediaPlayer(parent),
    initialized(false), framesProcessed(0)
{
    // At the time of this constructor call, the QMediaPlayer doesn't have a 
    // QVideoSink yet. So I wait until the media is played for the first time
    // to do the remaining sink-dependent setup. Not the best way to handle
    // this, but the fastest/easiest for now with my limited Qt experience.
    connect( this, &QMediaPlayer::playbackStateChanged, this, &FrameProcessor::setupSinkStuff );
}

QVideoSink *FrameProcessor::outputVideoSink() const
{
    return m_outputVideoSink.get();
}

void FrameProcessor::setOutputVideoSink(QVideoSink *newVideoSink)
{
    if (m_outputVideoSink == newVideoSink)
        return;
    m_outputVideoSink = newVideoSink;
    emit outputVideoSinkChanged();
}


void FrameProcessor::setupSinkStuff(QMediaPlayer::PlaybackState newState)
{
    // initialized variable used to ensure we only connect this ever once.
    if (!initialized)
        connect( videoSink(), &QVideoSink::videoFrameChanged, this, &FrameProcessor::processFrame );
    initialized = true;
}

void FrameProcessor::processFrame(const QVideoFrame &frame)
{
    qDebug() << "Starting a frame!" << framesProcessed;
    if( m_outputVideoSink) {
        // This check is probably unnecessary for a fixed video file, but if I
        // change the code in the future, this might help me remember why
        // things might suddenly be broken then.
        if (frame.pixelFormat() == QVideoFrameFormat::Format_BGRA8888 || frame.pixelFormat() == QVideoFrameFormat::Format_RGBA8888) {
            // Because frame is const, we need to perform a shallow copy
            // before we can even do a ReadOnly mapping.
            QVideoFrame shallowCopy(frame);
            bool success = shallowCopy.map(QVideoFrame::ReadOnly);

            if (success) {
                // If we can successfully map the shallow copy, we can read its
                // contents, using them to construct our new frame "f".
                QVideoFrame f(frame.surfaceFormat());
                f.map(QVideoFrame::WriteOnly);
                uchar* origData = shallowCopy.bits(0);
                uchar* fData = f.bits(0);

                // Below is an intentionally inefficient processing to see how
                // this affects which frames I'm given.
                for (int i = 0; i < f.width() * f.height() * 4; i++) {
                    int maxCollatz = 0;
                    for (int j = origData[i]/5; j > 1; j--) {
                        int currentCollatz = collatz(j);
                        if (currentCollatz > maxCollatz && currentCollatz < 255) {
                            maxCollatz = currentCollatz;
                        }
                    }
                    // I want to leave alpha untouched but use the above to edit
                    // the other channels' values.
                    if (i % 4 != 3) {
                        fData[i] = maxCollatz;
                    }
                    else {
                        fData[i] = origData[i];
                    }
                }
                // I've encoded the frame number into one of the sample vids as
                // the length of a black bar. This can be used to tell if the
                // video is being fully processed.
                int barPixels = -1;
                int idx1D = frame.width() * (frame.height() - 1) * 4;
                int observedVal = 0;
                while (observedVal < 75 && barPixels < frame.width() - 2) {
                    observedVal = origData[idx1D] + origData[idx1D + 1] + origData[idx1D + 2];
                    fData[idx1D] = 255;
                    idx1D += 4;
                    barPixels++;
                }
                qDebug() << "barPixels:" << barPixels;
                // Now some cleanup.
                f.unmap();
                shallowCopy.unmap();

                // Send the new video frame to the respective QVideoSink.
                m_outputVideoSink->setVideoFrame( f );

                // The below code was used to ensure that every frame of the 
                // video was being processed, even if not every processed frame
                // was being displayed in the QML VideoOutput.
                //
                // f.toImage().save("./imageForFrame" + QString::number(framesProcessed) + ".jpg");

            }
        }
        else {
            qDebug() << "Format wasn't the expected BGRA8888 or RGBA8888 for frame" << framesProcessed;
            qDebug() << "Instead, format was:" << frame.pixelFormat();
        }
    }
    else {
        qDebug() << "No video sink output to send frame copy to for frame" << framesProcessed;
    }
    framesProcessed++;
}

// Returns the stopping time for int "input".
// https://en.wikipedia.org/wiki/Collatz_conjecture
int FrameProcessor::collatz(int input)
{
    int iterations = 0;
    int x = input;
    while (x > 1) {
        if (x % 2 == 0) {
            x = x/2;
        }
        else {
            x = 3*x + 1;
        }
        iterations++;
    }
    return iterations;
}





