/*
 * \file
 * \brief MCD Field.
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
 * $Id: MPDTable.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MPDTABLE_H_
#define _MPDTABLE_H_

#include <string>
#include <list>

#include "MPDField.h"
#include "Graphviz.h"

namespace MeriseAcide
{

typedef std::list<MPDField> MPDFieldList;

class MPDTable
{
	public:
		MPDTable(void):
       			mIsLinkTable(false) { }
		MPDTable(const std::string& n):
			mName(n), mIsLinkTable(false) { }

		const std::string& GetName(void)          const { return mName; }
		bool               IsLinkTable(void)      const { return mIsLinkTable; }
		const MPDFieldList& GetKeyList(void)      const { return mKeyList; }
		const MPDFieldList& GetForeingKeyList(void) const { return mFKeyList; }
		const MPDFieldList& GetFieldList(void)    const { return mFieldList; }
		const Agnode_t*     GetGraphvizNode(void) const { return mGraphvizNode; }
		
		void SetName(const std::string& n)     { mName = n; }
		void SetLinkTable(bool l)              { mIsLinkTable = l; }
		void AddKey(const MPDField& k);
		void AddKey(const MPDFieldList& kl);
		void AddForeingKey(const MPDField& fk);
		void AddForeingKey(const MPDFieldList& fkl);
		void AddField(const MPDField& f)         { mFieldList.push_back(f); }
		void SetGraphvizNode(const Agnode_t* gn) { mGraphvizNode = gn; }

	private:
		std::string     mName;
		MPDFieldList    mKeyList;
		MPDFieldList    mFKeyList;
		MPDFieldList    mFieldList;
		bool            mIsLinkTable;
		const Agnode_t* mGraphvizNode;

		const MPDField* FindFieldByName(const MPDFieldList& fl,
				const std::string& n) const;

		MPDField UniqField(const MPDField& f,
				const MPDFieldList& RefList) const;
};

// Trait specialization for entity with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MPDTable &e)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		os << e.GetName()
			<< " Clé("           << e.GetKeyList().size()        << ")"
			<< " Clé étrangère(" << e.GetForeingKeyList().size() << ")"
			<< " Champ("         << e.GetFieldList().size()      << ")";
		if (e.IsLinkTable())
			os << " [LINK TABLE]";
	}
	return os;
}

}

#endif /* _MPDTABLE_H_ */
