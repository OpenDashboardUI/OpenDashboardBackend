// Copyright (C) 2020 twyleg
import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

ColumnLayout {
	id: controlBarSelector

	property alias controlBarLeftEnabled: controlBarLeftCheckBox.checked
	property alias controlBarRightEnabled: controlBarRightCheckBox.checked
	property alias controlBarBottomEnabled: controlBarBottomCheckBox.checked

	CheckBox {
		id: controlBarLeftCheckBox
		checked: true

		style: CheckBoxStyle {
			label: Label {
				color: "white"
				text: "Left"
			}
		}
	}


	CheckBox {
		id: controlBarRightCheckBox
		checked: true

		style: CheckBoxStyle {
			label: Label {
				color: "white"
				text: "Right"
			}
		}
	}

	CheckBox {
		id: controlBarBottomCheckBox
		checked: true

		style: CheckBoxStyle {
			label: Label {
				color: "white"
				text: "Bottom"
			}
		}
	}
}
