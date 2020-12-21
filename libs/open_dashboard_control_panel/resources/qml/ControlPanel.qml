// Copyright (C) 2020 twyleg
import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15

import QtQml.Models 2.2

import "./items"
import "./views"

ApplicationWindow {
	id: controlPanelWindow

	property real backgroundOpacity: 0.8

	Material.theme: Material.Dark
	Material.accent: Material.White
	Material.foreground: Material.White

	title: qsTr("OpenDashboard - ControlPanel")
	visible: true

	minimumHeight: 800
	minimumWidth: 800

	BackgroundImage {
		id: backgroundImage
	}

	HeaderItem {
		id: headerItem

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: Screen.height * 0.05

		backgroundOpactity: backgroundOpacity

		onBurgerMenuButtonClicked: customDrawer.open()
	}

	Item {
		id: centralItem

		anchors.top: headerItem.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: footerItem.top
		anchors.topMargin: 20
		anchors.bottomMargin: 20

		NavigationItem {
			id: navigationItemLeft

			direction: NavigationItem.Direction.LEFT
			backgroundOpacity: controlPanelWindow.backgroundOpacity

			width: parent.width * 0.025
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.bottom: parent.bottom

			onClicked: contentItem.moveLeft()
		}

		NavigationItem {
			id: navigationItemRight

			direction: NavigationItem.Direction.RIGHT
			backgroundOpacity: controlPanelWindow.backgroundOpacity

			width: parent.width * 0.025
			anchors.top: parent.top
			anchors.right: parent.right
			anchors.bottom: parent.bottom

			onClicked: contentItem.moveRight()
		}

		SplitView {
			id: contentItemSplitView

			orientation: Qt.Vertical

			anchors.top: parent.top
			anchors.left: navigationItemLeft.right
			anchors.right: navigationItemRight.left
			anchors.bottom: parent.bottom

			handleDelegate: Rectangle {
				color: "transparent"
				height: 20

				Rectangle {
					color: "black"
					width: parent.width
					height: 3
					anchors.centerIn: parent
				}
			}

			ContentItem {
				id: contentItem

				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.minimumHeight: parent.height * 0.15

				backgroundOpactity: backgroundOpacity

				Component.onCompleted: initView(0)

				items: [
					connectionViewComponent.createObject(),
					quantitiesViewComponent.createObject(),
					harddiskPlayerViewComponent.createObject(),
				]

				Component {
					id: connectionViewComponent
					ConnectionView {}
				}

				Component {
					id: quantitiesViewComponent
					QuantitiesView {}
				}

				Component {
					id: harddiskPlayerViewComponent
					HarddiskPlayerView {}
				}
			}

			LogItem {
				id: logItem

				Layout.fillWidth: true
				Layout.minimumHeight: parent.height * 0.15

				backgroundOpactity: backgroundOpacity
			}
		}
	}

	FooterItem {
		id: footerItem

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		height: Screen.height * 0.025

		backgroundOpactity: backgroundOpacity

		Text {
			id: footerText
			text: "Tx: total = " + connectionManager.transmitCount + " B, current = " + (connectionManager.transmitBps / 1000).toPrecision(3) + " kb/s"
			color: "white"
			font.pointSize: 10
			verticalAlignment: Qt.AlignVCenter
			anchors.fill: parent
		}
	}

	CustomDrawer {
		id: customDrawer

		width: parent.width/4
		height: parent.height

		model: ObjectModel {

			CustomDrawerElement {
				text: "Connection"
				onClicked: contentItem.changeView(0)
			}

			CustomDrawerElement {
				text: "Quantities Control"
				onClicked: contentItem.changeView(1)
			}

			CustomDrawerElement {
				text: "Harddisk Player"
				onClicked: contentItem.changeView(2)
			}
		}
	}
}
