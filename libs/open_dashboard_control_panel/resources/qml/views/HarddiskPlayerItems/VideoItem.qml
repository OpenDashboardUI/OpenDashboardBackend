// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.15

ColumnLayout {
	id: videoItem

	RowLayout {

		Layout.fillWidth: true

		Label {
			text: "File:"
			font.pointSize: 12
			verticalAlignment: Qt.AlignVCenter
		}

		ComboBox {
			id: comboBox

			Layout.fillWidth: true
			editable: false
			model: ListModel {
				id: model
			}

			onCurrentIndexChanged: video.source = harddiskPlayerController.videoStreams[currentIndex]
		}
	}

	Video {
		id: video

		property real ratio: metaData.resolution ? metaData.resolution.height / metaData.resolution.width : 16.0/9.0

		autoPlay: false
		width: 400
		height: width * ratio

		onStatusChanged: {
			if(status === MediaPlayer.Loaded){
				console.log("loaded")
				video.seek(harddiskPlayerController.recordCurrentTimestamp * 1000)
				video.play()
				if(!harddiskPlayerController.running){
					video.pause()
				}
			}
		}

		Connections {
			target: harddiskPlayerController
			function onRunningChanged() {
				if(harddiskPlayerController.running){
					video.play()
				}else{
					video.pause()
				}
			}

			function onJumped() {
				video.seek(harddiskPlayerController.recordCurrentTimestamp * 1000)
			}

			function basename(str) {
				return (str.slice(str.lastIndexOf("/")+1))
			}

			function onVideoStreamsChanged() {
				model.clear()
				for(var i=0; i<harddiskPlayerController.videoStreams.length; i++){
					model.append({ "text": basename(harddiskPlayerController.videoStreams[i].toString()) })
				}
				comboBox.currentIndex = 0
			}
		}
	}



}
