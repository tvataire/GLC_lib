/*
 *  glc_qmlcamera.h
 *
 *  Created on: 02/05/2014
 *      Author: Laurent Ribon
 */

#ifndef GLC_QMLCAMERA_H
#define GLC_QMLCAMERA_H

#include <QObject>
#include <QPointer>
#include <QVector3D>

class GLC_Camera;

class GLC_QMLCamera : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVector3D upVector READ upVector WRITE setUpVector)
    Q_PROPERTY(QVector3D defaultUpVector READ defaultUpVector WRITE setDefaultUpVector)

public:
    explicit GLC_QMLCamera(QObject *parent = 0);

public:
    QVector3D upVector() const;
    QVector3D defaultUpVector() const;

public:
    void setCamera(GLC_Camera* pCamera);

signals:
    void changed();
    void updateView();

public slots:
    void setUpVector(const QVector3D& arg);
    void setDefaultUpVector(const QVector3D& arg);

private:
    QPointer<GLC_Camera> m_Camera;
};

#endif // GLC_QMLCAMERA_H
