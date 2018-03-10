#pragma once

#include <qnamespace.h>

class QString;
class QPoint;

namespace Studio
{
	/**
	 * TimelinePanel�ӿ�
	 */
	class ITimelinePanelEvent
	{
	public:
		enum Button
		{
			PlayButton,
			PauseButton,
			ResumeButton,
			StopButton,
			StepButton
		};
		
		virtual ~ITimelinePanelEvent() {}

		// ��ť���
		virtual void onButtonClicked(ITimelinePanelEvent::Button button, const QString& skillName) {}
		virtual void onEventButtonMoved(int row, int index, float tick) {}
		virtual void onEventButtonDoubleClicked(int row, int index) {}
		virtual void onEventButtonClicked(int row, int index, int subIndex){}
		virtual void onFrameMoveto(float tick) {}
		virtual void onItemClicked(int row) {}
		virtual void onItemChecked(int row, Qt::CheckState state) {}
		virtual void onCustomContextMenuRequested(int row) {}
		virtual void onCustomContextMenuRequested(int row, int column) {}
	};
}
