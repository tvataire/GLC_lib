/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 *****************************************************************************/

//! \file glc_repflymover.cpp Implementation of the GLC_RepFlyMover class.

#include "glc_repflymover.h"
#include "glc_viewport.h"
#include "../geometry/glc_polylines.h"
#include "../glc_context.h"

#include <QFontMetrics>

GLC_RepFlyMover::GLC_RepFlyMover(GLC_Viewport* pViewport)
: GLC_RepMover(pViewport)
, m_Radius(0.06)
, m_CenterCircle()
, m_Plane()
, m_Hud()
, m_HudOffset(m_Radius * 5.0, m_Radius * 5.3)
{

	createRepresentation();
}

GLC_RepFlyMover::GLC_RepFlyMover(const GLC_RepFlyMover& repFlyMover)
: GLC_RepMover(repFlyMover)
, m_Radius(repFlyMover.m_Radius)
, m_CenterCircle(repFlyMover.m_CenterCircle)
, m_Plane(repFlyMover.m_Plane)
, m_Hud(repFlyMover.m_Hud)
, m_HudOffset(repFlyMover.m_HudOffset)
{

}

GLC_RepFlyMover::~GLC_RepFlyMover()
{

}

GLC_RepMover* GLC_RepFlyMover::clone() const
{
	return new GLC_RepFlyMover(*this);
}

void GLC_RepFlyMover::update()
{
	Q_ASSERT(NULL != m_pRepMoverInfo);
	Q_ASSERT(!m_pRepMoverInfo->m_VectorInfo.isEmpty());
	Q_ASSERT(!m_pRepMoverInfo->m_DoubleInfo.isEmpty());

    GLC_Vector3d vector(m_pRepMoverInfo->m_VectorInfo.constFirst());

	// Rotation
	double deltaX= vector.x();
	double angle= - deltaX;
	GLC_Matrix4x4 rotation(glc::Z_AXIS, angle);

	// Translation
	vector.setX(vector.x() * m_Radius * 4.0);
	vector.setY(vector.y() * m_Radius * 4.0);
	GLC_Matrix4x4 translation(vector);

	m_Plane.setMatrix(translation * rotation);
}
void GLC_RepFlyMover::setMainColor(const QColor& color)
{
	GLC_RepMover::setMainColor(color);
	m_CenterCircle.geomAt(0)->setWireColor(color);
	m_Plane.geomAt(0)->setWireColor(color);
	m_Hud.geomAt(0)->setWireColor(color);
}

void GLC_RepFlyMover::setThickness(double thickness)
{
	GLC_RepMover::setThickness(thickness);
	m_CenterCircle.geomAt(0)->setLineWidth(thickness);
	m_Plane.geomAt(0)->setLineWidth(thickness);
	m_Hud.geomAt(0)->setLineWidth(thickness);
}

void GLC_RepFlyMover::glDraw()
{
	Q_ASSERT(NULL != m_pRepMoverInfo);
	Q_ASSERT(!m_pRepMoverInfo->m_DoubleInfo.isEmpty());

	// Get viewport informations
	const double calibre= 800.0;
	const double hRatio= static_cast<double>(m_pViewport->viewHSize()) / calibre;
	const double vRatio= static_cast<double>(m_pViewport->viewVSize()) / calibre;

	glDisable(GL_TEXTURE_2D);
    GLC_Context* pContext= GLC_ContextManager::instance()->currentContext();

    pContext->glcEnableLighting(false);
	glDisable(GL_DEPTH_TEST);

    pContext->glcMatrixMode(GL_PROJECTION);
    pContext->glcPushMatrix();
    pContext->glcLoadIdentity();
    pContext->glcOrtho(hRatio * -1.0 ,hRatio * 1.0 ,vRatio * -1.0 ,vRatio * 1.0 ,-1.0 ,1.0);
    pContext->glcMatrixMode(GL_MODELVIEW);
    pContext->glcPushMatrix();
    pContext->glcLoadIdentity();

	m_CenterCircle.render(glc::WireRenderFlag);
	m_Hud.render(glc::WireRenderFlag);
	m_Plane.render(glc::WireRenderFlag);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_CenterCircle.render(glc::TransparentRenderFlag);
	m_Hud.render(glc::TransparentRenderFlag);
	m_Plane.render(glc::TransparentRenderFlag);

	// Render velocity value + text
    QString velocity(QChar(' ') + QString::number(static_cast<int>(100.0 * m_pRepMoverInfo->m_DoubleInfo.constFirst())));
    QFont myFont;
    myFont.setBold(true);
    QFontMetrics fontmetrics(myFont);
    int txtHeight= fontmetrics.boundingRect(velocity).height();
    int textWidth= fontmetrics.horizontalAdvance(velocity);
    double posy= 2.0 * static_cast<double>(txtHeight) / calibre;
    double posx= 2.0 * static_cast<double>(textWidth / 2) / calibre + 0.02;
    m_pViewport->renderText(GLC_Point3d(- m_HudOffset.x() + posx, m_HudOffset.y() - posy, 0.0), velocity, m_MainColor, myFont);

    pContext->glcPopMatrix();
    pContext->glcMatrixMode(GL_PROJECTION);
    pContext->glcPopMatrix();
    pContext->glcMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
}

void GLC_RepFlyMover::createRepresentation()
{
	// HUD creation
	GLC_Circle* pCircle= new GLC_Circle(m_Radius);
	pCircle->setWireColor(GLC_RepMover::m_MainColor);
	pCircle->setLineWidth(GLC_RepMover::m_Thickness);
	m_CenterCircle.addGeometry(pCircle);

	GLC_Polylines* pPolylines= new GLC_Polylines();
	GLfloatVector  points;
	const double hudx= m_HudOffset.x();
	const double hudy= m_HudOffset.y();
	points << -hudx << -hudy << 0.0;
	points << -hudx << hudy << 0.0;
	pPolylines->addPolyline(points);
	points.clear();
	points << hudx << -hudy << 0.0;
	points << hudx << hudy << 0.0;
	pPolylines->addPolyline(points);
	pPolylines->setWireColor(GLC_RepMover::m_MainColor);
	pPolylines->setLineWidth(GLC_RepMover::m_Thickness);
	m_Hud.addGeometry(pPolylines);

	// Plane creation
	pPolylines= new GLC_Polylines();
	points.clear();
	const double l1= m_Radius * 1.5;
	points << (-m_Radius - l1) << -m_Radius << 0.0;
	points << -m_Radius << -m_Radius << 0.0;
	points << 0.0 << 0.0 << 0.0;
	points << m_Radius << -m_Radius << 0.0;
	points << (m_Radius + l1) << -m_Radius << 0.0;
	pPolylines->addPolyline(points);
	pPolylines->setWireColor(GLC_RepMover::m_MainColor);
	pPolylines->setLineWidth(GLC_RepMover::m_Thickness);
	m_Plane.addGeometry(pPolylines);
}
