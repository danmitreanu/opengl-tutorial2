#include "Pool.h"

template class Pool<UniformNode>;
template class Pool<TextureNode>;

template <typename T>
Pool<T>::Pool(std::size_t initial_size)
{
    m_SizeUnit = initial_size;
    m_Reserved = initial_size;

    m_Array = new T[m_Reserved];
}

template <typename T>
Pool<T>::~Pool()
{
    delete [] m_Array;
}

template <typename T>
void Pool<T>::reset()
{
    m_Index = 0;
}

template <typename T>
void Pool<T>::extend(std::size_t magnitude)
{
    std::size_t extend_count = magnitude * m_SizeUnit;

    T* temp = new T[m_Reserved + extend_count];
    for (std::size_t i = 0; i < m_Reserved; i++)
    {
        temp[i] = m_Array[i];
    }

    m_Reserved += extend_count;
    delete [] m_Array;
    m_Array = temp;
}

template <typename T>
T* Pool<T>::add(T& item)
{
    std::size_t needed_size = m_Index + 1;
    if (needed_size >= m_Reserved)
        extend();

    m_Array[m_Index] = item;
    return (m_Array + m_Index++);
}
