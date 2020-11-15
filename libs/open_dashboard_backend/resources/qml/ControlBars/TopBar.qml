// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Window 2.1

import "./TopBarItems"

ControlBar {

	id: topbar

	layout: ControlBar.Layout.TOP
	widthRelative: 0.5
	heightRelative: 0.1
	resizeable: false

	signal openButtonClicked()
	signal reloadButtonClicked()
	signal zoomInButtonClicked()
	signal zoomOutButtonClicked()

	Row {

		height: parent.height
		anchors.centerIn: parent
		spacing: 20

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/buttons_open.svg"
			onClicked: openButtonClicked()
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/buttons_reload.svg"
			onClicked: reloadButtonClicked()
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/buttons_fullscreen.svg"
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
			source: "qrc:/svg-multilayer-extracted/buttons_zoom_plus.svg"
			onClicked: zoomInButtonClicked()
		}

		SquareButton {
			size: parent.height * 0.8
			anchors.verticalCenter: parent.verticalCenter
			source: "qrc:/svg-multilayer-extracted/buttons_zoom_minus.svg"
			onClicked: zoomOutButtonClicked()
		}
	}
}
