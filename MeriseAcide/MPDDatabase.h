/*
 * \file
 * \brief MPD Database.
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
 * Base de donnée représentant un MPD.
 *
 * $Id: MPDDatabase.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MPDDATABASE_H_
#define _MPDDATABASE_H_

#include <string>
#include <sstream>
#include <list>

#include "MCDDatabase.h"
#include "MPDTable.h"
#include "MPDField.h"
#include "MPDLink.h"
#include "SQLPrint.h"

namespace MeriseAcide
{

typedef std::list<MPDTable> MPDTableList;
typedef std::list<MPDLink>  MPDLinkList;

class MPDDatabase
{
	public:
		bool BuildFromMCDDatabase(const MCDDatabase& Db, bool Verbose = false);
		bool FillGraph(Graphviz& gv, bool PrintType = false);
		bool PrintToSQL(SQLPrint& SQL, bool FKeyConstraintDisabled = false);

		std::string GetErrorMsg(void) const { return mErrorMsg.str(); }

	private:
		std::ostringstream mErrorMsg;
		MPDTableList       mTableList;
		MPDLinkList        mLinkList;

		MPDTable* FindTableByName(const std::string& Name);

		void PrintFieldList(const std::string& Msg, const MPDFieldList& fl);
		void BuildTableFromMCD(const MCDDatabase& Db, bool Verbose = false);
		void PrintTableAndContent(const MPDTable& t);
		bool BuildLinkFromMCD(const MCDDatabase& Db, bool Verbose = false);

		MPDField     KeyToFKey(
				const MPDField& k,
				const std::string& tn,
				const std::string& ln);

		MPDFieldList KeyListToFKeyList(
				const MPDFieldList& kl,
				const std::string& tn,
				const std::string& ln);

		MPDFieldConstPtrTab FieldListToFieldConstPtrTab(
				const MPDFieldList& fl);

		MPDField MCDAttributeToMPDField(const MCDAttribute& f);
};


}

#endif /* _MPDDATABASE_H_ */
