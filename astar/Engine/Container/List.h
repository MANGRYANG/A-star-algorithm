#pragma once

#pragma warning(disable : 4251)

#include <iostream>
#include <stdexcept>

template <typename T>
class List
{
public:
    List()
        : capacity(2), size(0)
    {
        data = new T[capacity];
    }

    ~List()
    {
        delete[] data;
    }

    void PushBack(const T& value)
    {
        if (size == capacity)
        {
            ReAllocate(capacity * 2);
        }

        data[size] = value;
        ++size;
    }

    void PushBack(T&& value)
    {
        if (size == capacity)
        {
            ReAllocate(capacity * 2);
        }

        data[size] = std::move(value);
        ++size;
    }

    void Erase(int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of bounds in List::Erase()");
        }

        for (int ix = index; ix < size - 1; ++ix)
        {
            data[ix] = std::move(data[ix + 1]);
        }

        --size;
    }

    void Clear()
    {
        size = 0;
    }

    int Size() const
    {
        return size;
    }

    int Capacity() const
    {
        return capacity;
    }

    const T& operator[](int index) const
    {
        if (index >= size)
        {
            throw std::out_of_range("Index out of bounds in List::operator[]");
        }

        return data[index];
    }

    T& operator[](int index)
    {
        if (index >= size)
        {
            throw std::out_of_range("Index out of bounds in List::operator[]");
        }

        return data[index];
    }

    T* begin() const
    {
        return data;
    }

    T* end() const
    {
        return data + size;
    }

private:
    void ReAllocate(int newCapacity)
    {
        T* newBlock = new T[newCapacity];

        for (int ix = 0; ix < size; ++ix)
        {
            newBlock[ix] = std::move(data[ix]);
        }

        delete[] data;
        data = newBlock;
        capacity = newCapacity;
    }

private:
    T* data;
    int size;
    int capacity;
};
