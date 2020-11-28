import QtQuick 2.0

Item {
	id: accelerationItem

	property int size: width > height ? height : width
	property int radius: size * 0.5 * 0.9

	property real ax: 0
	property real ay: 0

	onAxChanged: dotCanvas.requestPaint()
	onAyChanged: dotCanvas.requestPaint()

	Canvas {
		id: bullsEyeCanvas

		anchors.centerIn: parent
		width: accelerationItem.size
		height: accelerationItem.size

		onWidthChanged: requestPaint()
		onHeightChanged: requestPaint()
		onPaint: drawBullsEye()

		function drawBullsEye() {

			var radius = accelerationItem.radius
			var centerX = width / 2;
			var centerY = height / 2;

			var ctx = getContext("2d");
			ctx.reset();
			ctx.beginPath();
			ctx.strokeStyle = "white";
			ctx.lineWidth = 1

			var dR = (radius * 0.9) / 5
			for(var i=0; i<=5; ++i) {
				var r = dR * i
				ctx.arc(centerX, centerY, r, 0, Math.PI * 2, true);
				ctx.stroke();
			}

			ctx.moveTo(centerX, centerY - radius);
			ctx.lineTo(centerX, centerY + radius);
			ctx.stroke();

			ctx.moveTo(centerX - radius, centerY);
			ctx.lineTo(centerX + radius, centerY);
			ctx.stroke();
		}
	}

	Canvas {
		id: dotCanvas

		anchors.centerIn: parent
		width: accelerationItem.size
		height: accelerationItem.size

		onWidthChanged: requestPaint()
		onHeightChanged: requestPaint()
		onPaint: drawDot()

		function drawDot() {

			var radius = accelerationItem.radius
			var centerX = width/2;
			var centerY = height/2;

			var ctx = getContext("2d");
			ctx.reset();

			ctx.beginPath();
			ctx.fillStyle = "red";
			var xPos = -(accelerationItem.ay / 5.0) * radius + centerX
			var yPos = -(accelerationItem.ax / 5.0) * radius + centerY
			ctx.arc(xPos, yPos, radius*0.05, 0, Math.PI * 2, true);
			ctx.fill();
		}

		Text {
			anchors.left: parent.left
			anchors.bottom: parent.bottom
			color: "white"
			text: "ay: " + accelerationItem.ax.toPrecision(2);
		}

		Text {
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			color: "white"
			text: "ax: " + accelerationItem.ay.toPrecision(2);
		}

		MouseArea {
			anchors.fill: parent
			onPositionChanged: {

				if (pressed) {
					var ay = -(mouseX - (width/2)) * (5.0 / accelerationItem.radius)
					var ax = -(mouseY - (height/2)) * (5.0 / accelerationItem.radius)

					accelerationItem.ax = Math.min(Math.max(ax, -5.0), 5.0)
					accelerationItem.ay = Math.min(Math.max(ay, -5.0), 5.0)

					accelerationItem.focus = true
				}
			}
		}
	}

	Keys.onUpPressed: accelerationItem.ax += 0.1
	Keys.onDownPressed: accelerationItem.ax -= 0.1

	Keys.onLeftPressed: accelerationItem.ay += 0.1
	Keys.onRightPressed: accelerationItem.ay -= 0.1
}
