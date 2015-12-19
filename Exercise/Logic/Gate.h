#pragma once
#include <functional>
#include <array>
#include <memory>
#include <assert.h>

class IGate abstract
{
public:
	virtual bool Output() const abstract;
};

template<typename... Inputs>
class Gate : public IGate
{
private:
	std::function<bool ()> m_logicFunction;
	
public:
	Gate(const std::function<bool(const Inputs&...)> &logicFunction,
		const Inputs&... inputs)
		: m_logicFunction(std::bind(logicFunction, cref(inputs)...)) {}
	
	bool Output() const override
	{
		return m_logicFunction();
	}
};

typedef Gate<const IGate&> one_input_gate_t;
typedef Gate<const IGate&, const IGate&> two_inputs_gate_t;

class On : public IGate
{

public:
	bool Output() const override
	{
		return true;
	}

};

class Off : public IGate
{
public:
	bool Output() const override
	{
		return false;
	}
};


class Switch : public IGate
{
private:
	bool m_state = false;
public:
	void Set() { m_state = true; }
	void Reset() { m_state = false; }
	void Toggle() { m_state = !m_state; }
	bool Output() const override
	{
		return m_state;
	}
};


