//#pragma once
//
//#include <QSyntaxHighlighter>
//#include <QTextCharFormat>
//
//class QTextDocument;
//namespace Studio
//{
//	//-------------------------------
//	// Lua�߶ȱ༭ 2017-04-06
//	//-------------------------------
//	class LuaSyntaxHighLighter : public QSyntaxHighlighter
//	{
//		Q_OBJECT
//
//	public:
//		// ���캯��
//		LuaSyntaxHighLighter(QTextDocument* parent = 0);
//
//	protected:
//		// �߶�block
//		void  highlightBlock( const QString& text);
//
//	private:
//		// �߶ȹ���
//		struct HighlightingRule
//		{
//			QRegExp			pattern;
//			QTextCharFormat format;
//		};
//
//		QRegExp commentStartExpression;
//		QRegExp commentEndExpression;
//		QTextCharFormat classFormat;
//		QTextCharFormat singleLineCommentFormat;
//		QTextCharFormat multiLineCommentFormat;
//		QTextCharFormat quotationFormat;
//		QTextCharFormat functionFormat;
//		QTextCharFormat globalFormat;						// ȫ�ֱ���
//		QTextCharFormat	mainFunFormat;	
//
//		QVector<HighlightingRule>	m_highLightRules;
//		QTextCharFormat				m_keyWordFormat;		// �ؼ��ָ�ʽ
//	};
//}