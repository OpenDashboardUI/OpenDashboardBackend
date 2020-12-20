// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
import QtMultimedia 5.15

import "./HarddiskPlayerItems"

Item {
	id: harddiskPlayerView

	ScrollView {
		id: harddiskPlayerScrollView

		ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

		anchors.fill: parent
		anchors.horizontalCenter: parent.horizontalCenter
		contentWidth: availableWidth

		ColumnLayout {
			id: columnLayout

			width: harddiskPlayerView.width
			height: parent.height
			anchors.centerIn: parent

			spacing: 10

			Text {
				text: "Harddisk Player"
				color: "white"
				horizontalAlignment: Qt.AlignHCenter
				Layout.preferredWidth: parent.width
				font.pointSize: 12
			}

			FileLoader {
				id: fileLoader

				Layout.fillWidth: true

				onLoadClicked: {
					console.log(fileLoader.filepath)
					harddiskPlayerController.openFile(fileLoader.filepath)
					detailsItem.active = true
				}
			}

			MenuSeparator {
				Layout.fillWidth: true
				padding: 0
				contentItem: Rectangle {
					implicitHeight: 1
					width: parent.width
					height: 1
					color: "white"
				}
			}

			Item {
				id: detailsItem

				property bool active: false

				Layout.fillWidth: true
				height: detailsColumnLayout.height


				ColumnLayout {
					id: detailsColumnLayout

					width: parent.width
					anchors.centerIn: parent

					RecordDetails {
						id: recordDetails

						Layout.fillWidth: true
						Layout.preferredWidth: parent.width

						filename: harddiskPlayerController.recordFilename
						sampleRate: harddiskPlayerController.recordSampleRateMillis
						dataSource: harddiskPlayerController.recordDataSource
						trackName: harddiskPlayerController.recordTrackName
						vehicleName: harddiskPlayerController.recordVehiclekName
						comment: harddiskPlayerController.recordComment
					}

					PlayerSlider {
						id: playerSlider

						Layout.fillWidth: true

						currentFrame: harddiskPlayerController.recordCurrentFrame
						totalFrames: harddiskPlayerController.recordFramesCount

						currentTimestamp: harddiskPlayerController.recordCurrentTimestamp
						endTimestamp: harddiskPlayerController.recordEndTimestamp

						onFrameRequest: harddiskPlayerController.jump(requestedFrame)
						onSliderGrabbed: harddiskPlayerController.pause()
					}

					PlayerControl {
						id: playerControl

						Layout.fillWidth: true
						playing: harddiskPlayerController.running

						onFastBackwardClicked: harddiskPlayerController.seek(-100);
						onBackwardClicked: harddiskPlayerController.seek(-1);
						onForewardClicked: harddiskPlayerController.seek(1);
						onFastForewardClicked: harddiskPlayerController.seek(100);
						onPlayPauseClicked: {
							if(harddiskPlayerController.running) {
								harddiskPlayerController.pause()
							}else{
								harddiskPlayerController.play()
							}
						}
					}

					VideoItem {
						id: videoItem

//						width: parent.width /2
						Layout.fillWidth: true
					}

//					ComboBox {
//						id: comboBox
//						editable: true
//						model: ListModel {
//							id: model
//						}

//						onCurrentIndexChanged: video.source = harddiskPlayerController.videoStreams[currentIndex]
//					}

//					Video {
//						id: video

//						autoPlay: false
//						width: 400
////						height: metaData.resolution ? width * (metaData.resolution.height / metaData.resolution.width) : 0
//						height: 400

//						onStatusChanged: {
//							if(status === MediaPlayer.Loaded){
//								console.log("loaded")
//								video.seek(harddiskPlayerController.recordCurrentTimestamp * 1000)
//								video.play()
//								if(!harddiskPlayerController.running){
//									video.pause()
//								}
//							}
//						}
//					}


//					Connections {
//						target: harddiskPlayerController
//						function onRunningChanged() {
//							if(harddiskPlayerController.running){
//								video.play()
//							}else{
//								video.pause()
//							}
//						}

//						function onJumped() {
//							video.seek(harddiskPlayerController.recordCurrentTimestamp * 1000)
//						}


//						function basename(str) {
//							return (str.slice(str.lastIndexOf("/")+1))
//						}

//						function onVideoStreamsChanged() {
//							model.clear()
//							for(var i=0; i<harddiskPlayerController.videoStreams.length; i++){
//								model.append({ "text": basename(harddiskPlayerController.videoStreams[i].toString()) })
//							}
//							comboBox.currentIndex = 0
//						}
//					}
				}

				Rectangle {
					anchors.fill: parent
					color: "black"
					visible: !detailsItem.active
					enabled: visible
					opacity: 0.8

					MouseArea {
						anchors.fill: parent
					}
				}
			}

			Item {
				Layout.fillWidth: true
				Layout.fillHeight: true
			}

		}
	}
}
