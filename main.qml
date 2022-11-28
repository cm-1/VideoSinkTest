import QtQuick
import QtMultimedia

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Video Sink Test")


    MediaPlayer {
        id: mediaplayer
        source:"qrc:/videoFiles/QuickRender.mp4"
        audioOutput: AudioOutput {}
        videoOutput: videoOutput
    }


    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        onPressed: mediaplayer.play()
    }

}
