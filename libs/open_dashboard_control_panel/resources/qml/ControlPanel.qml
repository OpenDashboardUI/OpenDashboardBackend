// Copyright (C) 2020 twyleg
import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15

import "./items"

ApplicationWindow {
	id: controlPanelWindow

	title: qsTr("OpenDashboard - ControlPanel")
	visible: true

	minimumHeight: 800
	minimumWidth: 800

	onWidthChanged: adjustBackground()
	onHeightChanged: adjustBackground()
	Component.onCompleted: adjustBackground()

	property real contentOpacity: 0.8

	Image {
		id: backgroundImage

		source: "qrc:/control_panel/images/external/background_1.jpg"

		fillMode: Image.PreserveAspectFit

		anchors.bottom: parent.bottom
		anchors.right: parent.right
	}

	Item {
		id: headerItem

		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right

		height: Screen.height * 0.05

		Rectangle {
			id: headerItemBackground
			anchors.fill: parent

			color: "black"
			opacity: contentOpacity
		}

		Image {
			source: "qrc:/common_ui/images/svg/open_dashboard_logo_1.svg"

			anchors.centerIn: parent
			height: parent.height
			fillMode: Image.PreserveAspectFit

			sourceSize.width: width
			sourceSize.height: height
		}
	}

	Item {
		id: contentItem

		anchors.top: headerItem.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: footerItem.top

		Rectangle {

			anchors.horizontalCenter: parent.horizontalCenter
			anchors.top: parent.top
			anchors.topMargin: 20

			width: parent.width * 0.9
			height: parent.height * 0.7

			color: "black"
			opacity: contentOpacity

			radius: 10

			ColumnLayout {

				anchors.top: parent.top
				anchors.horizontalCenter: parent.horizontalCenter
				width: parent.width * 0.95

				spacing: 1

				Text {
					text: "Dynamics"
					color: "white"
					horizontalAlignment: Qt.AlignHCenter
					Layout.preferredWidth: parent.width
				}

				CustomSlider {
					name: "Velocity"
					Layout.preferredWidth: parent.width
					Layout.preferredHeight: 40

					min: 0
					max: 400
					tick: 1.0

					onValueChanged: dataModel.vehicleData.dynamics.velocity = value
				}

				Text {
					text: "Powertrain"
					color: "white"
					horizontalAlignment: Qt.AlignHCenter
					Layout.preferredWidth: parent.width
				}

				CustomSlider {
					name: "RPM"
					Layout.preferredWidth: parent.width
					Layout.preferredHeight: 40

					min: 0
					max: 10000
					tick: 10

					onValueChanged: dataModel.vehicleData.powertrain.engineRotation = value
				}

				CustomSlider {
					name: "Gear"
					Layout.preferredWidth: parent.width
					Layout.preferredHeight: 40

					min: -1
					max: 7
					tick: 1.0

					onValueChanged: dataModel.vehicleData.powertrain.gearboxGear = value
				}

				RowLayout {
					id: rowLayout

					Layout.preferredWidth: parent.width
					Layout.preferredHeight: 400

					AccelerationItem {
						id: accelerationItem

						Layout.preferredWidth: parent.width/3
						Layout.preferredHeight: 250

						onAxChanged: dataModel.vehicleData.dynamics.ax = ax * 9.81
						onAyChanged: dataModel.vehicleData.dynamics.ay = ay * 9.81
					}

					PedalsItem {
						id: pedalsItem

						Layout.preferredWidth: parent.width/3
						Layout.preferredHeight: 250

						onThrottleChanged: dataModel.vehicleData.driverInput.throttle = throttle
						onBrakeChanged: dataModel.vehicleData.driverInput.brake = brake
						onClutchChanged: dataModel.vehicleData.driverInput.clutch = clutch
					}

					SteeringWheelItem {
						id: steeringWheelItem

						Layout.preferredWidth: parent.width/3
						Layout.preferredHeight: 250

						onAngleChanged: dataModel.vehicleData.driverInput.steeringWheelAngle = angle
					}
				}
			}
		}
	}

	Item {
		id: footerItem

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		height: Screen.height * 0.025

		Rectangle {
			id: footerItemBackground
			anchors.fill: parent

			color: "black"
			opacity: contentOpacity
		}

	}

	function adjustBackground() {

		var windowRatio = width / height
		var backgroundRatio = backgroundImage.sourceSize.width / backgroundImage.sourceSize.height

		if (windowRatio > backgroundRatio) {
			backgroundImage.width = controlPanelWindow.width
			backgroundImage.height = controlPanelWindow.width / backgroundRatio
		} else {
			backgroundImage.width = controlPanelWindow.height * backgroundRatio
			backgroundImage.height = controlPanelWindow.height
		}
	}

}
