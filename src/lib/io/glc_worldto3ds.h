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
//! \file glc_worldto3ds.h interface for the GLC_WorldTo3ds class.

#ifndef GLC_WORLDTO3DS_H_
#define GLC_WORLDTO3DS_H_

#include <QObject>
#include <QString>
#include <QMultiHash>

#include "../sceneGraph/glc_world.h"

#include "../glc_config.h"

struct Lib3dsFile;
struct Lib3dsMesh;
struct Lib3dsMaterial;
struct Lib3dsNode;

class GLC_StructReference;
class GLC_3DRep;
class GLC_Mesh;
class GLC_StructOccurrence;
class GLC_Matrix4x4;

//////////////////////////////////////////////////////////////////////
//! \class GLC_WorldTo3ds
/*! \brief GLC_WorldTo3ds : Export a GLC_World to a 3ds file */
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_WorldTo3ds : public QObject
{
	Q_OBJECT
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
    GLC_WorldTo3ds(const GLC_World& world, bool exportAll= true);
	virtual ~GLC_WorldTo3ds();
//@}

//////////////////////////////////////////////////////////////////////
/*! @name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Save the world to the specified file name
	bool exportToFile(const QString& fileName, bool useAbsolutePosition= false);

//@}

//////////////////////////////////////////////////////////////////////
/*! @name Private services functions */
//@{
//////////////////////////////////////////////////////////////////////
private:
	//! Save the world into the lib3ds file structure
	void saveWorld();

    QList<Lib3dsMesh*> saveMeshes(GLC_StructReference* pRef);

	//! Save the branch from the given GLC_StructOccurrence
	void saveBranch(GLC_StructOccurrence* pOcc);

	//! Create 3ds node from the given GLC_StructOccurrence
	void createNodeFromOccurrence(GLC_StructOccurrence* pOcc);

	//! Return the list of 3ds mesh from the given GLC_3DRep
	QList<Lib3dsMesh*> createMeshsFrom3DRep(GLC_3DRep* pRep, const QString& name, const GLC_Matrix4x4& matrix= GLC_Matrix4x4());

	//! Return the 3ds mesh from the given GLC_Mesh
	Lib3dsMesh* create3dsMeshFromGLC_Mesh(GLC_Mesh* pMesh, const QString& meshName);

	//! Return the 3ds material from the given GLC_Material
	Lib3dsMaterial* get3dsMaterialFromGLC_Material(GLC_Material* pMat);

	//! Create and return the 3ds material from the given GLC_Material and name
    Lib3dsMaterial* create3dsMaterialFromGLC_Material(GLC_Material* pMat);

    //! Return a 3ds compatible name
    QString to3dsName(const QString& name, int id) const;

	//! Set the object data position from the given matrix
	void setNodePosition(Lib3dsNode* pNode, const GLC_Matrix4x4& matrix);

//@}

//////////////////////////////////////////////////////////////////////
// Qt Signals
//////////////////////////////////////////////////////////////////////
signals:
	void currentQuantum(int);

//////////////////////////////////////////////////////////////////////
	/* Private members */
//////////////////////////////////////////////////////////////////////
private:
	//! The world to export
	GLC_World m_World;

	//! The Lib3dsFile Structure
	Lib3dsFile* m_pLib3dsFile;

	//! The file absolute path
	QString m_FileName;

	//! Reference to 3ds mesh hash table
    QMultiHash<GLC_StructReference*, Lib3dsMesh*> m_ReferenceToMesh;

    QHash<GLC_uint, Lib3dsMaterial*> m_WorldMaterialIdTo3dsMaterial;

	//! The root lib3ds node
	Lib3dsNode* m_pRootLib3dsNode;

	//! The current node id
    unsigned short m_CurrentNodeId;

	//! Occurrence id to node id hash
	QHash<GLC_uint, int> m_OccIdToNodeId;

    //! The current mesh index
	int m_CurrentMeshIndex;

    //! The current material index
    int m_CurrentMaterialIndex;

    //! The current Texture index
    int m_CurrentTextureIndex;

	//! Use absolute position (meshes are duplicated)
	bool m_UseAbsolutePosition;

	//! GLC_Texture to fileName hash table
    QHash<QString, QString> m_TextureToFileName;

    bool m_ExportAll;
};

#endif /* GLC_WORLDTO3DS_H_ */
