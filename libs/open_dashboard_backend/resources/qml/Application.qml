// Copyright (C) 2020 twyleg
import QtQuick 2.15
import QtQuick.Window 2.1


import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4



import "./ControlBars"

ApplicationWindow {
	id: applicationWindow

	visible: true
	color: "black"
	title: qsTr("OpenDashboard - Frontend")

	width: 1920
	height: 1080

	FrontendLoader {
		id: frontendLoader

		onBackgroundColorChanged: applicationWindow.color = backgroundColor
		onClicked: openFileDialog()
	}

	TopBar {
		onOpenButtonClicked: frontendLoader.openFileDialog()
		onReloadButtonClicked: backend.handleReloadRequest()
		onZoomInButtonClicked: frontendLoader.zoomIn()
		onZoomOutButtonClicked: frontendLoader.zoomOut()
	}

	BottomBar {
	}

	LeftSideBar {
	}

	RightSideBar {
	}

}
