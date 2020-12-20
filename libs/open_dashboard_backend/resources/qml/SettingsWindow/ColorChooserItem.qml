// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

RowLayout {
	id: colorChooserItem

	property string title
	property color color
	property color defaultColor
	property font font

	width: parent.width
	height: font.pixelSize * 1.5
	spacing: 10

	Label {
		Layout.fillWidth: true

		height: parent.height
		font: colorChooserItem.font
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter

		text: title
	}

	Item {
		width: colorTextEdit.implicitWidth * 1.1
		height: parent.height

		Rectangle {

			width: parent.width
			height: parent.height * 0.95

			color: "black"
			border.color: "white"
			border.width: 1

			TextEdit {
				id: colorTextEdit

				color: "white"
				text: colorChooserItem.color
				enabled: false

				anchors.fill: parent
				verticalAlignment: Text.AlignVCenter
				font: colorChooserItem.font
			}
		}
	}

	Button {
		height: parent.height
		text: "Color picker"
		font: colorChooserItem.font
		onClicked: colorDialog.open()
		focusPolicy: Qt.NoFocus
		icon.source: "qrc:/svg-multilayer-extracted/icons_color.svg"
		icon.color: "transparent"
	}

	Button {
		height: parent.height
		text: "Default"
		font: colorChooserItem.font
		onClicked: colorChooserItem.color = defaultColor
		focusPolicy: Qt.NoFocus
	}

	ColorDialog {
		id: colorDialog

		title: "Please choose a color"
		visible: false
		color: defaultColor

		onAccepted: {
			close()
			colorChooserItem.color = color
		}
		onRejected: {
			close()
			colorChooserItem.color = color
		}

		onVisibleChanged: {
			if(visible)
				color = colorChooserItem.color
		}

		onCurrentColorChanged: {
			colorChooserItem.color = currentColor
		}
	}
}
