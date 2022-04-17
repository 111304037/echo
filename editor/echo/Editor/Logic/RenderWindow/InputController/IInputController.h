#pragma once

#include <QPoint>
#include "engine/core/geom/AABB.h"
#include <engine/core/scene/node.h>

class QWheelEvent;
class QMouseEvent;
class QKeyEvent;

namespace Studio
{
	struct InputContext
	{
		InputContext()
			: pressedButton(Qt::NoButton)
			, elapsedTime(0.f)
		{}

		InputContext(const QPointF& p, Qt::MouseButton button, float elapsed)
			: pos(p)
			, pressedButton(button)
			, elapsedTime(elapsed)
		{}

		QPointF pos;
		Qt::MouseButton pressedButton;
		float elapsedTime;
	};

	class IRenderWindowInputController
	{
	public:
		virtual ~IRenderWindowInputController() {}

		// ÿ֡����
		virtual void tick(const InputContext& ctx) = 0;

		// �������¼�
		virtual void wheelEvent(QWheelEvent* e) = 0;

		// ����ƶ��¼�
		virtual void mouseMoveEvent(QMouseEvent* e) = 0;

		// ��갴���¼�
		virtual void mousePressEvent(QMouseEvent* e) = 0;

		// ���˫���¼�
		virtual void mouseDoubleClickEvent(QMouseEvent* e) = 0; 

		// ����ͷ��¼�
		virtual void mouseReleaseEvent(QMouseEvent* e) = 0;

		// ��갴���¼�
		virtual void keyPressEvent(QKeyEvent* e) = 0;

		// ���̧���¼�
		virtual void keyReleaseEvent(QKeyEvent* e) = 0;

		// ��갴��
		virtual Qt::MouseButton pressedMouseButton() = 0;

		// ���λ��
		virtual QPointF mousePosition() = 0;

		// �������size
		virtual void onSizeCamera(unsigned int width, unsigned int height) = 0;

		// �������Ӧ
		virtual void onAdaptCamera() {}

		// on focuse node
		virtual void onFocusNode(Echo::Node* node)=0;

	public:
		// on open node tree
		virtual void onOpenNodeTree(const Echo::String& resPath) {}

		// on save node tree
		virtual void onSaveConfig() {}
	};
	typedef IRenderWindowInputController IRWInputController;
}