#pragma once

#include "Engine/modules/Model/Mesh.h"

namespace Echo
{
	/**
	 * ������ƶ�̬Mesh
	 */
	class DynamicMesh : public Mesh
	{
	public:
		DynamicMesh(const String& name);
		virtual ~DynamicMesh();

		// �Ƿ���Ҫ����
		virtual bool isNeedUpdate(){ return true; }

		// ����
		virtual void update(ui32 delta, const Echo::Vector3& vpos, const Echo::Quaternion& qrotate);

	protected:
		virtual bool prepareImpl(DataStream* stream);
		virtual void unprepareImpl();
	};
}
