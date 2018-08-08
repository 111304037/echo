#pragma once

#include <Foundation/Util/LogManager.h>
#include "Example.h"
#include "UIManager.h"

namespace Examples
{
	/**
	 * ִ�г���
	 */
	class Application : public CEGUI::EventSet
	{
	public:
		Application();
		~Application();

		// for Android: set resource dir.
		void setAssetPath(const char* path, const char* internal_path); 

		// ����ִ��
		void start(LORD::i32 width = 0, LORD::i32 height = 0);

		void end();

		// ����
		void tick(LORD::ui32 elapsedTime);

		// ����ʾ��
		void startExample(LORD::ui32 index);

		// ����������ȡʾ������
		ExampleFactory* getFactoryByIdx(int idx);

	public:
		// ������Ϣ
		void keyboardProc( LORD::ui32 keyChar, bool isKeyDown);

		// ��������Ϣ
		void mouseWhellProc( LORD::i32 parma);

		// �����Ϣ����
		void mouseProc( LORD::i32 xpos, LORD::i32 ypos);

		// ��������Ϣ
		void mouseLBProc( LORD::i32 xpos, LORD::i32 ypos, bool isUp = false);

		// ����ƶ���Ϣ
		void mouseMoveProc( LORD::i32 xpos, LORD::i32 ypos);

		// ������һʾ��
		bool nextExample(const CEGUI::EventArgs& args);

		// ������һʾ��
		bool lastExample(const CEGUI::EventArgs& args);

		// ������ǰʾ��
		bool restartExample(const CEGUI::EventArgs& args);

		void flushRender();

	private:
		// ��ʼ������
		void initEngine();

		// ��ʼ��UI
		void initUI();

		// ���ô��ڴ�С
		void resize(int cx, int cy);

		// ��ⴰ�ڴ�С�仯
		void checkScreenSize();

	private:
		LORD::i32					m_screenWidth;		// ��Ļ��
		LORD::i32					m_screenHeight;		// ��Ļ��
		LORD::LogDefault*			m_log;				// ��־ʵ��
		LORD::ui32					m_curExampleIdx;	// ʾ������
		Example*					m_curExample;		// ��ǰʾ��
		UIManager*					m_uiManager;		// �������
		CEGUI::Window*				m_selectWindow;		// ����ѡ�񴰿�
		CEGUI::Window*				m_circulateWindow;	// ѭ��ѡ�񴰿�

		LORD::String				m_path; 
		LORD::String				m_internalPath; 
	};

	/**
	 * ��Ϣ�ص�
	 */
	class CEGUIFunctor
	{
	public:
		CEGUIFunctor(int id):m_id(id) {}
		~CEGUIFunctor() {}

		// ���������
		bool operator()(const CEGUI::EventArgs& args) const;

	private:
		int	m_id;
	};
}