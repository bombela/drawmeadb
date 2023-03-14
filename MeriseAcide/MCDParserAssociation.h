/*
 * \file
 * \brief Association.
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
 * Class association.
 *
 * $Id: MCDParserAssociation.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDPARSERASSOCIATION_H_
#define _MCDPARSERASSOCIATION_H_

#include <string>
#include <vector>
#include <sstream>
#include <iterator>

#include "MCDParserAttributeContainer.h"
#include "MCDParserCardinality.h"

namespace MeriseAcide
{

typedef std::vector<std::string> TabMCDParserEntity;

/**
 * \brief Association.
 *
 * Cette class représente une association.
 * Elle est capable de se charger depuis
 * une chaîne de texte au format MeriseAcide.
 */
class MCDParserAssociation: public MCDParserAttributeContainer
{
	public:
		/**
		 * Tester rapidement si la ligne
		 * passée en paramètre représente
		 * une association.
		 */
		static bool Check(std::string& Line);

		/**
		 * \return true si succès.
		 */
		bool LoadFromString(std::string& Line);

		const std::string& GetLeftEntity(void)       const { return mLeftEntity; }
		const TabMCDParserEntity& GetRightEntity(void)
			const { return mRightEntity; }

		const MCDParserCardinality& GetLeftCardinality(void)
			const { return mLeftCardinality; }
		
		const MCDParserCardinality& GetRightCardinality(void)
			const { return mRightCardinality; }

		std::string GetErrorMsg(void) const { return mErrorMsg; }

	private:
		std::string mErrorMsg;

		std::string          mLeftEntity;
		TabMCDParserEntity   mRightEntity;
		MCDParserCardinality mLeftCardinality;
		MCDParserCardinality mRightCardinality;
};

// Trait specialization for Association with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MCDParserAssociation &a)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		os << a.GetLeftEntity()
			<< " " << a.GetLeftCardinality()
			<< " " << a.GetName()
			<< " " << a.GetRightCardinality()
			<< " ";

		const TabMCDParserEntity &e = a.GetRightEntity();
		if (e.size() > 0)
		{
			std::copy(e.begin(), e.end()-1,
					std::ostream_iterator<std::string>(os, ", ") );
			os << e[e.size()-1];
		}
	}
	return os;
}

}

#endif /* _MCDPARSERASSOCIATION_H_ */
