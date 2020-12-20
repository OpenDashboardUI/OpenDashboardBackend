// Copyright (C) 2020 twyleg
import QtQuick 2.0

Item {
	id: fpsItem

	property int fps: 0
	property int frames: 0
	property real counter: 0
	signal updated()

	onCounterChanged: frames++

	PropertyAnimation on counter {
		id: animation

		from: 0
		to: 100
		duration: 1000
		running: true

		onStopped: {
			fps = frames
			frames = 0
			updated()
			start()
		}
	}
}
