/*
 * \file
 * \brief Parseur de MCD au format MeriseAcide.
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
 * Ce parser est capable de contruire
 * une base d'inforation depuis un MCD
 * au format MeriseAcide.
 *
 * $Id: MCDParser.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDPARSER_H_
#define _MCDPARSER_H_

#include <string>
#include <sstream>
#include <list>

#include "MCDParserAttributeContainer.h"

namespace MeriseAcide
{

typedef std::list<MCDParserAttributeContainer*> MCDParserAttributeContainerPtrList;

class MCDParser
{
	public:
		MCDParser(void);
		~MCDParser(void);

		bool Parse(const std::string& FileName, bool Verbose = false);
		const MCDParserAttributeContainerPtrList& GetEntityList(void)
			const { return mEntityList; };
		const MCDParserAttributeContainerPtrList& GetAssociationList(void)
			const { return mAssocList; };

		std::string GetErrorMsg(void) const { return mErrorMsg.str(); }

	private:
		std::ostringstream mErrorMsg;
		MCDParserAttributeContainerPtrList  mEntityList;
		MCDParserAttributeContainerPtrList  mAssocList;

		void RemoveComment(std::string& Line);
};

}

#endif /* _MCDPARSER_H_ */
