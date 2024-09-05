#ifndef KMLPARSER_H
#define KMLPARSER_H

#include <QXmlStreamReader>
#include <QDateTime>
#include <QDir>
#include "parser.h"

class KMLParser : public Parser
{
public:
	bool parse(QFile *file, QList<TrackData> &tracks, QList<RouteData> &routes,
	  QList<Area> &areas, QVector<Waypoint> &waypoints);
	QString errorString() const {return _reader.errorString();}
	int errorLine() const {return _reader.lineNumber();}

private:
	struct Ctx {
		Ctx(const QString &path, const QDir &dir, bool zip)
		  : path(path), dir(dir), zip(zip) {}

		QString path;
		QDir dir;
		bool zip;
	};

	typedef QMap<QString, PointStyle> PointStyleMap;
	typedef QMap<QString, PolygonStyle> PolygonStyleMap;
	typedef QMap<QString, LineStyle> LineStyleMap;

	void kml(const Ctx &ctx, QList<TrackData> &tracks, QList<Area> &areas,
	  QVector<Waypoint> &waypoints);
	void document(const Ctx &ctx, QList<TrackData> &tracks, QList<Area> &areas,
	  QVector<Waypoint> &waypoints);
	void folder(const Ctx &ctx, QList<TrackData> &tracks, QList<Area> &areas,
	  QVector<Waypoint> &waypoints, PointStyleMap &pointStyles,
	  PolygonStyleMap &polyStyles, LineStyleMap &lineStyles,
	  QMap<QString, QString> &map);
	void placemark(const Ctx &ctx, QList<TrackData> &tracks, QList<Area> &areas,
	  QVector<Waypoint> &waypoints, PointStyleMap &pointStyles,
	  PolygonStyleMap &polyStyles, LineStyleMap &lineStyles,
	  QMap<QString, QString> &map);
	void multiGeometry(const Ctx &ctx, QList<TrackData> &tracks,
	  QList<Area> &areas, QVector<Waypoint> &waypoints);
	void photoOverlay(const Ctx &ctx, QVector<Waypoint> &waypoints,
	  PointStyleMap &pointStyles, QMap<QString, QString> &map);
	void track(SegmentData &segment);
	void multiTrack(TrackData &t);
	void lineString(SegmentData &segment);
	void linearRing(QVector<Coordinates> &coordinates);
	void boundary(QVector<Coordinates> &coordinates);
	void polygon(Area &area);
	void point(Waypoint &waypoint);
	bool pointCoordinates(Waypoint &waypoint);
	bool lineCoordinates(SegmentData &segment);
	bool polygonCoordinates(QVector<Coordinates> &points);
	bool coord(Trackpoint &trackpoint);
	void extendedData(SegmentData &segment);
	void schemaData(SegmentData &segment);
	void heartRate(SegmentData &segment);
	void cadence(SegmentData &segment);
	void speed(SegmentData &segment);
	void temperature(SegmentData &segment);
	void power(SegmentData &segment);
	QDateTime timeStamp();
	qreal number();
	QDateTime time();
	QColor color();
	QString icon();
	QString styleUrl();
	void style(const QDir &dir, PointStyleMap &pointStyles,
	  PolygonStyleMap &polyStyles, LineStyleMap &lineStyles);
	void styleMapPair(const QString &id, QMap<QString, QString> &map);
	void styleMap(QMap<QString, QString> &map);
	void iconStyle(const QDir &dir, const QString &id, PointStyleMap &style);
	void polyStyle(const QString &id, PolygonStyleMap &styles);
	void lineStyle(const QString &id, LineStyleMap &styles);

	QXmlStreamReader _reader;
};

#endif // KMLPARSER_H
