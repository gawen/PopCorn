// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#include "Demeter/DemeterDrawable.h"

using namespace Demeter;
using namespace osg;

DemeterDrawable::DemeterDrawable()
{
	// force the support of display list off for this Drawable
	// since it is not appropriate for dynamic geometry.
	setSupportsDisplayList(false);
}

DemeterDrawable::DemeterDrawable(const DemeterDrawable & other, const CopyOp & copyop):Drawable(other, copyop), m_RefTerrain(other.m_RefTerrain)	// assume shallow copy.
{
}

DemeterDrawable::~DemeterDrawable()
{
}

DemeterDrawable & DemeterDrawable::operator =(const DemeterDrawable & other)
{
	if (&other == this)
		return *this;	// don't copy if assigning to self. 
	m_RefTerrain = other.m_RefTerrain;	// Increments ref count
	return *this;
}

bool DemeterDrawable::isSameKindAs(const Object * obj) const
{
	return dynamic_cast < const DemeterDrawable *>(obj) != NULL;
}

Object *DemeterDrawable::cloneType() const
{
	// return a clone of this type of drawable.
	return new DemeterDrawable();
}

Object *DemeterDrawable::clone(const osg::CopyOp & copyop) const
{
	// return a clone of this object, via the copy constructor.
	return new DemeterDrawable(*this, copyop);
}

void DemeterDrawable::SetTerrain(Terrain * pTerrain)
{
	m_RefTerrain = pTerrain;
}

const char *DemeterDrawable::className() const
{
	return "DemeterDrawable";
}

const char *DemeterDrawable::libraryName() const
{
	return "Demeter";
}

void DemeterDrawable::drawImplementation(State & state) const
{
	if (m_RefTerrain.valid())
	{
		// we could get the modelview, projection and viewport from the state object here
		// and avoid the expensive calls to glGet which exist in Terrain.

		// disable the active vertex arrays to prevent state overflowing into terrain drawing,
		state.disableAllVertexArrays();

		const_cast < Terrain * >(m_RefTerrain.get())->ModelViewMatrixChanged();
		const_cast < Terrain * >(m_RefTerrain.get())->Render();

		// dirty all the state modified by Demeter so that OSG can keep the lazy state updating in sync.
		state.dirtyVertexPointer();
		state.dirtyNormalPointer();
		state.dirtyTexCoordPointer(0);
		state.dirtyTexCoordPointer(1);

		state.haveAppliedAttribute(osg::StateAttribute::DEPTH);
		state.haveAppliedAttribute(osg::StateAttribute::BLENDFUNC);
		state.haveAppliedAttribute(osg::StateAttribute::ALPHAFUNC);
		state.haveAppliedAttribute(osg::StateAttribute::MATERIAL);
		state.haveAppliedAttribute(osg::StateAttribute::FRONTFACE);

		state.haveAppliedTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OFF);
		state.haveAppliedTextureAttribute(0, osg::StateAttribute::TEXTURE);
		state.haveAppliedTextureAttribute(0, osg::StateAttribute::TEXENV);

		//state.haveAppliedTextureMode(1,GL_TEXTURE_2D,osg::StateAttribute::OFF);
		//state.haveAppliedTextureAttribute(1,osg::StateAttribute::TEXTURE);
		//state.haveAppliedTextureAttribute(1,osg::StateAttribute::TEXENV);
	}
}

bool DemeterDrawable::computeBound() const
{
	if (m_RefTerrain.valid())
	{
		_bbox._min.x() = _bbox._min.y() = _bbox._min.z() = 0.0f;
		_bbox._max.x() = m_RefTerrain->GetWidth();
		_bbox._max.y() = m_RefTerrain->GetHeight();
		_bbox._max.z() = m_RefTerrain->GetMaxElevation();
		_bbox_computed = true;
	}
	else
	{
		_bbox.init();
	}
	return true;
}
