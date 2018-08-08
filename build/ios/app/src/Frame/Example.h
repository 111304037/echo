#pragma once

#include <Foundation/Util/StringUtil.h>
#include <Engine/Object/Root.h>

namespace Examples
{
	/**
	 * ʾ������
	 */
	class Example
	{
	public:
		Example() {}
		virtual ~Example(){}

		// ������Ϣ
		virtual const LORD::String getHelp() = 0;

		// ��ʼ��
		virtual bool initialize() = 0;

		// ����
		virtual void tick(LORD::ui32 elapsedTime) = 0;

		// ����
		virtual void destroy() = 0;

		// ������Ϣ
		virtual void keyboardProc(LORD::ui32 keyChar, bool isKeyDown){}

		// �����Ϣ
		virtual	void mouseWhellProc( LORD::i32 parma ) {}
		virtual void mouseProc( LORD::i32 xpos, LORD::i32 ypos ) {}
		virtual void mouseLBProc( LORD::i32 xpos, LORD::i32 ypos ) {}
		virtual void mouseMoveProc( LORD::i32 xpos, LORD::i32 ypos ) {}

	public:
	
	};

	/**
	 * ʾ����������
	 */
	struct ExampleFactory
	{
		LORD::String m_name;			// ʾ������
		LORD::String m_imageSet;		// ��ʶ����ͼʹ�õ�����

		// ����ʾ��
		virtual Example* create() = 0;
	};

	extern LORD::vector<ExampleFactory*>::type	g_exampleFactorys;

	/**
	 * ʾ������ģ��
	 */
	template<typename T> 
	struct ExampleFactoryT : public Examples::ExampleFactory
	{
		// ���캯��
		ExampleFactoryT( const LORD::String& name, const LORD::String& imageset)
		{
			m_name     = name;
			m_imageSet = imageset;

			Examples::g_exampleFactorys.push_back(this);
		}

		// ��ʼ��
		virtual Example* create()
		{
			return LordNew(T);
		}
	};

	// ����ȫ�ֱ���ע��ʾ����������,���ͣ����ƣ�����ͼ(imageset)
	#define EXAMPLE_REGISTER(T,N, IS) static ExampleFactoryT<T> G_##T(N, IS)
}

