#include <QLocale>
#include "data/data.h"
#include "cadencegraphitem.h"
#include "cadencegraph.h"


CadenceGraph::CadenceGraph(QWidget *parent) : GraphTab(parent)
{
	_showTracks = false;

	GraphView::setYUnits(tr("rpm"));
	setYLabel(tr("Cadence"));

	setSliderPrecision(1);
}

CadenceGraph::~CadenceGraph()
{
	qDeleteAll(_tracks);
}

void CadenceGraph::setInfo()
{
	if (_showTracks) {
		QLocale l(QLocale::system());

#ifdef Q_OS_ANDROID
		GraphView::addInfo(tr("Avg"), l.toString(avg() * yScale() + yOffset(),
		  'f', 1) + UNIT_SPACE + yUnits());
		GraphView::addInfo(tr("Max"), l.toString(max() * yScale() + yOffset(),
		  'f', 1) + UNIT_SPACE + yUnits());
#else // Q_OS_ANDROID
		GraphView::addInfo(tr("Average"), l.toString(avg() * yScale()
		  + yOffset(), 'f', 1) + UNIT_SPACE + yUnits());
		GraphView::addInfo(tr("Maximum"), l.toString(max() * yScale()
		  + yOffset(), 'f', 1) + UNIT_SPACE + yUnits());
#endif // Q_OS_ANDROID
	} else
		clearInfo();
}

QList<GraphItem*> CadenceGraph::loadData(const Data &data, Map *map)
{
	Q_UNUSED(map);
	QList<GraphItem*> graphs;

	for (int i = 0; i < data.tracks().count(); i++) {
		const Track &track = data.tracks().at(i);
		const Graph &graph = track.cadence();

		if (graph.isEmpty()) {
			_palette.nextColor();
			graphs.append(0);
		} else {
			CadenceGraphItem *gi = new CadenceGraphItem(graph, _graphType,
			  _width, _palette.nextColor());

			_tracks.append(gi);
			if (_showTracks)
				addGraph(gi);

			_avg.append(QPointF(track.distance(), gi->avg()));
			graphs.append(gi);
		}
	}

	for (int i = 0; i < data.routes().count(); i++) {
		_palette.nextColor();
		graphs.append(0);
	}

	for (int i = 0; i < data.areas().count(); i++)
		_palette.nextColor();

	setInfo();
	redraw();

	return graphs;
}

qreal CadenceGraph::avg() const
{
	qreal sum = 0, w = 0;

	for (int i = 0; i < _avg.size(); i++) {
		const QPointF &p = _avg.at(i);
		sum += p.y() * p.x();
		w += p.x();
	}

	return (sum / w);
}

void CadenceGraph::clear()
{
	qDeleteAll(_tracks);
	_tracks.clear();

	_avg.clear();

	GraphTab::clear();
}

void CadenceGraph::showTracks(bool show)
{
	_showTracks = show;

	for (int i = 0; i < _tracks.size(); i++) {
		if (show)
			addGraph(_tracks.at(i));
		else
			removeGraph(_tracks.at(i));
	}

	setInfo();

	redraw();
}
