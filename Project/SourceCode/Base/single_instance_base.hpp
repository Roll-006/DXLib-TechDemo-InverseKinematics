#pragma once
#include <cassert>

/// @brief インスタンスが1つに制限される基底クラス
template<typename T>
class SingleInstanceBase abstract
{
public:
	SingleInstanceBase()
	{
		assert(m_instance == nullptr);
		m_instance = static_cast<T*>(this);
	}

	virtual ~SingleInstanceBase()
	{
		m_instance = nullptr;
	}

	static T* GetInstance() { return m_instance; }

protected:
	static T* m_instance;
};

template<typename T>
T* SingleInstanceBase<T>::m_instance = nullptr;