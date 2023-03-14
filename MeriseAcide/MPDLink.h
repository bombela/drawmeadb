/*
 * \file
 * \brief MCD Association.
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
 * $Id: MPDLink.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MPDLINK_H_
#define _MPDLINK_H_

#include <string>
#include <vector>

#include "MPDTable.h"
#include "Cardinality.h"

namespace MeriseAcide
{

typedef std::vector<const MPDField*> MPDFieldConstPtrTab;

/** Juste two pointer to each key
 * and associate foreing key field
 * between start en destination table.
 */
struct MPDKeyMap
{
	MPDKeyMap(void): Key(0), ForeingKey(0) { }
	MPDKeyMap(const MPDField* k, const MPDField* fk):
		Key(k), ForeingKey(fk) { }

	const MPDField* Key;
	const MPDField* ForeingKey;
};

typedef std::vector<MPDKeyMap> MPDKeyMapTab;

/**
 * \brief The class for link the key of table with foreing key of the destination table.
 * Each key of the start table and each foreing key of the desintation table
 * are stored in a simple vector of struct associate two string.
 */
class MPDLink
{
	public:
		MPDLink(void):
			mStartTable(0),
			mEndTable(0)
			{}

		const MPDTable*
			GetStartTable(void) const { return mStartTable; }

		const MPDTable*
			GetEndTable(void) const { return mEndTable; }

		const MPDKeyMapTab&
			GetKeyMapTab(void) const { return mKeyMap; }

		void SetStartTable(const MPDTable* t)  { mStartTable = t; }
		void SetEndTable(const MPDTable* t)    { mEndTable = t; }
		void SetKeyMapTab(const MPDKeyMapTab& km) { mKeyMap = km; }

	private:
		const MPDTable*  mStartTable;
		const MPDTable*  mEndTable;
		MPDKeyMapTab     mKeyMap;
};

// Trait specialization for MPDKeyMap with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MPDKeyMap &km)
{
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		os << "[";
		if (km.Key)
			os << km.Key->GetName();
		else
			os << "UNKNOW";
		os << ", ";
		if (km.ForeingKey)
			os << km.ForeingKey->GetName();
		else
			os << "UNKNOW";
		os << "]";
	}
	return os;
}

// Trait specialization for link with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MPDLink &a)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		if (a.GetStartTable())
		{	
			os << a.GetStartTable()->GetName();
		}
		else
			os << "UNKNOWN";
		
		os << " --> ";

		if (a.GetEndTable())
		{
			os << a.GetEndTable()->GetName() << " ( ";
			for (MPDKeyMapTab::const_iterator
					i  = a.GetKeyMapTab().begin();
					i != a.GetKeyMapTab().end();
					i++)
				std::cout << (*i) << ", ";
			os << ")";
		}
		else
			os << "UNKNOWN";
	}
	return os;
}

}

#endif /* _MPDLINK_H_ */
