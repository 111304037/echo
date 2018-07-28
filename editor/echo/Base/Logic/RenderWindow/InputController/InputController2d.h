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
	class InputController2d : public IRWInputController
	{
	public:
		InputController2d();
		virtual ~InputController2d();

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

		// ��Ӧģ��
		virtual void CameraZoom(const Echo::AABB& box, float scale);

		void SetCameraPosition(const Echo::Vector3& pos) { m_cameraPositon = pos; }
		const Echo::Vector3& GetCameraPosition() const { return m_cameraPositon; }

		void SetCameraForward(const Echo::Vector3& forward) { m_cameraForward = forward; }
		const Echo::Vector3& GetCameraForward() const { return m_cameraForward; }

		void SetCameraMoveDirVal(const Echo::Vector3& dir) { m_cameraMoveDir = dir; }
		const Echo::Vector3& GetCameraMoveDirVal() const { return m_cameraMoveDir; }

		void UpdateCameraInfo();

		bool isCameraMoving() const;

		// �����Ƿ���������
		void setNeedUpdateCamera(bool need) { m_bNeedUpdateCamera = need; UpdateCamera(0.01f); }

		void resetPerspectiveCamera();

	protected:
		// ��ʼ�����������
		void InitializeCameraSettings(float offsetdir = 5);

		// ���������
		void UpdateCamera(float elapsedTime);

		// ���������
		void CameraZoom(float zValue);

		// λ�������
		void MoveCamera(float xValue, float yValue);

		// ���������Ӧ
		void AdaptCamera();

		// ��ȡ�����
		Echo::Camera* GetCamera() { return m_camera; }

	protected:
		void updateMouseButtonPressedStatus(QMouseEvent* e, bool pressed);
		void updateKeyPressedStatus(QKeyEvent* e, bool pressed);

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
		float				m_cameraScale;		// ����ֵ
		Echo::Vector3		m_cameraMoveDir;	// ������ƶ�
		Echo::Vector3		m_cameraForward;	// ����
		Echo::Vector3		m_cameraPositon;	// �����λ��

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
	};
}
