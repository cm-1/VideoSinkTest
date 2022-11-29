#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <QMediaPlayer>
#include <QQmlEngine>
#include <QVideoFrame>
#include <QVideoSink>

class FrameProcessor : public QMediaPlayer
{
    // We extend QMediaPlayer to add in another video sink it sends processed
    // frames to. This will be set in main.qml.
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVideoSink* outputVideoSink READ outputVideoSink WRITE setOutputVideoSink NOTIFY outputVideoSinkChanged)

public:

    FrameProcessor(QObject *parent = nullptr);

    QVideoSink *outputVideoSink() const;
    void setOutputVideoSink(QVideoSink *newVideoSink);

signals:
    void outputVideoSinkChanged();


private slots:
    // QMediaPlayer doesn't have a QVideoSink at the time of construction.
    // So the remaining setup needs to be called later, via signal/slot communication.
    // Thus, sink-dependent setup happens in this funciton.
    void setupSinkStuff(QMediaPlayer::PlaybackState newState);

    // Processes frames. Called by the default QVideoFrame's videoFrameChanged
    // signal (but not our new, custom video sink's).
    void processFrame(const QVideoFrame& frame);

private:
    // Just need a function that can take a bit of time to call for each pixel.
    // A simple way of simulating long "real"/useful processing.
    int collatz(int input);

    // The video sink that will receive processed frames.
    QPointer<QVideoSink> m_outputVideoSink;

    // Whether setup has completely finished.
    bool initialized;

    // Frame counter. Just for debugging purposes right now.
    int framesProcessed;
};

#endif // FRAMEPROCESSOR_H
