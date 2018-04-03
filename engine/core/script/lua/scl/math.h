////////////////////////////////////////////////////////////////////////////////
//	mathematical
//	2010.05.01 caolei
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "scl/type.h"

namespace scl {

const float PI			= 3.1415926535897932384626433832795f;
const float PI2			= 6.283185307179586476925286766559f;
const float PI_HALF		= 1.5707963267948966192313216916398f;
const float PI_QUATER	= 0.78539816339744830961566084581988f;

const	int		MAX_INT		= 0x7FFFFFFF;
const	uint	MAX_UNIT	= 0xFFFFFFFF;
const	int64	MAX_INT64	= 0x7FFFFFFFFFFFFFFF;
const	uint64	MAX_UINT64	= 0xFFFFFFFFFFFFFFFF;

const	int		MIN_INT		= 0x80000000;
const	int64	MIN_INT64	= 0x8000000000000000;

//�����ֵ
template<typename T> 
inline T absolute(T x)
{
	return x > 0 ? x : -x;
}

//��������ֵ
#ifdef SCL_WIN //windows�����<utility>�ļ�д�������⣬���¶�swap�ĵ��û�����
#else
template<typename T> 
inline void swap(T& a, T& b)
{
	T t = a;
	a = b;
	b = t;
}
#endif

//�Ƕ�תΪ����
inline float radian(const float a)
{
	return PI * a / 180;
}

//����תΪ�Ƕ�
inline float angle(const float r)
{
	return 180 * r / PI;
}

//��ϴ�ֵ
#ifdef max
#undef max
#endif
template<typename T> 
inline T max(const T a, const T b)
{
	return a > b ? a : b;
}

#ifdef min 
#undef min 
#endif
template<typename T> 
inline T min(const T a, const T b)
{
	return a < b ? a : b;
}



//�жϸ������Ƿ����
inline bool float_equal(const float a, const float b, const float precision = 0.0001)
{
	return absolute(a - b) < precision;
}

//Բ��������Ϊ����
//inline int round(const float f)
//{
//	if (f > 0)
//	{
//		return static_cast<int>(f + 0.5f);
//	}
//	else
//	{
//		return -static_cast<int>(-f + 0.5);
//	}
//}

//Բ��������Ϊ����
template <typename T>
inline int round(const T f)
{
	return f > 0 ? static_cast<int>(f + 0.5f) : -static_cast<int>(-f + 0.5f);
}

//�ж�һ��int�Ƿ�Ϊ����
bool is_prime(uint n);

//�ҳ�С��n���������
uint min_prime(uint n);

//��base��times�η�
int pow(int base, int times);

//��2����ģ
inline double mod2PI(const double angel)
{
	double r = angel - static_cast<int>(angel / PI2) * PI2;
	return r;
}

//���Բ�ֵ
float	lerpf	(float from, float to, float t);
int		lerp	(int from, int to, float t);
//float	lerpf	(float from, float to, float t_from, float t_to, float t_value);
//float	lerpf	(float from, float to, uint t_from, uint t_to, uint t_value);

template <typename T>
inline bool is_mul_overflow(const int v1, const T v2)
{
	if (v1 == 0 || v1 == 1)
		return false;
	double max_v2 = scl::MAX_INT / static_cast<double>(v1);
	return v2 >= max_v2;
}

inline bool is_add_overflow(const int v1, const int v2)
{
	if (v1 == 0 || v2 == 0)
		return false;
	int64 t1 = static_cast<int64>(v1) + v2;
	return t1 > scl::MAX_INT || t1 < scl::MIN_INT;
}

inline bool is_add_overflow(const int64 v1, const int64 v2)
{
	if (v1 == 0 || v2 == 0)
		return false;
	if (v1 < 0 && v2 > 0)
		return false;
	if (v1 > 0 && v2 < 0)
		return false;
	int64 left = scl::MAX_INT64 - absolute(v1);
	if (absolute(v2) > left)
		return true;
	return false;
}

template <typename T>
inline int safe_mul(const int v1, const T v2)
{
	if (is_mul_overflow(v1, v2))
		return scl::MAX_INT;
	else
		return static_cast<int>(v1 * v2);
}

class rander
{
public:
	static const unsigned int MAX16 = 0x7fff;
	static const unsigned int MAX32 = 0x7fffffff;
	rander();

	void			srand(const int seed);

	int				rand	(const int min, const int max);
	unsigned int	rand16	();		//rand 16 bit number, [0 ~ 32767] (0x7fff)
	unsigned int	rand32	();		//rand 32 bit number, [0 ~ 2147483647] (0x7fffffff)

	bool			rand_percent(const float percent);	//rand percent, return is success;

	unsigned int	seed	() {return m_seed;};
	void			set_seed(unsigned int seed) { m_seed = seed; }

private:
	unsigned int m_seed;
};

////////////////////////////////////
//ȫ��rand�������̲߳���ȫ
////////////////////////////////////
unsigned int rand	(const int min, const int max); //random in [min, max]
void		 srand	(int);							//����ȫ��������� 
unsigned int rand16	();								//random 16 bit number, [0 ~ 32767] (0x7fff)
unsigned int rand32	();								//random 32 bit number, [0 ~ 2147483647] (0x7fffffff)

} //namespace scl
