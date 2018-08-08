#ifndef __DESTRUCTION_DEMO_H__
#define __DESTRUCTION_DEMO_H__

#include "Frame/Example.h"

namespace Examples
{
	class DestructionDemo : public Example
	{
	public:
		DestructionDemo( );
		virtual ~DestructionDemo( ) {}

		// ������Ϣ
		virtual const LORD::String getHelp( );

		// ��ʼ��
		virtual bool initialize( );

		// ����
		virtual void tick( LORD::ui32 elapsedTime );

		// ����
		virtual void destroy( );

		virtual void keyboardProc( LORD::ui32 keyChar, bool isKeyDown );
		virtual	void mouseWhellProc( LORD::i32 parma );
		virtual void mouseProc( LORD::i32 xpos, LORD::i32 ypos );
		virtual void mouseLBProc( LORD::i32 xpos, LORD::i32 ypos );
		virtual void mouseMoveProc( LORD::i32 xpos, LORD::i32 ypos );

//		void OnCollideHit( const hknpEventHandlerInput& input, const hknpEvent& event );

	private:
		void	updateCamera( const LORD::ui32 elapse );
		void	cameraZoom( const LORD::Box& box, float scale );
		void	cameraZoom( float zValue );
		void	setCameraMoveDir( const LORD::Vector3& dir );
		void	rotationCamera( float xValue, float yValue );
		void	addScreenOffset( float xOffset, float yOffset );

		void	throwBox( );
	private:
		LORD::Scene*	m_scene;

		LORD::Camera*		m_camera;			// �������
		float				m_cameraRadius;		// ������۲�뾶
		LORD::Vector3		m_cameraLookAt;		// ������۲��
		LORD::Vector3		m_cameraMoveDir;	// ������ƶ�
		LORD::Vector3		m_cameraForward;	// ����
		LORD::Vector3		m_cameraPositon;	// �����λ��
		float				m_horizonAngle;		// ˮƽ��ת�Ƕ�(˳ʱ��)
		float				m_verticleAngle;	// ��ֱ��ת�Ƕ�(˳ʱ��)
		float				m_xOffset;			// X����ƫ��
		float				m_yOffset;			// Y����ƫ��
		LORD::Vector2		m_mousePos;

		LORD::QueryObject*	m_box;
		LORD::SceneNode*	m_boxNode;
		LORD::QueryObject*	m_ground;
		LORD::SceneNode*	m_groundNode;

	};
}

#endif
