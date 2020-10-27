// Copyright (C) 2020 twyleg
#pragma once

#include <QMetaProperty>
#include <QObject>

#define AddAttribute(datatype, name, funcName, initial_value) \
private: \
	datatype name = initial_value; \
	Q_PROPERTY(datatype name READ Get##funcName WRITE Set##funcName NOTIFY  funcName##Changed) \
public: \
	datatype Get##funcName() const { return name; } \
	void Set##funcName(const datatype value) \
	{ \
		name = value; \
		emit funcName##Changed(); \
	} \
Q_SIGNALS: \
	void funcName##Changed(); \
private: \


class VehicleDataModel : public QObject
{
	Q_OBJECT

	AddAttribute(double, vehicleVelocity, VehicleVelocity, 0.0);
	AddAttribute(double, vehicleAy, VehicleAy, 0.0);
	AddAttribute(double, vehicleAx, VehicleAx, 0.0);
	AddAttribute(double, vehicleAz, VehicleAz, 0.0);

	AddAttribute(double, inputThrottle, InputThrottle, 0.0);
	AddAttribute(double, inputBrake, InputBrake, 0.0);
	AddAttribute(double, inputSteeringWheelAngle, InputSteeringWheelAngle, 0.0);

	AddAttribute(double, engineRpm, EngineRpm, 0.0);
	AddAttribute(double, enginePowerOut, EnginePowerOut, 0.0);
	AddAttribute(double, engineTorque, EngineTorque, 0.0);
	AddAttribute(double, engineFuelConsumptionAbs, EngineFuelConsumptionAbs, 0.0);
	AddAttribute(double, engineFuelConsumptionAct, EngineFuelConsumptionAct, 0.0);
	AddAttribute(double, engineFuelConsumptionAvg, EngineFuelConsumptionAvg, 0.0);
	AddAttribute(double, engineFuelLevel, EngineFuelLevel, 0.0);

	AddAttribute(int, gearboxGear, GearboxGear, 0);
	AddAttribute(double, gearboxPowerIn, GearboxPowerIn, 0.0);
	AddAttribute(double, gearboxPowerOut, GearboxPowerOut, 0.0);
	AddAttribute(double, gearboxRotationIn, GearboxRotationIn, 0.0);
	AddAttribute(double, gearboxRotationOut, GearboxRotationOut, 0.0);

	AddAttribute(double, positionLon, PositionLon, 0.0);
	AddAttribute(double, positionLat, PositionLat, 0.0);
};

class ControlDataStaticModel : public QObject
{
	Q_OBJECT

	AddAttribute(bool, sidebarsDisabled, SidebarsDisabled, true);

	AddAttribute(QString, videoChannelOnePath, VideoChannelOnePath, "");
	AddAttribute(QString, videoChannelTwoPath, VideoChannelTwoPath, "");
	AddAttribute(QString, videoChannelThreePath, VideoChannelThreePath, "");
};

class ControlDataDynamicModel : public QObject
{
	Q_OBJECT

public:
	enum class State {
		STOPPED = 0,
		STARTED = 1,
		RESET = 2
	};
	Q_ENUM(State)

	AddAttribute(State, state, State, State::STOPPED);
	AddAttribute(double, lon, Lon, 6.9491048);
	AddAttribute(double, lat, Lat, 50.338412);
	AddAttribute(QString, serializedData, SerializedData, "");
};
