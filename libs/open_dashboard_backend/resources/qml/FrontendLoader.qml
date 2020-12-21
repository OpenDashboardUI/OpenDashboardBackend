// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Dialogs 1.0
import Qt.labs.settings 1.1

Loader {
	id: frontendLoader

	focus: true
	enabled: true
	Keys.forwardTo: [item]
	anchors.centerIn: parent

	property int nativeWidth: 0
	property int nativeHeight: 0
	property real zoom: 1.0
	property color color: "transparent"

	signal clicked()

	FileDialog {
		id: fileDialog
		title: "Please choose a frontend config file"
		folder: shortcuts.home
		nameFilters: [ "Frontend config file (*.xml)" ]
		visible: false
		onAccepted: {
			backend.handleOpenFileRequest(fileDialog.fileUrl)
			visible = false
		}
		onRejected: {
			visible = false
		}
	}

	Connections {
		target: backend

		function onUnloadFrontendRequest() {
			frontendLoader.unloadFrontend()
		}

		function onLoadFrontendRequest() {
			frontendLoader.loadFrontend()
		}
	}

	Settings {
		id: settings

		property alias lastFile: fileDialog.folder
	}

	onLoaded: {
		nativeWidth = item.width
		nativeHeight = item.height

		if (item.color !== undefined)
			color = item.color

		// This is only relevant for the LogoStartscreen to open the file open dialog on click
		if (item.clicked !== undefined)
			item.clicked.connect(clicked)
	}

	onZoomChanged: {
		item.width = nativeWidth * zoom
		item.height = nativeHeight * zoom
	}

	onColorChanged: {
		if (item !== null && item.color !== undefined)
			item.color = color
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
