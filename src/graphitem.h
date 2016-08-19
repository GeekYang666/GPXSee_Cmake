#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QGraphicsPathItem>

class GraphItem : public QGraphicsPathItem
{
public:
	GraphItem(const QPainterPath &path, QGraphicsItem * parent = 0)
	  : QGraphicsPathItem(path, parent) {_id = 0;}

	int id() const {return _id;}
	void setId(int id) {_id = id;}
	void setColor(const QColor &color);

private:
	int _id;
};

#endif // GRAPHITEM_H
