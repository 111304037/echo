#pragma once

#include <Engine/Core.h>
#include <QPainter>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

using namespace std;

namespace QT_UI
{
	//-------------------------------------------
	// ColorSelect ��ɫѡ��ť 2013-3-4 ����
	//-------------------------------------------
	class QColorSelect : public QPushButton
	{
		Q_OBJECT
	public:
		QColorSelect( QWidget* parent = 0);

		// ������ɫ
		void SetColor( string color);

		// ������ɫ
		void SetColor( const Echo::Color& color);

		// ��ȡ��ɫ
		string GetColor();

		// ��ȡ��ɫ
		const Echo::Color& GetColor() const { return m_color; }

		// MVC��Ⱦ
		static void ItemDelegatePaint( QPainter *painter, const QRect& rect, const string& val);

	protected:
		// �ض�����Ⱦ
		void paintEvent( QPaintEvent* event); 

		// ��С
		QSize sizeHint() const;

	public slots:
		// ѡ����ɫ
		void OnSelectColor();

	private:
		Echo::Color		m_color;
	};
}