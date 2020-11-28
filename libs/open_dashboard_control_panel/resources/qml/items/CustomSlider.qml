// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

Item {

	id: customSlider

	property alias name: label.text
	property double min: 0
	property double max: 10
	property double tick: 1
	property double value: 0

	RowLayout {

		anchors.fill: parent

		spacing: 0

		Label {
			id: label
			color: "white"

			horizontalAlignment: "AlignHCenter"

			Layout.fillWidth: true
			Layout.minimumWidth: parent.width * 0.1
			Layout.preferredWidth: parent.width * 0.1
			Layout.maximumWidth: parent.width * 0.1
		}

		Slider {
			id: slider

			from: min
			to: max
			stepSize: tick

			Layout.fillWidth: true

			onValueChanged: customSlider.value = value
		}

		SpinBox {
			id: spinBox

			from: min
			to: max
			stepSize: tick

			Layout.fillWidth: true
			Layout.minimumWidth: implicitWidth
			Layout.preferredWidth: implicitWidth
			Layout.maximumWidth: implicitWidth

			onValueChanged: customSlider.value = value
		}
	}

	onValueChanged: {
		slider.value = value
		spinBox.value = value
	}
}
