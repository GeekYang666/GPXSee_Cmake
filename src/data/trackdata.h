#ifndef TRACKDATA_H
#define TRACKDATA_H

#include <QList>
#include <QVector>
#include <QString>
#include "trackpoint.h"
#include "link.h"
#include "style.h"

typedef QVector<Trackpoint> SegmentData;

class TrackData : public QList<SegmentData>
{
public:
	TrackData() {}
	TrackData(const SegmentData &segment)
	{
		reserve(1);
		append(segment);
	}

	const QString &name() const {return _name;}
	const QString &description() const {return _desc;}
	const QString &comment() const {return _comment;}
	const QVector<Link> &links() const {return _links;}
	const LineStyle &style() const {return _style;}
	const QString &file() const {return _file;}

	void setName(const QString &name) {_name = name;}
	void setDescription(const QString &desc) {_desc = desc;}
	void setComment(const QString &comment) {_comment = comment;}
	void addLink(const Link &link) {_links.append(link);}
	void setStyle(const LineStyle &style) {_style = style;}
	void setFile(const QString &path) {_file = path;}

private:
	QString _name;
	QString _desc;
	QString _comment;
	QString _file;
	QVector<Link> _links;
	LineStyle _style;
};

#endif // TRACKDATA_H
