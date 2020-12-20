// Copyright (C) 2020 twyleg
import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15

import QtQml.Models 2.2

import "../items"

Item {
	id: connectionView

	ScrollView {
		id: connectionScrollView

		anchors.fill: parent
		contentWidth: availableWidth
		anchors.horizontalCenter: parent.horizontalCenter

		ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

		ColumnLayout {

			width: connectionView.width
			height: parent.height
			anchors.centerIn: parent

			spacing: 10

			Text {
				text: "Connection"
				color: "white"
				horizontalAlignment: Qt.AlignHCenter
				Layout.fillWidth: true
				font.pointSize: 12
			}

			RowLayout {
				id: connectionRowLayout

				Layout.fillWidth: true
				height: Screen.pixelDensity * 5

				Text {
					text: "IP:"
					color: "white"
					font.pointSize: 12
				}

				HorizontallyScrollableTextEdit {
					id: hostnameTextEdit

					text: "127.0.0.1"
					Layout.fillWidth: true
					Layout.fillHeight: true
				}

				Text {
					text: "Port:"
					color: "white"
					font.pointSize: 12
				}

				SpinBox {
					id: portSpinBox

					from: 1
					to: 0xFFFF
					stepSize: 1
					value: 50000
				}
			}

			Button {
				id: connectButton

				text: "Connect"
				Layout.fillWidth: true

				onClicked: {
					connectionManager.initTransmitter(hostnameTextEdit.text, portSpinBox.value)
				}
			}

			MenuSeparator {
				Layout.fillWidth: true
				padding: 0
				contentItem: Rectangle {
					implicitHeight: 1
					width: parent.width
					height: 1
					color: "white"
				}
			}
		}
	}
}
