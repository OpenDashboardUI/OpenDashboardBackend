// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Window 2.1
import Qt.labs.settings 1.1

import "./ControlBarTopItems"

ControlBar {

	id: controlBarTop

	layout: ControlBar.Layout.TOP
//	widthRelative: 0.5
	widthRelative: Math.max(0.5, (buttonRow.implicitWidth + 40) / parent.width)
	heightRelative: 0.1
	resizeable: false

	signal openButtonClicked()
	signal reloadButtonClicked()
	signal zoomInButtonClicked()
	signal zoomOutButtonClicked()
	signal settingsButtonClicked()

	Row {
		id: buttonRow

		height: parent.height
		anchors.centerIn: parent
		spacing: 20

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/icons_open.svg"
			onClicked: openButtonClicked()
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/icons_reload.svg"
			onClicked: reloadButtonClicked()
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/icons_settings.svg"
			onClicked: settingsButtonClicked()
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/icons_fullscreen.svg"
			onClicked: {
				if (applicationWindow.visibility === Window.Windowed){
					applicationWindow.visibility = Window.FullScreen
				} else {
					applicationWindow.visibility = Window.Windowed
				}
			}
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/icons_zoom_plus.svg"
			onClicked: zoomInButtonClicked()
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/icons_zoom_minus.svg"
			onClicked: zoomOutButtonClicked()
		}

		ControlBarSelector {
			id: controlBarSelector

			height: parent.height * 0.8
			width: height
			anchors.verticalCenter: parent.verticalCenter

			controlBarLeftEnabled: dataModel.controlDataStatic.controlBarLeftEnabled
			controlBarRightEnabled: dataModel.controlDataStatic.controlBarRightEnabled
			controlBarBottomEnabled: dataModel.controlDataStatic.controlBarBottomEnabled

			onControlBarLeftEnabledChanged: dataModel.controlDataStatic.controlBarLeftEnabled = controlBarLeftEnabled
			onControlBarRightEnabledChanged: dataModel.controlDataStatic.controlBarRightEnabled = controlBarRightEnabled
			onControlBarBottomEnabledChanged: dataModel.controlDataStatic.controlBarBottomEnabled = controlBarBottomEnabled
		}

		Settings {
			id: settings

			property alias controlBarLeftEnabled: controlBarSelector.controlBarLeftEnabled
			property alias controlBarRightEnabled: controlBarSelector.controlBarRightEnabled
			property alias controlBarBottomEnabled: controlBarSelector.controlBarBottomEnabled
		}

	}
}
