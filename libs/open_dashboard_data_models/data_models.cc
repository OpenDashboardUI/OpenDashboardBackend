// Copyright (C) 2020 twyleg
#include "data_models.h"

namespace OpenDashboard::DataModels {

void Dynamics::FromProto(const Proto::Dynamics& dynamics)
{
	UPDATE(dynamics, velocity, *this, Velocity);
	UPDATE(dynamics, ax, *this, Ax);
	UPDATE(dynamics, ay, *this, Ay);
	UPDATE(dynamics, az, *this, Az);
}

void Dynamics::ToProto(Proto::Dynamics& dynamics)
{
	dynamics.set_velocity(GetVelocity());
	dynamics.set_ax(GetAx());
	dynamics.set_ay(GetAy());
	dynamics.set_az(GetAz());
}

void Gps::FromProto(const Proto::Gps& gps)
{
	UPDATE(gps, lon, *this, PositionLon);
	UPDATE(gps, lat, *this, PositionLat);
}

void Gps::ToProto(Proto::Gps& gps)
{
	gps.set_lon(GetPositionLon());
	gps.set_lat(GetPositionLat());
}

void Powertrain::FromProto(const Proto::Powertrain& powertrain)
{
	UPDATE(powertrain, engine_rotation, *this, EngineRotation);
	UPDATE(powertrain, engine_power_out, *this, EnginePowerOut);
	UPDATE(powertrain, engine_torque, *this, EngineTorque);
	UPDATE(powertrain, engine_fuel_consumption_abs, *this, EngineFuelConsumptionAbs);
	UPDATE(powertrain, engine_fuel_consumption_act, *this, EngineFuelConsumptionAct);
	UPDATE(powertrain, engine_fuel_consumption_avg, *this, EngineFuelConsumptionAvg);
	UPDATE(powertrain, engine_fuel_level, *this, EngineFuelLevel);

	UPDATE(powertrain, gearbox_gear, *this, GearboxGear);
	UPDATE(powertrain, gearbox_power_in, *this, GearboxPowerIn);
	UPDATE(powertrain, gearbox_power_out, *this, GearboxPowerOut);
	UPDATE(powertrain, gearbox_rotation_in, *this, GearboxRotationIn);
	UPDATE(powertrain, gearbox_rotation_out, *this, GearboxRotationOut);
}

void Powertrain::ToProto(Proto::Powertrain& powertrain)
{
	powertrain.set_engine_rotation(GetEngineRotation());
	powertrain.set_engine_power_out(GetEnginePowerOut());
	powertrain.set_engine_torque(GetEngineTorque());
	powertrain.set_engine_fuel_consumption_abs(GetEngineFuelConsumptionAbs());
	powertrain.set_engine_fuel_consumption_act(GetEngineFuelConsumptionAct());
	powertrain.set_engine_fuel_consumption_avg(GetEngineFuelConsumptionAvg());
	powertrain.set_engine_fuel_level(GetEngineFuelLevel());

	powertrain.set_gearbox_gear(GetGearboxGear());
	powertrain.set_gearbox_power_in(GetGearboxPowerIn());
	powertrain.set_gearbox_power_out(GetGearboxPowerOut());
	powertrain.set_gearbox_rotation_in(GetGearboxRotationIn());
	powertrain.set_gearbox_rotation_out(GetGearboxRotationOut());
}

void DriverInput::FromProto(const Proto::DriverInput& driverInput)
{
	UPDATE(driverInput, throttle, *this, Throttle);
	UPDATE(driverInput, brake, *this, Brake);
	UPDATE(driverInput, clutch, *this, Clutch);
	UPDATE(driverInput, steering_wheel_angle, *this, SteeringWheelAngle);
}

void DriverInput::ToProto(Proto::DriverInput& driverInput)
{
	driverInput.set_throttle(GetThrottle());
	driverInput.set_brake(GetBrake());
	driverInput.set_clutch(GetClutch());
	driverInput.set_steering_wheel_angle(GetSteeringWheelAngle());
}

void VehicleData::SerializeQuantities(QVector<QString>& output)
{
	output.append("VehicleData.Gps:");
	gps->SerializeQuantities(output);
	output.append("");

	output.append("VehicleData.DriverInput:");
	driverInput->SerializeQuantities(output);
	output.append("");

	output.append("VehicleData.Dynamics:");
	dynamics->SerializeQuantities(output);
	output.append("");

	output.append("VehicleData.Powertrain:");
	powertrain->SerializeQuantities(output);
	output.append("");
}

void DataModel::SerializeQuantities()
{
	serializedQuantities.clear();

	vehicleData->SerializeQuantities(serializedQuantities);

	controlDataStatic->SerializeQuantities(serializedQuantities);
	controlDataDynamic->SerializeQuantities(serializedQuantities);

	emit(SerializedQuantitiesChanged());
}

}
