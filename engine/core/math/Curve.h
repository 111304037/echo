#ifndef __ECHO_CURVE_H__
#define __ECHO_CURVE_H__

#include "Vector3.h"

namespace Echo
{
	//---------------------------------------------------------------
	// Bezier3 2011-07-12  ����
	// ��Ҫ�ο���<<DirectX ʵʱ��Ⱦ�������>> ��21�½� ��������Ƭ
	//			 ����˹̹����ʽ ( Bernstein polynomials)
	// 
	// ˵����
	//	��һ����Ƶ�������������(�˴�����ֻ�����ĸ����Ƶ��ʵ��)
	// out ���ص�
	// v1,v2,v3,v4 ���Ƶ�
	// amount      ������
	Vector3 Bezier3( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount);

	//---------------------------------------------------------------
	// Catmull_Rom3 2011-07-12  ����
	// ��Ҫ�ο���<<game programming gems 1>> 3.4-A*�����Ż�
	// 
	// ˵����
	//     Catmull-Rom��������ʹ�ռ��еĵ����б��ƽ��,��������һ��
	// �ܹ�����ԭʼ·�������п��Ƶ������(����Bezier����,��Ȼ�Ƚ�ƽ
	// �����ǲ��ܾ������еĿ��Ƶ�)��
	//	   Catmull-Rom��ʽҪ��4�������,Ȼ�����һ��λ�ڵ�2��͵�3��
	// ֮��Ĺ⻬���ߡ�
	//     Ϊ�˵õ���1�������͵ڶ��������֮��ĵ�,�����Ϊ�������
	// �����1��������,���������2��͵�3�㡣Ϊ�˻�õ�3��͵�4��֮��
	// �ĵ�,������Ϊ���������2��͵�3�㣬Ȼ�������4�����Ρ�
	// out ���ص�
	// v1,v2,v3,v4 ���Ƶ�
	// amount      ������
	Vector3 CatmullRom3(Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount);

	Vector3 Bezier2(Vector3& out, const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

	// ���㾭�� p0 p1 p2 �Ķ��α��������ߵĿ��Ƶ�
	Vector3 Bezier2TensionPoint(Vector3& out, const Vector3& p0, const Vector3& p1, const Vector3& p2);
}

#endif