// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

// $ID$

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

#ifndef _DEMETER_DRAWABLE_
#define _DEMETER_DRAWABLE_

#include <osg/BoundingBox>
#include <osg/StateSet>
#include <osg/State>
#include <osg/Drawable>
#include "Demeter/Terrain.h"

#ifdef _WIN32
#ifdef DEMETEROSG_EXPORTS
#define DEMETEROSG_API __declspec(dllexport)
#else
#define DEMETEROSG_API __declspec(dllimport)
#endif
#else
#define DEMETEROSG_API
#endif

/// \brief Demeter Namespace brief description.
/// Demeter Namepsace detailed description.  With
/// lots of awesome details.
namespace Demeter
{
	/// \brief DemeterDrawable class brief description.
	/// DemeterDrawable class detailed description.
	class DEMETEROSG_API DemeterDrawable:public osg::Drawable
	{
	      public:

		DemeterDrawable();
		DemeterDrawable(const DemeterDrawable &, const osg::CopyOp & copyop = osg::CopyOp::SHALLOW_COPY);

		  virtual ~ DemeterDrawable();

		  DemeterDrawable & operator =(const DemeterDrawable &);

		virtual osg::Object * cloneType() const;
		virtual osg::Object * clone(const osg::CopyOp & copyop) const;
		virtual bool isSameKindAs(const osg::Object * obj) const;

		virtual const char *className() const;
		virtual const char *libraryName() const;

		void SetTerrain(Terrain * pTerrain);
		Terrain *GetTerrain()
		{
			return m_RefTerrain.get();
		}
		const Terrain *GetTerrain() const
		{
			return m_RefTerrain.get();
		}

		virtual void drawImplementation(osg::State &) const;
	      protected:

		  virtual bool computeBound() const;
		  osg::ref_ptr < Terrain > m_RefTerrain;
	};
};

#endif
