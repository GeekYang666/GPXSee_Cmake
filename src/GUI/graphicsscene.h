#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "tooltip.h"

class GraphicsItem : public QGraphicsItem
{
public:
	GraphicsItem(QGraphicsItem *parent = 0) : QGraphicsItem(parent) {}

	virtual ToolTip info(bool extended) const = 0;
	int type() const {return QGraphicsItem::UserType + 1;}

	static void useStyle(bool use) {_useStyle = use;}

protected:
	static bool _useStyle;
};

class GraphicsScene : public QGraphicsScene
{
public:
	GraphicsScene(QObject *parent = 0)
	  : QGraphicsScene(parent), _showExtendedInfo(false) {}

	bool showExtendedInfo() const {return _showExtendedInfo;}
	void showExtendedInfo(bool show) {_showExtendedInfo = show;}

public slots:
	void clear();

protected:
	void helpEvent(QGraphicsSceneHelpEvent *event);

private:
	bool _showExtendedInfo;

	QList<QGraphicsItem *> itemsAtPosition(const QPoint &screenPos,
	  const QPointF &scenePos, QWidget *widget) const;
};

#endif // GRAPHICSSCENE_H
