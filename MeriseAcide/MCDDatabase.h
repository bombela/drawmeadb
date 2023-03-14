/*
 * \file
 * \brief MCD Database.
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
 * Base de donnée réprésentant un MCD.
 *
 * $Id: MCDDatabase.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDDATABASE_H_
#define _MCDDATABASE_H_

#include <string>
#include <sstream>

#include "MCDParser.h"
#include "MCDEntity.h"
#include "MCDAssociation.h"

namespace MeriseAcide
{

typedef std::list<MCDEntity>             MCDEntityList;
typedef std::list<MCDAssociation>        MCDAssociationList;

class MCDDatabase
{
	public:
		bool BuildFromMCDParser(const MCDParser& Parser, bool Verbose = false);
		bool FillGraph(Graphviz& gv, bool PrintType = false);

		const MCDEntityList&      GetEntityList(void)
			const { return mEntityList; }
		const MCDAssociationList& GetAssociationList(void)
			const { return mAssocList; }

		std::string GetErrorMsg(void) const { return mErrorMsg.str(); }

	private:
		std::ostringstream mErrorMsg;
		MCDEntityList      mEntityList;
		MCDAssociationList mAssocList;

		MCDEntity*      FindEntityByName(const std::string& Name);
		MCDAssociation* FindAssociationByName(const std::string& Name);
		MCDAttribute        MCDParserFIeldToMCDAttribute(const MCDParserAttribute& f);
};


}

#endif /* _MCDDATABASE_H_ */

