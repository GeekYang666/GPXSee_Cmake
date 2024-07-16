#include <cmath>
#include <QLineF>
#include "dem.h"
#include "map.h"


#define SAMPLES 100
#define DELTA 1e-6

static void growLeft(const Coordinates &c, RectC &rect)
{
	if (c.lon() < rect.left())
		rect.setLeft(c.lon());
}

static void growRight(const Coordinates &c, RectC &rect)
{
	if (c.lon() > rect.right())
		rect.setRight(c.lon());
}

static void growTop(const Coordinates &c, RectC &rect)
{
	if (c.lat() > rect.top())
		rect.setTop(c.lat());
}

static void growBottom(const Coordinates &c, RectC &rect)
{
	if (c.lat() < rect.bottom())
		rect.setBottom(c.lat());
}

RectC Map::llBounds()
{
	QRectF b(bounds());
	double dx = b.width() / SAMPLES;
	double dy = b.height() / SAMPLES;

	Coordinates tl(xy2ll(b.topLeft()));
	Coordinates br(xy2ll(b.bottomRight()));
	RectC rect(tl, br);

	for (int i = 0; i <= SAMPLES; i++) {
		double x = b.left() + i * dx;
		growBottom(xy2ll(QPointF(x, b.bottom())), rect);
		growTop(xy2ll(QPointF(x, b.top())), rect);
	}

	for (int i = 0; i <= SAMPLES; i++) {
		double y = b.top() + i * dy;
		growLeft(xy2ll(QPointF(b.left(), y)), rect);
		growRight(xy2ll(QPointF(b.right(), y)), rect);
	}

	if (rect.right() <= -180.0 + DELTA)
		rect.setRight(180.0);
	if (rect.left() >= 180.0 - DELTA)
		rect.setLeft(-180.0);

	return rect;
}

qreal Map::resolution(const QRectF &rect)
{
	/* The haversine formula used in Coordinates::distanceTo() only gives
	   "half world" distances and shorter so we have to use only the center
	   "half rectangle" in case e.g. world map bounds are on the input */
	QRectF halfRect(QPointF(rect.left() + (rect.width() / 4.0),
	  rect.top() + (rect.height() / 4.0)),
	  QPointF(rect.right() - (rect.width() / 4.0),
	  rect.bottom() - (rect.height() / 4.0)));

	qreal cy = halfRect.center().y();
	QPointF cl(halfRect.left(), cy);
	QPointF cr(halfRect.right(), cy);

	qreal ds = xy2ll(cl).distanceTo(xy2ll(cr));
	qreal ps = QLineF(cl, cr).length();

	return ds/ps;
}

double Map::elevation(const Coordinates &c)
{
	double ele;

	DEM::lock();
	ele = DEM::elevation(c);
	DEM::unlock();

	return ele;
}
