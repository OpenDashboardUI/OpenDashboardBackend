// Copyright (C) 2020 twyleg
#pragma once

#include <QObject>
#include <QMetaProperty>

namespace OpenDashboard::DataModels {

#define ADD_PROPERTY(datatype, name, funcName, initial_value) \
private: \
	datatype name = initial_value; \
	Q_PROPERTY(datatype name READ Get##funcName WRITE Set##funcName NOTIFY  funcName##Changed) \
public: \
	datatype& Get##funcName() { return name; } \
	void Set##funcName(const datatype value) \
	{ \
		name = value; \
		emit funcName##Changed(); \
	} \
Q_SIGNALS: \
	void funcName##Changed(); \
private: \

#define ADD_PROPERTY_PTR(datatype, name, funcName) \
private: \
	datatype* name = new datatype(this); \
	Q_PROPERTY(datatype* name READ Get##funcName WRITE Set##funcName NOTIFY  funcName##Changed) \
public: \
	datatype* Get##funcName() { return name; } \
	void Set##funcName(datatype* value) \
	{ \
		name = value; \
		emit funcName##Changed(); \
	} \
Q_SIGNALS: \
	void funcName##Changed(); \
private: \

#define UPDATE(protoObj, protoProperty, qObj, qProperty) \
	if ((protoObj).has_##protoProperty()) \
		(qObj).Set##qProperty((protoObj).protoProperty()) \

class SerializableQObject : public QObject
{
	Q_OBJECT
public:
	void SerializeQuantities(QVector<QString>& output)
	{
		const QMetaObject* metaObject = this->metaObject();
		for (qint32 i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
		{
			const char* pPropertyName = metaObject->property(i).name();
			const QString propertyName = QString::fromLatin1(pPropertyName);
			const QString propertyValue = property(pPropertyName).toString();
			static QString seperator(": ");

			const QString outputLine = propertyName + seperator + propertyValue;
			output.append(outputLine);
		}
	}

protected:
	SerializableQObject(QObject* parent = nullptr)
	: QObject(parent)
	{}
};


}
