// Copyright (C) 2020 twyleg
import QtQuick 2.15
import QtQuick.Window 2.1

import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

import QtMultimedia 5.15
import QtPositioning 5.5
import QtLocation 5.6

ApplicationWindow {
	id: root

	visible: true
	color: "black"
	title: qsTr("OpenDashboard - Frontend")

	width: 1920
	height: 1080

	Loader {
		id: myLoader

		anchors.fill: parent
		source: qApplication
	}

	Item {
		id: extraData

		anchors.fill: parent
		enabled: !controlDataStaticModel.sidebarsDisabled
		visible: !controlDataStaticModel.sidebarsDisabled

		FullscreenButton {
			id: fullscreenButton

			width: parent.height * 0.05
			height: parent.height * 0.05

			anchors.top: parent.top
			anchors.horizontalCenter: parent.horizontalCenter

			onClicked: {
				if (root.visibility == Window.Windowed){
					root.visibility = Window.FullScreen
				} else {
					root.visibility = Window.Windowed
				}
			}
		}

		Sidebar  {
			id: sidebarLeft

			layout: Sidebar.Layout.LEFT
			widthRelative: 0.09

			Column {

				width: parent.width * 0.9
				height: parent.height * 0.9
				anchors.centerIn: parent
				spacing: 20

				TextMetrics {
					id: metrics
					font.family: "Arial"
					elideWidth: 300
					elide: Text.ElideRight
					text: controlDataDynamicModel.serializedData
				}

				Text {
					id: serializedData
					color: "white"
					text: controlDataDynamicModel.serializedData
					width: parent.width
					height: parent.height
					wrapMode: Text.WrapAnywhere
				}
			}
		}

		Sidebar  {
			id: sidebarRight

			layout: Sidebar.Layout.RIGHT
			widthRelative: 0.15

			Column {

				width: parent.width * 0.9
				height: parent.height * 0.9
				anchors.centerIn: parent
				spacing: 20

				property int currentState: controlDataDynamicModel.state

				onCurrentStateChanged: {

					switch (currentState)
					{
					case 0:
					case 2:
						videoChannel0.seek(0);
						videoChannel0.stop();
						videoChannel1.seek(0);
						videoChannel1.stop();
						videoChannel2.seek(0);
						videoChannel2.stop();
						break;
					case 1:
						videoChannel0.play();
						videoChannel1.play();
						videoChannel2.play();
						break;
					}
				}


				VideoChannel {
					id: videoChannel0

					autoPlay: false
					width: parent.width
					source: "file:" + controlDataStaticModel.videoChannelOnePath
				}

				VideoChannel {
					id: videoChannel1

					autoPlay: false
					width: parent.width
					source: "file:" + controlDataStaticModel.videoChannelTwoPath
				}

				VideoChannel {
					id: videoChannel2

					autoPlay: false
					width: parent.width
					source: "file:" + controlDataStaticModel.videoChannelThreePath
				}

				MapView {
					id: mapView

					width: parent.width
					height: width
				}
			}
		}
	}

}
