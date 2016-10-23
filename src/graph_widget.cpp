#include "graph_widget.h"

#include <cassert>

#include <QHBoxLayout>
#include <QGLWidget>
#include <QResizeEvent>

#include "connected_edge.h"

GraphWidget::GraphWidget(QWidget* parent) : QGraphicsView(parent), m_scene(this) {
	m_scene.setSceneRect(-width() / 2, -height() / 2, width(), height());

	setScene(&m_scene);
	setDragMode(QGraphicsView::RubberBandDrag);
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setBackgroundBrush(Qt::black);
}

GraphScene& GraphWidget::scene() {
	return m_scene;
}

void GraphWidget::resizeEvent(QResizeEvent* event) {
	const QPointF mid = m_scene.sceneRect().center();

	m_scene.setSceneRect(mid.x() - event->size().width() / 2,
	                      mid.y() - event->size().height() / 2,
	                      event->size().width(),
	                      event->size().height());

	QGraphicsView::resizeEvent(event);
}

void GraphWidget::mousePressEvent(QMouseEvent* event) {
	if(event->button() == Qt::RightButton) {
		if(m_contextMenuCallback)
			m_contextMenuCallback(event->globalPos());
	}
	else
		QGraphicsView::mousePressEvent(event);
}

void GraphWidget::setContextMenuCallback(std::function<void(QPoint)> fn) {
	m_contextMenuCallback = fn;
}

void GraphWidget::setKeyPressCallback(std::function<void(const QKeyEvent&)> fn) {
	m_keyPressCallback = fn;
}

void GraphWidget::keyPressEvent(QKeyEvent* event) {
	if(m_keyPressCallback)
		m_keyPressCallback(*event);
}
