#pragma once
//http://stackoverflow.com/questions/13358672/how-to-convert-a-lambda-to-an-stdfunction-using-templates

template<typename T>
struct memfun_type
{
	using type = void;
};

template<typename Ret, typename Class, typename... Args>
struct memfun_type<Ret(Class::*)(Args...) const>
{
	using type = std::function<Ret(Args...)>;
};

template<typename F>
typename memfun_type<decltype(&F::operator())>::type
FFL(F const &func)
{ // Function from lambda !
	return func;
}


