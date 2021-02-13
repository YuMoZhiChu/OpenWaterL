#pragma once
// string 的 hash 化
// 参考 https://blog.csdn.net/yozidream/article/details/22789147

#include<cstdint>

typedef std::uint16_t _openwaterl_hash_t;

constexpr _openwaterl_hash_t _openwaterl_hash_prime = _openwaterl_hash_t(0x100000001B3ull);
constexpr _openwaterl_hash_t _openwaterl_hash_basis = _openwaterl_hash_t(0xCBF29CE484222325ull);

// 函数写法，const char* str 转换为 uint16_t
_openwaterl_hash_t _openwaterl_hash_(const char* str)
{
	// 初始化 ret
	_openwaterl_hash_t ret{ _openwaterl_hash_basis };
	while (*str)
	{
		ret ^= *str;
		ret *= _openwaterl_hash_prime;
		++str;
	}
	return ret;
}

// 常量表达式写法，利用递归的特性，在编译期就可以得到一个 字符串的 hash 值
constexpr _openwaterl_hash_t _openwaterl_hash_compile_time(const char* str, _openwaterl_hash_t last_value = _openwaterl_hash_basis)
{
	return *str ? _openwaterl_hash_compile_time(str + 1, (*str ^ last_value) * _openwaterl_hash_basis) : last_value;
}

// 自定义文字常量，重载 operator
// 写法 xxx_openwaterl_hash 即可
constexpr unsigned long long operator ""_openwaterl_hash_compile_time(char const* p, size_t)
{
	return _openwaterl_hash_compile_time(p);
}
