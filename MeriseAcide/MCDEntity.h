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
 * $Id: MCDEntity.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDENTITY_H_
#define _MCDENTITY_H_

#include <string>
#include <list>
#include <vector>

#include "MCDAttribute.h"
#include "Graphviz.h"

namespace MeriseAcide
{

// Anticipate declaration.
class MCDAssociation;

typedef std::list<MCDAttribute> MCDAttributeList;
typedef std::vector<const MCDAssociation*> MCDAssociationPtrTabConst;

class MCDEntity
{
	public:
		const std::string&  GetName(void)         const { return mName; }
		const MCDAttributeList& GetKeyList(void)      const { return mKeyList; }
		const MCDAttributeList& GetFieldList(void)    const { return mFieldList; }
		const Agnode_t*     GetGraphvizNode(void) const	{ return mGraphvizNode; }

		const MCDAssociationPtrTabConst& GetAssociation(void) const
			{ return mAssocList; }
		
		void SetName(const std::string& n)       { mName = n; }
		void AddKey(const MCDAttribute& k)           { mKeyList.push_back(k); }
		void AddField(const MCDAttribute& f)         { mFieldList.push_back(f); }
		void SetGraphvizNode(const Agnode_t* gn) { mGraphvizNode = gn; }
		void AddAssociation(const MCDAssociation* a);

	private:
		std::string               mName;
		MCDAttributeList              mKeyList;
		MCDAttributeList              mFieldList;
		MCDAssociationPtrTabConst mAssocList;
		const Agnode_t*           mGraphvizNode;
};

// Trait specialization for entity with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MCDEntity &e)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		os << e.GetName()
			<< " Clé("   << e.GetKeyList().size()   << ")"
			<< " Champ(" << e.GetFieldList().size() << ")";
	}
	return os;
}

}

#endif /* _MCDENTITY_H_ */

