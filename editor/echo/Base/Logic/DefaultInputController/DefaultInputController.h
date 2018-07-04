#pragma once

#include "IRenderWindowInputController.h"
#include <QPoint>
#include "engine/core/Math/Vector3.h"
#include "engine/core/camera/Camera.h"

class QWheelEvent;
class QMouseEvent;
class QKeyEvent;

namespace Studio
{
	/**
	* ��Ⱦ����Ĭ�Ͽ�����
	*/
	class DefaultInputController : public IRWInputController
	{
	public:
		enum OrthoCamMode
		{
			OCM_TOP,
			OCM_FRONT,
			OCM_LEFT,
			OCM_NONE,
		};

		DefaultInputController();
		virtual ~DefaultInputController();

		// ÿ֡����
		virtual void tick(const InputContext& ctx) override;

		// �������¼�
		virtual void wheelEvent(QWheelEvent* e) override;

		// ����ƶ��¼�
		virtual void mouseMoveEvent(QMouseEvent* e) override;

		// ��갴���¼�
		virtual void mousePressEvent(QMouseEvent* e) override;

		// ���˫���¼�
		virtual void mouseDoubleClickEvent(QMouseEvent* e) override {}

		// ����ͷ��¼�
		virtual void mouseReleaseEvent(QMouseEvent* e) override;

		// ��갴���¼�
		virtual void keyPressEvent(QKeyEvent* e) override;

		// ���̧���¼�
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

		// �ֶ���ʼ�����������
		virtual void onInitCameraSettings(float offsetdir) override;

		// ��Ӧģ��
		virtual void CameraZoom(const Echo::AABB& box, float scale);

		// ����������۲��
		void SetCameraLookAt(const Echo::Vector3& lookAt) { m_cameraLookAt = lookAt; }
		const Echo::Vector3& GetCameraLookAt() const { return m_cameraLookAt; }

		void SetCameraPosition(const Echo::Vector3& pos) { m_cameraPositon = pos; }
		const Echo::Vector3& GetCameraPosition() const { return m_cameraPositon; }

		void SetCameraForward(const Echo::Vector3& forward) { m_cameraForward = forward; }
		const Echo::Vector3& GetCameraForward() const { return m_cameraForward; }

		void SetCameraMoveDirVal(const Echo::Vector3& dir) { m_cameraMoveDir = dir; }
		const Echo::Vector3& GetCameraMoveDirVal() const { return m_cameraMoveDir; }

		void SetVerticleAngle(const float angle) { m_verticleAngle = angle; }
		const float GetVerticleAngle() const { return m_verticleAngle; }

		void SetHorizonAngle(const float angle) { m_horizonAngle = angle; }
		const float GetHorizonAngle() const { return m_horizonAngle; }

		void SetVerticleAngleGoal(const float angle) { m_verticleAngleGoal = angle; }
		const float GetVerticleAngleGoal() const { return m_verticleAngleGoal; }

		void SetHorizonAngleGoal(const float angle) { m_horizonAngleGoal = angle; }
		const float GetHorizonAngleGoal() const { return m_horizonAngleGoal; }

		void SetXOffset(const float offset) { m_xOffset = offset; }
		const float GetXOffset() const { return m_xOffset; }

		void SetYOffset(const float offset) { m_yOffset = offset; }
		const float GetYOffset() const { return m_yOffset; }

		// ��ȡ������뾶��������������Ǿ��룩
		float GetCameraRadius() { return m_cameraRadius; }

		// ����������뾶
		void SetCameraRadius(float radius) { m_cameraRadius = Echo::Math::Clamp(radius, 0.1f, 1000.f); }

		void UpdateCameraInfo();

		bool isCameraMoving() const;

		// �����Ƿ���������
		void setNeedUpdateCamera(bool need) { m_bNeedUpdateCamera = need; UpdateCamera(0.01f); }

		void resetPerspectiveCamera();
		void switchToOrthoCam(OrthoCamMode destMode, Echo::Vector3 pos);
		void setOrthoCamDis(OrthoCamMode mode, float dis);
		float getOrthoCamDis(OrthoCamMode mode);
		void updateOrthoCamPos(OrthoCamMode mode);

	protected:
		// ��ʼ�����������
		void InitializeCameraSettings(float offsetdir = 5);

		// ���������
		void UpdateCamera(float elapsedTime);

		// ƽ�������
		void SetCameraMoveDir(const Echo::Vector3& dir);

		void SetCameraMoveDir(const Echo::Vector3& dir, Echo::Vector3 forward);

		// ���������
		void CameraZoom(float zValue);

		// ��ת�����
		void RotationCamera(float xValue, float yValue);

		// �����Ļƫ��
		void AddScreenOffset(float xOffset, float yOffset);

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
		float				m_xOffset;			// X����ƫ��
		float				m_yOffset;			// Y����ƫ��

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
