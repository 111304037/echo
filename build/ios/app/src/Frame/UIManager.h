#pragma once

#include <Cegui/include/CEGUI.h>
#include <Cegui/include/CEGUISystem.h>
#include <Cegui/include/CEGUIAdapter.h>
#include <Cegui/include/CEGUIPfsResourceProvider.h>
#include <Cegui/include/RendererModules/Lord/CEGUILordRenderer.h>
#include <Cegui/include/CEGUIEvent.h>
#include "UIAdapter.h"

namespace Examples
{
	/**
	 * ���ӽ��������
	 */
	class UIManager
	{
	public:
		UIManager();
		~UIManager();

		// ��ʼ��
		void initialize();

		// ����
		void tick( int delta);

		// ���ڴ�С�ı�
		void onSize(int screenW, int screenH);

		// ��ȡ������
		CEGUI::Window* getRootWindow() { return m_rootWindow; }

	private:
		// ������ԴĿ¼
		void initResourceDirectoriesAndGroups();

		// ��ʼģ̬
		bool onStartModalState(const CEGUI::EventArgs& e);

		// ����UI����ǰ����
		bool handleBeforeLoadlayout(const CEGUI::EventArgs& e);

	private:
		CEGUI::CEGUIRenderer*		m_uiRenderer;			// CEGUI��Ⱦ��
		Examples::UIAdapter			m_adapter;				// ������
		CEGUI::ResourceProvider*	m_uiResourceProvider;	// ��Դ������
		CEGUI::Window*              m_rootWindow;			// ������
	};
}