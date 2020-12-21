// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0

EmbeddedWindow {
	id: settingsWindow

	title: "Settings"

	property color controlBarBackgroundColor: colorChooserControlBarBackground.color
	property alias backgroundColor: colorChooserBackground.color

	property font h1font: Qt.font({
		pointSize: 14,
		underline: true
	})

	property font h2font: Qt.font({
		pointSize: 12
	})

	signal settingsChanged()

	Column {
		spacing: h1font.pixelSize * 1.5

		anchors.topMargin: spacing
		anchors.leftMargin: spacing
		anchors.rightMargin: spacing
		anchors.fill: parent

		Label {
			text: "Google Maps"
			font: h1font
		}

		ApiKeyItem {
			id: apiKeyItem
			font: h2font

			onApply: {
				mapGenerator.generateMap(value)
				settingsChanged()
			}
		}

		Label {
			text: "Colors"
			font: h1font
		}

		ColorChooserItem {
			id: colorChooserBackground
			title: "Background:"
			font: h2font
		}

		ColorChooserItem {
			id: colorChooserControlBarBackground

			color: "#161618"
			defaultColor: "#161618"

			title: "Control-Bar Background:"
			font: h2font
		}
	}

	Settings {
		id: setings

		property alias controlBarBackgroundColor: settingsWindow.controlBarBackgroundColor
		property alias googleMapsApiKey: apiKeyItem.value
	}

	function setBackgroundColor(color) {
		colorChooserBackground.color = color
		colorChooserBackground.defaultColor = color
	}

}
