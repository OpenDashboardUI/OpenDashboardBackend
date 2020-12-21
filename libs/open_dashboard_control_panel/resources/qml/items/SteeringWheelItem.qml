// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
	id: steeringWheelItem

	property double min: -360 * 2.5
	property double max: 360 * 2.5
	property double tick: 1
	property double angle: 0

	property bool active: false

	Image {
		source: "qrc:/control_panel/images/external/steering_wheel.svg"

		fillMode: Image.PreserveAspectFit
		sourceSize: Qt.size(width, height)
		smooth: true

		rotation: slider.value
		anchors.top: parent.top
		anchors.bottom: slider.top
		anchors.horizontalCenter: parent.horizontalCenter

		width: parent.width
	}

	Slider {
		id: slider

		anchors.bottom: spinBox.top
		anchors.horizontalCenter: parent.horizontalCenter

		from: min
		to: max
		stepSize: tick
		value: 0

		onValueChanged: steeringWheelItem.angle = value
	}

	SpinBox {
		id: spinBox

		from: min
		to: max
		stepSize: tick

		anchors.bottom: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter

		onValueChanged: steeringWheelItem.angle = value
	}

	onAngleChanged: {
		slider.value = angle
		spinBox.value = angle
	}

}
