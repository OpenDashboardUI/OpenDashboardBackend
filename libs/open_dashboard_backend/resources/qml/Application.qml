// Copyright (C) 2020 twyleg
import QtQuick 2.15
import QtQuick.Window 2.1
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.12

import "./ControlBars"
import "./SettingsWindow"

ApplicationWindow {
	id: applicationWindow

	visible: true
	color: frontendLoader.color
	title: qsTr("OpenDashboard - Frontend")

	width: 1920
	height: 1080

	Material.theme: Material.Dark
	Material.accent: Material.White
	Material.foreground: Material.White

	FrontendLoader {
		id: frontendLoader
		onClicked: openFileDialog()

		onLoaded: {
			settingsDialog.setBackgroundColor(color)
		}
	}

	MouseArea {
		id: scrollArea
		anchors.fill: parent
		acceptedButtons: Qt.MiddleButton

		onWheel: {
			if (wheel.angleDelta.y > 0) {
				frontendLoader.zoomIn()
			} else {
				frontendLoader.zoomOut()
			}
		}
	}

	ControlBarTop {
		id: controlBarTop
		color: settingsDialog.controlBarBackgroundColor

		onOpenButtonClicked: frontendLoader.openFileDialog()
		onReloadButtonClicked: backend.handleReloadRequest()
		onZoomInButtonClicked: frontendLoader.zoomIn()
		onZoomOutButtonClicked: frontendLoader.zoomOut()
		onSettingsButtonClicked: settingsDialog.visible = true
	}

	ControlBarBottom {
		id: controlBarBottom
		visible: dataModel.controlDataStatic.controlBarBottomEnabled
		color: settingsDialog.controlBarBackgroundColor
	}

	ControlBarLeft {
		id: controlBarLeft
		visible: dataModel.controlDataStatic.controlBarLeftEnabled
		color: settingsDialog.controlBarBackgroundColor
	}

	ControlBarRight {
		id: controlBarRight
		visible: dataModel.controlDataStatic.controlBarRightEnabled
		color: settingsDialog.controlBarBackgroundColor
	}

	SettingsWindow {
		id: settingsDialog

		visible: false

		width: parent.width/2
		height: parent.height/2

		x: (parent.width - width)/2
		y: (parent.height - height)/2

		backgroundColor: frontendLoader.color

		onBackgroundColorChanged: {
			frontendLoader.color = backgroundColor
		}

		onSettingsChanged: controlBarRight.reload()
	}

}
