#pragma once

#include "IInputController.h"
#include <QPoint>
#include "engine/core/Math/Vector3.h"
#include "engine/core/camera/Camera.h"

class QWheelEvent;
class QMouseEvent;
class QKeyEvent;

namespace Studio
{
	class InputController3d : public IRWInputController
	{
	public:
		enum OrthoCamMode
		{
			OCM_TOP,
			OCM_FRONT,
			OCM_LEFT,
			OCM_NONE,
		};

		InputController3d();
		virtual ~InputController3d();

		// ÿ֡����
		virtual void tick(const InputContext& ctx) override;

		// events
		virtual void wheelEvent(QWheelEvent* e) override;
		virtual void mouseMoveEvent(QMouseEvent* e) override;
		virtual void mousePressEvent(QMouseEvent* e) override;
		virtual void mouseDoubleClickEvent(QMouseEvent* e) override {}
		virtual void mouseReleaseEvent(QMouseEvent* e) override;
		virtual void keyPressEvent(QKeyEvent* e) override;
		virtual void keyReleaseEvent(QKeyEvent* e) override;

		// ��갴��
		virtual Qt::MouseButton pressedMouseButton() override;

		// ���λ��
		virtual QPointF mousePosition() override;

		// �����������ģʽ
		virtual void SetCameraOperateMode(int mode) override;

		// ���ص�ǰ�������ģʽ
		virtual int GetCameraOperateMode() override;

		// �������size
		virtual void onSizeCamera(unsigned int width, unsigned int height) override;

		// ���������
		virtual void onAdaptCamera() override;

		// ��Ӧģ��
		virtual void CameraZoom(const Echo::AABB& box, float scale);

		// ��ȡ������뾶��������������Ǿ��룩
		float GetCameraRadius() { return m_cameraRadius; }

		// ����������뾶
		void SetCameraRadius(float radius) { m_cameraRadius = Echo::Math::Clamp(radius, 0.1f, 1000.f); }

		bool isCameraMoving() const;

		// �����Ƿ���������
		void setNeedUpdateCamera(bool need) { m_bNeedUpdateCamera = need; UpdateCamera(0.01f); }

		void resetPerspectiveCamera();
		void switchToOrthoCam(OrthoCamMode destMode, Echo::Vector3 pos);
		void setOrthoCamDis(OrthoCamMode mode, float dis);
		float getOrthoCamDis(OrthoCamMode mode);
		void updateOrthoCamPos(OrthoCamMode mode);

	protected:
		// ���������
		void UpdateCamera(float elapsedTime);

		// ƽ�������
		void SetCameraMoveDir(const Echo::Vector3& dir);

		void SetCameraMoveDir(const Echo::Vector3& dir, Echo::Vector3 forward);

		// ���������
		void CameraZoom(float zValue);

		// ��ת�����
		void RotationCamera(float xValue, float yValue);

		// ���������Ӧ
		void AdaptCamera();

		// ��ȡ�����
		Echo::Camera* GetCamera() { return m_camera; }

		// ��ת�����(ƽ������)
		void SmoothRotation(float elapsedTime);
		void UpdateOrthoCamModeWH(OrthoCamMode mode);

	protected:
		void updateMouseButtonPressedStatus(QMouseEvent* e, bool pressed);
		void updateKeyPressedStatus(QKeyEvent* e, bool pressed);

		void rotateCameraAtPos(float xValue, float yValue, const Echo::Vector3& rotCenter);

	protected:
		// on open node tree
		virtual void onOpenNodeTree(const Echo::String& resPath) override;

		// on save node tree
		virtual void onSaveConfig() override;

	protected:
		bool m_mouseLButtonPressed;
		bool m_mouseMButtonPressed;
		bool m_mouseRButtonPressed;
		bool m_keyADown;
		bool m_keyWDown;
		bool m_keySDown;
		bool m_keyDDown;
		bool m_keyQDown;
		bool m_keyEDown;
		bool m_keyAltDown;
		bool m_keyCtrlDown;
		bool m_keyShiftDown;
		QPointF m_pos;
		int m_cameraOperateMode; //1��ʾ����,-1��ʾ������ת�ߵ�

	protected:
		Echo::Camera*		m_camera;			// �������
		float				m_cameraRadius;		// ������۲�뾶
		Echo::Vector3		m_cameraLookAt;		// ������۲��
		Echo::Vector3		m_cameraMoveDir;	// ������ƶ�
		Echo::Vector3		m_cameraForward;	// ����
		Echo::Vector3		m_cameraPositon;	// �����λ��
		float				m_horizonAngle;		// ˮƽ��ת�Ƕ�(˳ʱ��)
		float				m_verticleAngle;	// ��ֱ��ת�Ƕ�(˳ʱ��)
		float				m_horizonAngleGoal;	// Ŀ��ˮƽ��ת�Ƕ�
		float				m_verticleAngleGoal;// Ŀ�괹ֱ��ת�Ƕ�

		bool				m_bNeedUpdateCamera;// �Ƿ���Ҫ�������λ��

		Echo::Vector3		m_backCameraPos;
		Echo::Vector3		m_backCameraRot;

		float				m_orthoTopDis;
		Echo::Vector3		m_orthoTopCamPos;
		Echo::Vector3		m_orthoTopCamRot;

		float				m_orthoFrontDis;
		Echo::Vector3		m_orthoFrontCamPos;
		Echo::Vector3		m_orthoFrontCamRot;

		float				m_orthoLeftDis;
		Echo::Vector3		m_orthoLeftCamPos;
		Echo::Vector3		m_orthoLeftCamRot;

		OrthoCamMode		m_preMode;
		OrthoCamMode		m_curMode;
	};
}
