#pragma once

template <typename T>
class Storage1 {
private:
	T* data;
	int capacity;
	int length;
	void copy_data(T* from, T* to, T* dest) {
		for (T* i = from; i != to; ++i) {
			*dest++ = *i;
		}
	}
	void resize() {
		int newCapacity = capacity == 0 ? 1 : capacity * 2;
		T* newData = new T[newCapacity];
		copy_data(data, data + length, newData);
		delete[] data;
		data = newData;
		capacity = newCapacity;
	}

public:
	Storage1() : data(nullptr), capacity(0), length(0) {}
	~Storage1() {
		delete[] data;
	}
	void push_back(const T& value) {
		if (length >= capacity) {
			resize();
		}
		data[length++] = value;
	}
	T& operator[](int index) {
		return data[index];
	}
	T& at(int index) {
		return data[index];
	}
	int size() const {
		return length;
	}
	void erase(int index) {
		for (int i = index; i < length - 1; i++) {
			data[i] = data[i + 1];
		}
		length--;
	}
	void clear() {
		length = 0;
	}
};
