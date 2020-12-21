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
	id: quantitiesView

	ScrollView {
		id: quantitiesScrollView

		anchors.fill: parent
		contentWidth: availableWidth
		anchors.horizontalCenter: parent.horizontalCenter

		ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
		Component.onCompleted: setInteractive(true)


		function setInteractive(state) {
			contentItem.interactive = state
		}

		ColumnLayout {

			width: quantitiesView.width
			height: parent.height
			anchors.centerIn: parent

			spacing: 10

			Text {
				text: "Dynamics"
				color: "white"
				horizontalAlignment: Qt.AlignHCenter
				Layout.preferredWidth: parent.width
				font.pointSize: 12
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
				font.pointSize: 12
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

			CustomSlider {
				name: "Gear"
				Layout.preferredWidth: parent.width
				Layout.preferredHeight: 40

				min: -1
				max: 7
				tick: 1.0

				onValueChanged: dataModel.vehicleData.powertrain.gearboxGear = value
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

			CustomSlider {
				name: "Gear"
				Layout.preferredWidth: parent.width
				Layout.preferredHeight: 40

				min: -1
				max: 7
				tick: 1.0

				onValueChanged: dataModel.vehicleData.powertrain.gearboxGear = value
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

				spacing: 0

				AccelerationItem {
					id: accelerationItem

					Layout.preferredWidth: parent.width/3
					Layout.preferredHeight: 250

					onAxChanged: dataModel.vehicleData.dynamics.ax = ax * 9.81
					onAyChanged: dataModel.vehicleData.dynamics.ay = ay * 9.81

					onActiveChanged: quantitiesScrollView.setInteractive(!active)
				}

				PedalsItem {
					id: pedalsItem

					Layout.preferredWidth: parent.width/3
					Layout.preferredHeight: 250

					onThrottleChanged: dataModel.vehicleData.driverInput.throttle = throttle
					onBrakeChanged: dataModel.vehicleData.driverInput.brake = brake
					onClutchChanged: dataModel.vehicleData.driverInput.clutch = clutch

					onActiveChanged: quantitiesScrollView.setInteractive(!active)
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
