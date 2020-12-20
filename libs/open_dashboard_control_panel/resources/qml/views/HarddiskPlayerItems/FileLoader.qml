// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.12
import Qt.labs.settings 1.1

import "../../items"

ColumnLayout {
	id: fileLoader

	property alias filepath: fileLoaderTextEdit.text
	signal loadClicked()

	RowLayout {
		width: parent.width
		height: Screen.pixelDensity * 5

		Label {
			text: "Filepath:"
			font.pointSize: 12
		}

		HorizontallyScrollableTextEdit {
			id: fileLoaderTextEdit

			Layout.fillWidth: true
			height: parent.height

		}


//		Rectangle {
//			Layout.fillWidth: true
//			height: parent.height

//			color: "transparent"
//			border.color: "white"
//			border.width: 1

//			Flickable {
//				id: flick

//				height: parent.height
//				width: parent.width - 20
//				contentWidth: fileLoaderTextEdit.paintedWidth
//				contentHeight: fileLoaderTextEdit.paintedHeight
//				clip: true
//				boundsBehavior: Flickable.StopAtBounds

//				function ensureVisible(r)
//				{
//					if (contentX >= r.x)
//						contentX = r.x;
//					else if (contentX+width <= r.x+r.width)
//						contentX = r.x+r.width-width;
//				}

//				TextEdit {
//					id: fileLoaderTextEdit
//					height: flick.height
//					focus: true
//					onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
//					verticalAlignment: Qt.AlignVCenter
//					font.pointSize: 12
//					color: "white"
//				}
//			}
//		}

		Button {
			id: fileLoaderBrowseButton

			height: parent.height
			text: "Browse"

			onClicked: fileDialog.open()

			FileDialog {
				id: fileDialog
				title: "Please choose a file"
				folder: shortcuts.home
				onAccepted: {
					fileLoaderTextEdit.text = fileDialog.fileUrl.toString()
					fileDialog.visible = false
				}
				onRejected: {
					fileDialog.visible = false
				}
			}
		}
	}

	Button {
		text: "Load"
		height: Screen.pixelDensity * 5

		Layout.fillWidth: true

		onClicked: loadClicked()
	}

	Settings {
		id: settings
		category: "HarddiskPlayer"

		property alias filepath: fileLoaderTextEdit.text
	}

}
