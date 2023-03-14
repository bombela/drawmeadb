/*
 * \file
 * \brief Entity.
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
 * Class Entity. Simple classe
 * pour gérer une entitée.
 *
 * $Id: MCDParserEntity.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDPARSERENTITY_H_
#define _MCDPARSERENTITY_H_

#include <string>
#include <sstream>

#include "MCDParserAttributeContainer.h"

namespace MeriseAcide
{

class MCDParserEntity: public MCDParserAttributeContainer
{
	public:
		static bool Check(const std::string& Line);

		bool LoadFromString(std::string& Line);

		std::string GetErrorMsg(void) const { return mErrorMsg; }

	private:
		std::string mErrorMsg;
};

// Trait specialization for Entity with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MCDParserEntity &e)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		os << e.GetName();
	}
	return os;
}

}

#endif /* _MCDPARSERENTITY_H_ */
