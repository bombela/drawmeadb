/*
 * \file
 * \brief Data dictionnary representation.
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
 * Data dictionnary representation.
 * Can print data dictionnary with many
 * representation.
 *
 * $Id: DataDic.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _DATADIC_H_
#define _DATADIC_H_

#include "MCDDatabase.h"

#include <string>
#include <vector>
#include <ostream>

namespace MeriseAcide
{

class DataDic
{
	public:
		virtual ~DataDic(void) { }

		bool LoadFromMCDDatabase(const MCDDatabase& mcd);
		bool PrintToFile(const std::string& FileName);
		void SetShowType(bool s) { mShowType = s; }
		bool GetShowType(void) const { return mShowType; }

		std::string GetErrorMsg(void) const { return mErrorMsg.str(); }

		virtual bool Print(std::ostream& s) = 0;

	protected:
		struct OneData
		{
			std::string Data;
			std::string Type;
			bool        ID;
		};
		typedef std::vector<OneData> DataList;

		DataList mDataList;
		std::ostringstream mErrorMsg;
		bool     mShowType;
};

}

#endif /* _DATADIC_H_ */

