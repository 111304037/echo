#ifndef __ECHO_RENDERQUEUE_H__
#define __ECHO_RENDERQUEUE_H__

#include <engine/core/Memory/MemManager.h>
#include "ShaderProgram.h"

namespace Echo
{
	class Renderable;
	class Material;

	class ManualRenderListener
	{
	public:
		virtual void render() = 0;
		virtual void beginRender()=0;
	};

	/**
	 * ��Ⱦ����
	 */
	class RenderQueue
	{
		friend class Renderable;

		typedef vector<int>::type RenderableList;
	public:
		RenderQueue(const String& strName);
		~RenderQueue();

		// ��ʼ��Ⱦ
		void beginRender();
		void setManalRenderBegin(ManualRenderListener* mListener);
		void setManalRenderEnd(ManualRenderListener* mListener);

		bool hasRenderables(){ return !m_Renderables.empty(); }

		void sortByPos();

	private:
		// �����Ⱦ��
		void addRenderable(Renderable* pRenderable);

	public:
		// ִ����Ⱦ(����Ϊ������Ⱦ)
		void renderQueue();

		// ��տ���Ⱦ��
		void clearRenderables();

		// ��ȡ����
		const String& getName() { return m_strName; }

		// ��ȡ����
		Material* getMaterial() const;

		// ����Ϊ����Ⱦ
		void enableRender() { m_bEnableRender = true; }
		
		// ��ͣ��Ⱦ
		void disableRender(){ m_bEnableRender = false; }

		// �Ƿ����
		bool isEnable() const { return m_bEnableRender; }

		// �Ƿ��������
		//void setIsSort(bool isSort) { m_isSort = isSort; }

		void addGlobalParams(const String& name, ShaderParamType type, const void* value, size_t count);

		void setRenderWithoutMacros(const StringArray& macros,bool refresh = false);

	protected:
		String					m_strName;
		Material*				m_pMaterial;		// ����
		RenderableList			m_Renderables;		
		ManualRenderListener*	m_ManualBegin;
		ManualRenderListener*	m_ManualEnd;
		bool					m_bEnableRender;
		//bool					m_isSort;			// ��Ⱦǰ�Ƿ��������
		RenderQueue*			m_ManualRenderQueue;// �ֶ�����shader����Ⱦ 
		bool					m_isInitManualRQ;
		bool					m_useManualRQ;		//ֻ����һ�루��Ⱦ�����ó�false��

		// ��ɫ������
		struct ShaderParam
		{
			String			name;	
			ShaderParamType type;
			const void*		value;
			ui32			count;  // shader constance register num.
		};
		std::vector<ShaderParam>		m_globalParams;
	};
}

#endif
