#include "gauge.hpp"

Gauge::Gauge(const float max_value, const float current_max_value) :
	m_current_value		(current_max_value),
	m_prev_value		(current_max_value),
	m_current_max_value	(current_max_value),
	m_max_value			(max_value)
{

}

Gauge::Gauge(const float max_value) :
	m_current_value		(max_value),
	m_prev_value		(max_value),
	m_current_max_value	(max_value),
	m_max_value			(max_value)
{

}

Gauge::~Gauge()
{

}

void Gauge::IncreaseCurrentMax()
{
	m_current_value = m_current_max_value;
}

void Gauge::DecreaseZero()
{
	m_current_value = 0.0f;
}

void Gauge::Increase(const float increase_value)
{
	math::Increase(m_current_value, increase_value, m_current_max_value, false);
}

void Gauge::Decrease(const float decrease_value)
{
	m_prev_value = m_current_value;
	math::Decrease(m_current_value, decrease_value, 0.0f);
}

void Gauge::SetCurrentMaxValue(const float current_max_health)
{
	m_current_max_value = current_max_health;
	if (m_current_max_value > m_max_value)
	{
		m_current_max_value = m_max_value;
	}
}

void Gauge::SetCurrentValue(const float current_value)
{
	m_current_value = current_value;
	if (m_current_value > m_current_max_value)	{ m_current_value = m_current_max_value; }
	if (m_current_value < 0.0f)					{ m_current_value = 0.0f; }
}
