// Copyright (C) 2020 twyleg
#include "control_panel.h"

#include <open_dashboard_common/packet.h>

#include <open_dashboard.pb.h>

#include <iostream>

namespace OpenDashboard::ControlPanel {

#define CONNECT_HANDLER(qClass, qObj, qProperty, protoClass, protoProperty) \
	connect(qObj, &qClass::qProperty##Changed, [qObj, this] { \
		protoClass protoObj; \
		protoObj.set_##protoProperty(qObj->Get##qProperty()); \
		Send(protoObj); \
	}); \

ControlPanel::ControlPanel(int argc, char* argv[], QObject* parent)
:	QObject(parent),
	mArgc(argc),
	mArgv(argv),
	mApplication(mArgc, mArgv),
	mUdpTransmitter("127.0.0.1", 50000)
{
}

template<class T>
void ControlPanel::Send(const T& t)
{
	OpenDashboard::Common::OutboundPacket packet;
	packet.AddMessage(t.msg_type(), t);
	mUdpTransmitter.Transmit(packet.GetData(), packet.GetSize());
}

int ControlPanel::Run()
{
	QQmlApplicationEngine engine;
	engine.addImportPath("qrc:/");
	engine.rootContext()->setContextProperty("dataModel", &mDataModel);
	engine.load(QUrl(QStringLiteral("qrc:/control_panel/qml/ControlPanel.qml")));

	DataModels::DriverInput* driverInput = mDataModel.GetVehicleData()->GetDriverInput();
	CONNECT_HANDLER(DataModels::DriverInput, driverInput, Throttle, Proto::DriverInput, throttle);
	CONNECT_HANDLER(DataModels::DriverInput, driverInput, Brake, Proto::DriverInput, brake);
	CONNECT_HANDLER(DataModels::DriverInput, driverInput, Clutch, Proto::DriverInput, clutch);
	CONNECT_HANDLER(DataModels::DriverInput, driverInput, SteeringWheelAngle, Proto::DriverInput, steering_wheel_angle);

	DataModels::Dynamics* dynamics = mDataModel.GetVehicleData()->GetDynamics();
	CONNECT_HANDLER(DataModels::Dynamics, dynamics, Ax, Proto::Dynamics, ax);
	CONNECT_HANDLER(DataModels::Dynamics, dynamics, Ay, Proto::Dynamics, ay);
	CONNECT_HANDLER(DataModels::Dynamics, dynamics, Az, Proto::Dynamics, az);
	CONNECT_HANDLER(DataModels::Dynamics, dynamics, Velocity, Proto::Dynamics, velocity);

	DataModels::Powertrain* powertrain = mDataModel.GetVehicleData()->GetPowertrain();
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, EngineRotation, Proto::Powertrain, engine_rotation);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, EnginePowerOut, Proto::Powertrain, engine_power_out);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, EngineTorque, Proto::Powertrain, engine_torque);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, EngineFuelConsumptionAbs, Proto::Powertrain, engine_fuel_consumption_abs);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, EngineFuelConsumptionAct, Proto::Powertrain, engine_fuel_consumption_act);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, EngineFuelConsumptionAvg, Proto::Powertrain, engine_fuel_consumption_avg);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, EngineFuelLevel, Proto::Powertrain, engine_fuel_level);

	CONNECT_HANDLER(DataModels::Powertrain, powertrain, GearboxGear, Proto::Powertrain, gearbox_gear);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, GearboxPowerIn, Proto::Powertrain, gearbox_power_in);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, GearboxPowerOut, Proto::Powertrain, gearbox_power_out);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, GearboxRotationIn, Proto::Powertrain, gearbox_rotation_in);
	CONNECT_HANDLER(DataModels::Powertrain, powertrain, GearboxRotationOut, Proto::Powertrain, gearbox_rotation_out);

	return mApplication.exec();
}

}
