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
 * $Id: MCDAssociation.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDASSOCIATION_H_
#define _MCDASSOCIATION_H_

#include <string>
#include <vector>

#include "MCDEntity.h"
#include "Cardinality.h"

namespace MeriseAcide
{

typedef std::vector<const MCDEntity*> MCDEntityPtrTabConst;

class MCDAssociation
{
	public:
		MCDAssociation(void):
			mLeftEntity(0),
			mPriority(0)
			{}

		const std::string& GetName(void)
			const { return mName; }
		const MCDEntity* GetLeftEntity(void)
			const { return mLeftEntity; }
		const MCDEntityPtrTabConst& GetRightEntity(void)
			const { return mRightEntity; }
		const Cardinality& GetLeftCardinality(void)
			const { return mLeftCardinality; }
		const Cardinality& GetRightCardinality(void)
			const { return mRightCardinality; }
		const MCDAttributeList& GetFieldList(void)
			const { return mFieldList; }

		void SetName(const std::string& n)
			{ mName = n; }
		void AddRightEntity(const MCDEntity* e)
			{ mRightEntity.push_back(e); }
		void SetLeftEntity(const MCDEntity* e)
			{ mLeftEntity = e; }
		void SetLeftCardinality(const Cardinality& c)
			{ mLeftCardinality = c; }
		void SetRightCardinality(const Cardinality& c)
			{ mRightCardinality = c; }
		void AddField(const MCDAttribute& f)
			{ mFieldList.push_back(f); }

		bool operator<(MCDAssociation& a)
			{ return ComputeAndGetPriority() < a.ComputeAndGetPriority(); }

		// Ugly code ? Yes, i think too.
		bool operator>(const MCDAssociation& a) const
		{
			MCDAssociation& ma = const_cast<MCDAssociation&>(a);
			MCDAssociation& mb = const_cast<MCDAssociation&>(*this);
			return mb.ComputeAndGetPriority() > ma.ComputeAndGetPriority();
		}

	private:
		std::string           mName;
		const MCDEntity*      mLeftEntity;
		MCDEntityPtrTabConst  mRightEntity;
		Cardinality           mLeftCardinality;
		Cardinality           mRightCardinality;
		MCDAttributeList          mFieldList;
		int                   mPriority;

		int ComputeAndGetPriority(void);
};

// Trait specialization for association with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MCDAssociation &a)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		if (a.GetLeftEntity())
			os << a.GetLeftEntity()->GetName();
		else
			os << "UNKNOWN";
		os
			<< " " << a.GetLeftCardinality()
			<< " " << a.GetName()
			<< " " << a.GetRightCardinality()
			<< " ";

		const MCDEntityPtrTabConst &e = a.GetRightEntity();
		if (e.size() > 0)
		{
			for (MCDEntityPtrTabConst::const_iterator
					i  = e.begin();
					i != e.end()-1;
					i++)
				os << (*i)->GetName() << ", ";
		       os << e.back()->GetName();
		}
		else
			os << "UNKNOWN";
	}
	return os;
}

}

#endif /* _MCDASSOCIATION_H_ */
