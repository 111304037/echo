#pragma once

#include <engine/core/camera/Camera.h>

namespace Studio
{
	/**
	 * UI�����
	 */
	class UICamera : public Echo::Camera
	{
	public:
		UICamera();
		~UICamera();

		// ������Ļ���
		void setWidthHeight( Echo::ui32 width, Echo::ui32 height);

		// ����
		virtual void update();
	};

	// UI�߼���Сӳ��
	void MappingUILogicWH( Echo::ui32 screenWidth, Echo::ui32 screeHeight, Echo::ui32& oLogicWidth, Echo::ui32& oLogicHeight);
}