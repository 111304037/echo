#pragma once

#include <engine/core/Util/Array.hpp>
#include "Light.h"

namespace Echo
{
	/**
	 * ��Դ����
	 */
	class LightArray
	{
	public:
		LightArray();
		~LightArray();

		// ��ȡ����
		const char* getName() const { return m_name.c_str(); }

		// ����
		void update();

		void updatePointLightPos( const Vector4& pos );

		// ��ȡ��Դ����
		int getLightNum() { return static_cast<int>(m_lights.size()); }

		// ��ȡ��Դ
		Light* getLight(int idx) { return m_lights[idx]; }

		// ��ȡ��Դ��Ϣ
		void* getLightInfo() { return m_info.data(); }

		// ��ȡ��Դ����
		void* getLightData() { return m_data.data(); }

		void* getPBRLightPos() { return m_pbrLightPos.data(); }
		void* getPBRLightDir() { return m_pbrLightDir.data(); }
		void* getPBRLightSopt() { return m_pbrLightSpot.data(); }
		void* getPBRLightParam() { return m_pbrLightParam.data(); }
		void* getPBRLightColor() { return m_pbrLightColor.data(); }

		// ��ӹ�Դ
		Light* addLight(LightType type);

		// ɾ����Դ
		void deleteLight(int idx);

		// ɾ�����й�Դ
		void deleteAllLights();

		// ��������
		void setDirty() { m_isDirty = true; }

		bool isDirty() { return m_isDirty; }

		// ����
		void load(const char* fileName);

		// ����
		void save(const char* fullPath);

	private:
		String						m_name;		// �ļ�����
		array<LightType, 8>			m_info;		// ��Դ������Ϣ
		array<Vector4, 16>			m_data;		// ��Դ����
		vector<Light*>::type		m_lights;	// ��Դ
		bool						m_isDirty;	// ����

		vector<Vector4>::type		m_pbrLightPos;
		vector<Vector3>::type		m_pbrLightDir;
		vector<Vector3>::type		m_pbrLightColor;
		vector<Vector3>::type		m_pbrLightSpot;
		vector<Vector3>::type		m_pbrLightParam;
	};
}