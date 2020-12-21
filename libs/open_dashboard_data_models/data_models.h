// Copyright (C) 2020 twyleg
#pragma once

#include "helper.h"

#include <open_dashboard.pb.h>

#include <QMetaProperty>
#include <QObject>
#include <QUrl>

#include <google/protobuf/message.h>

namespace OpenDashboard::DataModels {

class Gps: public SerializableQObject
{
	Q_OBJECT

	ADD_PROPERTY(double, positionLon, PositionLon, 0.0);
	ADD_PROPERTY(double, positionLat, PositionLat, 0.0);

public:

	explicit Gps(QObject* parent = 0):
		SerializableQObject(parent)
	{}

	void FromProto(const Proto::Gps&);
	void ToProto(Proto::Gps&);
};

class DriverInput: public SerializableQObject
{
	Q_OBJECT

	ADD_PROPERTY(double, throttle, Throttle, 0.0);
	ADD_PROPERTY(double, brake, Brake, 0.0);
	ADD_PROPERTY(double, clutch, Clutch, 0.0);
	ADD_PROPERTY(double, steeringWheelAngle, SteeringWheelAngle, 0.0);

public:

	explicit DriverInput(QObject* parent = 0):
		SerializableQObject(parent)
	{}

	void FromProto(const Proto::DriverInput&);
	void ToProto(Proto::DriverInput&);
};

class Dynamics: public SerializableQObject
{
	Q_OBJECT

	ADD_PROPERTY(double, ax, Ax, 0.0);
	ADD_PROPERTY(double, ay, Ay, 0.0);
	ADD_PROPERTY(double, az, Az, 0.0);

	ADD_PROPERTY(double, velocity, Velocity, 0.0);

public:

	explicit Dynamics(QObject* parent = 0):
		SerializableQObject(parent)
	{}

	void FromProto(const Proto::Dynamics&);
	void ToProto(Proto::Dynamics&);
};

class Powertrain: public SerializableQObject
{
	Q_OBJECT

	ADD_PROPERTY(double, engineRotation, EngineRotation, 0.0);
	ADD_PROPERTY(double, enginePowerOut, EnginePowerOut, 0.0);
	ADD_PROPERTY(double, engineTorque, EngineTorque, 0.0);
	ADD_PROPERTY(double, engineFuelConsumptionAbs, EngineFuelConsumptionAbs, 0.0);
	ADD_PROPERTY(double, engineFuelConsumptionAct, EngineFuelConsumptionAct, 0.0);
	ADD_PROPERTY(double, engineFuelConsumptionAvg, EngineFuelConsumptionAvg, 0.0);
	ADD_PROPERTY(double, engineFuelLevel, EngineFuelLevel, 0.0);

	ADD_PROPERTY(int, gearboxGear, GearboxGear, 0);
	ADD_PROPERTY(double, gearboxPowerIn, GearboxPowerIn, 0.0);
	ADD_PROPERTY(double, gearboxPowerOut, GearboxPowerOut, 0.0);
	ADD_PROPERTY(double, gearboxRotationIn, GearboxRotationIn, 0.0);
	ADD_PROPERTY(double, gearboxRotationOut, GearboxRotationOut, 0.0);

public:

	explicit Powertrain(QObject* parent = 0):
		SerializableQObject(parent)
	{}

	void FromProto(const Proto::Powertrain&);
	void ToProto(Proto::Powertrain&);
};



class VehicleData: public QObject
{
	Q_OBJECT

	ADD_PROPERTY_PTR(Gps, gps, Gps);
	ADD_PROPERTY_PTR(DriverInput, driverInput, DriverInput);
	ADD_PROPERTY_PTR(Dynamics, dynamics, Dynamics);
	ADD_PROPERTY_PTR(Powertrain, powertrain, Powertrain);

public:

	VehicleData(QObject* parent = 0) :
		QObject(parent)
	{}

	void SerializeQuantities(QVector<QString>&);

};

class ControlDataStatic: public SerializableQObject
{
	Q_OBJECT

	ADD_PROPERTY(bool, sidebarsDisabled, SidebarsDisabled, true);
	ADD_PROPERTY(bool, controlBarLeftEnabled, ControlBarLeftEnabled, true);
	ADD_PROPERTY(bool, controlBarRightEnabled, ControlBarRightEnabled, true);
	ADD_PROPERTY(bool, controlBarBottomEnabled, ControlBarBottomEnabled, true);
	ADD_PROPERTY(bool, webViewAvailable, WebViewAvailable, true);

	ADD_PROPERTY(QUrl, videoChannelOnePath, VideoChannelOnePath, QUrl());
	ADD_PROPERTY(QUrl, videoChannelTwoPath, VideoChannelTwoPath, QUrl());
	ADD_PROPERTY(QUrl, videoChannelThreePath, VideoChannelThreePath, QUrl());

	ADD_PROPERTY(QUrl, mapFilePath, MapFilePath, QUrl());

public:

	explicit ControlDataStatic(QObject* parent = 0):
		SerializableQObject(parent)
	{}
};

class ControlDataDynamic : public SerializableQObject
{
	Q_OBJECT

public:
	enum class State {
		STOPPED = 0,
		STARTED = 1,
		RESET = 2
	};
	Q_ENUM(State)

	ADD_PROPERTY(State, state, State, State::STOPPED);

public:

	explicit ControlDataDynamic(QObject* parent = 0):
		SerializableQObject(parent)
	{}
};


class DataModel : public QObject
{
	Q_OBJECT

	ADD_PROPERTY_PTR(VehicleData, vehicleData, VehicleData);
	ADD_PROPERTY_PTR(ControlDataStatic, controlDataStatic, ControlDataStatic);
	ADD_PROPERTY_PTR(ControlDataDynamic, controlDataDynamic, ControlDataDynamic);

	ADD_PROPERTY(QVector<QString>, serializedQuantities, SerializedQuantities, {});

public:

	DataModel(QObject* parent = 0) :
		QObject(parent)
	{}

	void SerializeQuantities();
};

}
