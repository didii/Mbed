#include "stdafx.h"

#pragma once
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