#pragma once

#include "engine/core/Math/Vector3.h"

namespace Echo
{
	/**
	 * Triangle����
	 */
	class Triangle
	{
	public:
		Triangle(){}
		Triangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2): v0(vert0), v1(vert1), v2(vert2) {}

		// ����
		void set(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2);

		// ��ȡ����
		Vector3 GetNormal() const;

	public:
		Vector3	v0;		//!< Vertex 0
		Vector3	v1;		//!< Vertex 1
		Vector3	v2;		//!< Vertex 2
	};
}