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

#ifndef _DEMETER_EXCEPTION_DEMETER_H_
#define _DEMETER_EXCEPTION_DEMETER_H_

#include <string>
#include <iostream>

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief An exception that is thrown by Demeter objects to 
	/// indicate fatal errors that your application should handle.
	class DemeterException
	{
	      public:
		DemeterException(char *szErrorMessage)
		{
			m_ErrorMessage = "TERRAIN ERROR: ";
			m_ErrorMessage.append(szErrorMessage);
		}

		DemeterException(std::string & errorMessage)
		{
			m_ErrorMessage = "TERRAIN ERROR: ";
			m_ErrorMessage.append(errorMessage);
		}

		 ~DemeterException()
		{
		}

		/// \brief Gets a human readable string describing the error condition.
		const char *GetErrorMessage()
		{
			return m_ErrorMessage.c_str();
		}
	      private:
		std::string m_ErrorMessage;
	};
}

#endif
