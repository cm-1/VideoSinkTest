import QtQuick
import QtMultimedia
import FrameProcessorLib 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Video Sink Test")

    Text {
        anchors.left: parent.left
        anchors.top: parent.top
        text: "Click anywhere to start/restart vid!"
    }

    // Extends MediaPlayer to process frames and send
    // them to a new VideoSink.
    FrameProcessor {
        id: mediaplayer
        source:"qrc:/videoFiles/FrameNumRender.mp4"
        videoOutput: videoOutputLeft
        audioOutput: AudioOutput {}

        // Custom field representing new video sink to
        // send processed frames to.
        outputVideoSink: videoOutputRight.videoSink
    }

    // Left VideoOutput, for original video's frames.
    VideoOutput {
        id: videoOutputLeft
        width: parent.width / 2
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }

    // Right VideoOutput, for processed video frames.
    VideoOutput {
        id: videoOutputRight
        width: parent.width / 2
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }

    // Clicking anywhere in the window starts/restarts
    // video playback.
    MouseArea {
        anchors.fill: parent
        onPressed: mediaplayer.play()
    }

}
