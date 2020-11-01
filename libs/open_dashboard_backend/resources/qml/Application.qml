// Copyright (C) 2020 twyleg
import QtQuick 2.15
import QtQuick.Window 2.1

import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Dialogs 1.0

import QtMultimedia 5.15
import QtPositioning 5.5
import QtLocation 5.6

ApplicationWindow {
	id: applicationWindow

	visible: true
	color: "black"
	title: qsTr("OpenDashboard - Frontend")

	width: 1920
	height: 1080

	Loader {
		id: frontendLoader

		anchors.centerIn: parent

		property int nativeWidth: 0
		property int nativeHeight: 0
		property real zoom: 1.0

		Connections{
			target: backend

			function onUnloadFrontendRequest() {
				frontendLoader.unloadFrontend()
			}

			function onLoadFrontendRequest() {
				frontendLoader.loadFrontend()
			}
		}

		onLoaded: {
			nativeWidth = item.width
			nativeHeight = item.height

			if (item.color !== undefined)
				applicationWindow.color = item.color

			// This is only relevant for the LogoStartscreen to open the file open dialog on click
			if (item.clicked !== undefined)
				item.clicked.connect(openFileDialog)
		}

		onZoomChanged: {
			item.width = nativeWidth * zoom
			item.height = nativeHeight * zoom
		}

		function zoomIn() {
			zoom += 0.05
		}

		function zoomOut() {
			zoom -= 0.05
		}

		function loadFrontend() {
			frontendLoader.source = qApplication
		}

		function unloadFrontend() {
			frontendLoader.sourceComponent = undefined
		}

		function openFileDialog() {
			fileDialog.visible = true
		}
	}

	FileDialog {
		id: fileDialog
		title: "Please choose a QML file"
		folder: shortcuts.home
		nameFilters: [ "QML file (*.qml)" ]
		visible: false
		onAccepted: {
			backend.handleOpenFileRequest(fileDialog.fileUrl)
			visible = false
		}
		onRejected: {
			visible = false
		}
	}

	Item {
		id: extraData

		anchors.fill: parent
		enabled: !controlDataStaticModel.sidebarsDisabled
		visible: !controlDataStaticModel.sidebarsDisabled

		Topbar {
			id: topbar

			widthRelative: 0.5
			heightRelative: 0.1

			Row {

				height: parent.height
				anchors.centerIn: parent
				spacing: 20

				SquareButton {
					size: parent.height * 0.8
					anchors.verticalCenter: parent.verticalCenter
					source: "qrc:/svg-multilayer-extracted/buttons_open.svg"
					onClicked: frontendLoader.openFileDialog()
				}

				SquareButton {
					size: parent.height * 0.8
					anchors.verticalCenter: parent.verticalCenter
					source: "qrc:/svg-multilayer-extracted/buttons_reload.svg"
					onClicked: backend.handleReloadRequest()
				}

				SquareButton {
					size: parent.height * 0.8
					anchors.verticalCenter: parent.verticalCenter
					source: "qrc:/svg-multilayer-extracted/buttons_fullscreen.svg"
					onClicked: {
						if (applicationWindow.visibility == Window.Windowed){
							applicationWindow.visibility = Window.FullScreen
						} else {
							applicationWindow.visibility = Window.Windowed
						}
					}
				}

				SquareButton {
					size: parent.height * 0.8
					anchors.verticalCenter: parent.verticalCenter
					source: "qrc:/svg-multilayer-extracted/buttons_zoom_plus.svg"
					onClicked: frontendLoader.zoomIn()
				}

				SquareButton {
					size: parent.height * 0.8
					anchors.verticalCenter: parent.verticalCenter
					source: "qrc:/svg-multilayer-extracted/buttons_zoom_minus.svg"
					onClicked: frontendLoader.zoomOut()
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

				Text {
					id: serializedData
					color: "white"
					text: controlDataDynamicModel.serializedData
					width: parent.width
					height: parent.height
					wrapMode: Text.WrapAnywhere
					elide: Text.ElideRight
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
