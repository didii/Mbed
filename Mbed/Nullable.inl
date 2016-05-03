template <typename T>
Nullable<T>::Nullable() {
	_hasValue = false;
}

template <typename T>
void Nullable<T>::SetAsNull() {
	_hasValue = false;
}

template <typename T>
void Nullable<T>::SetValue(T val) {
	_hasValue = true;
	_value = val;
}

template <typename T>
T Nullable<T>::Value() const {
	if (!_hasValue)
		throw "Null exception";
	return _value;
}

template <typename T>
bool Nullable<T>::HasValue() const {
	return _hasValue;
}

template <typename T>
void Nullable<T>::operator=(T val) {
	_hasValue = true;
	_value = val;
}
