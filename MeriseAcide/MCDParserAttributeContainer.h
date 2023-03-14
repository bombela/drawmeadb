/*
 * \file
 * \brief Attribute contener.
 * \author Copyright © 2008 Bombela <bombela@free.fr>
 * \date 2008
 *
 * This file is part of MeriseAcide.
 *
 * MeriseAcide is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * MeriseAcide is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MeriseAcide; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Purpose:
 *
 * $Id: MCDParserAttributeContainer.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDPARSERATTRIBUTECONTAINER_H_
#define _MCDPARSERATTRIBUTECONTAINER_H_

#include <string>
#include <list>

#include "MCDParserAttribute.h"

namespace MeriseAcide
{

typedef std::list<MCDParserAttribute> MCDParserAttributeList;

class MCDParserAttributeContainer
{
	public:
		virtual ~MCDParserAttributeContainer(void) { }

		const std::string&        GetName(void) const { return mName; }
		const MCDParserAttributeList& GetFieldList(void) { return mChildField; }

		MCDParserAttribute& NewField(void);
	
	protected:
		void SetName(const std::string& n) { mName = n; }
		std::string mName;

	private:
		MCDParserAttributeList   mChildField;
};

}

#endif /* _MCDPARSERATTRIBUTECONTAINER_H_ */
