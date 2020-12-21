// Copyright (C) 2020 twyleg
import QtQuick 2.0

Rectangle {
	id: horizontallyScrollableTextEdit

	property alias text: textEdit.text

	color: "transparent"
	border.color: "white"
	border.width: 1

	Flickable {
		id: flick

		height: parent.height
		width: parent.width - 20
		contentWidth: textEdit.paintedWidth
		contentHeight: textEdit.paintedHeight
		clip: true
		boundsBehavior: Flickable.StopAtBounds

		function ensureVisible(r)
		{
			if (contentX >= r.x)
				contentX = r.x;
			else if (contentX+width <= r.x+r.width)
				contentX = r.x+r.width-width;
		}

		TextEdit {
			id: textEdit
			height: flick.height
			focus: true
			onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
			verticalAlignment: Qt.AlignVCenter
			font.pointSize: 12
			color: "white"
		}
	}
}
