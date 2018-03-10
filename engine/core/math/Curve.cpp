#include "Curve.h"

namespace Echo
{
	//---------------------------------------------------------------
	// Bezier3 2011-07-12  ����
	// ��Ҫ�ο���<<DirectX ʵʱ��Ⱦ�������>> ��21�½� ��������Ƭ
	// 
	// ˵����
	//	��һ����Ƶ�������������(�˴�����ֻ�����ĸ����Ƶ��ʵ��)
	// out ���ص�
	// v1,v2,v3,v4 ���Ƶ�
	// amount      ������
	Vector3 Bezier3( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount)
	{
		// amount
		float  s  = amount;
		float _s = 1.f - amount;

		out = v1 *   _s * _s * _s +
			  v2 *  3.f *  s * _s * _s +
			  v3 *  3.f *  s *  s * _s +
			  v4 *    s *  s *  s;

		return out;
	}

	//---------------------------------------------------------------
	// Bezier3 2017-2-28
	// ��Ҫ�ο���<<DirectX ʵʱ��Ⱦ�������>> ��21�½� ��������Ƭ
	// 
	// ˵����
	//	��һ����Ƶ�������������(�˴�����ֻ�����ĸ����Ƶ��ʵ��)
	// out ���ص�
	// v1,v2,v3,v4 ���Ƶ�
	// amount      ������
	float Bezier3(float& out, const float& v1, const float& v2, const float& v3, const float& v4, float amount)
	{
		// amount
		float  s = amount;
		float _s = 1.f - amount;

		out = v1 *   _s * _s * _s +
			v2 *  3.f *  s * _s * _s +
			v3 *  3.f *  s *  s * _s +
			v4 *    s *  s *  s;

		return out;
	}

	// ˵����
	//     Catmull-Rom��������ʹ�ռ��еĵ����б��ƽ��,��������һ��
	// �ܹ�����ԭʼ·�������п��Ƶ������(����Bezier����,��Ȼ�Ƚ�ƽ
	// �����ǲ��ܾ������еĿ��Ƶ�)��
	Vector3 CatmullRom3( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount)
	{
		// amount
		float u3 = amount * amount * amount;
		float u2 = amount * amount;
		float u1 = amount;

		out = v1 * ( -0.5f * u3 +         u2 - 0.5f * u1) +
			v2 * (  1.5f * u3 + -2.5f * u2 + 1.f)		  +
			v3 * ( -1.5f * u3 +   2.f * u2 + 0.5f * u1) +
			v4 * (  0.5f * u3 -  0.5f * u2);

		return out;
	}

	Vector3 Bezier2(Vector3& out, const Vector3& p0, const Vector3& p1, const Vector3& p2, float t)
	{
		out = (1 - t) * (1 - t) * p0 + 2 * t * (1 - t) * p1 + t * t * p2;
		return out;
	}

	Vector3 Bezier2TensionPoint(Vector3& out, const Vector3& p0, const Vector3& p1, const Vector3& p2)
	{
		Vector3 p01 = p0 - p1;
		Vector3 p21 = p2 - p1;
		auto p01Len = p01.len();
		auto p21Len = p21.len();
		if (Math::Abs(p01Len) < Math::EPSILON || Math::Abs(p21Len) < Math::EPSILON)
		{
			return Vector3::ZERO;
		}
		out = p1 - Real(0.5) * std::sqrt(p01Len * p21Len) * (p01 / p01Len + p21 / p21Len);
		return out;
	}
}