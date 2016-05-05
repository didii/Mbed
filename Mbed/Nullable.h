#include "stdafx.h"

#pragma once

// <Momenteel ongebruikt en zal waarschijnlijk ook zo blijven>
// Nullable template class, allows null value for any non-nullable object
template <typename T>
class Nullable {
	T _value;
	bool _hasValue;

public:
	Nullable<T>();

	void SetAsNull();
	void SetValue(T val);
	T Value() const;
	bool HasValue() const;

	void operator=(T val);
};

#include "Nullable.inl"