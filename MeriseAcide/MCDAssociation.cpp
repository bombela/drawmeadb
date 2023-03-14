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
 * $Id: MCDAssociation.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "MCDAssociation.h"

namespace MeriseAcide
{

int MCDAssociation::ComputeAndGetPriority(void)
{
	if (mPriority) return mPriority;

	if (mLeftCardinality.IsRelative() || mRightCardinality.IsRelative())
	{
		const MCDEntity* e;

		if (mLeftCardinality.IsRelative())
			e = mLeftEntity;
		else
			// MCD Relative association
			// is only accepted with 2 entity.
			// (Left and Right).
			e = mRightEntity.front();

		mPriority = 1;
		int p;
		
		for (MCDAssociationPtrTabConst::const_iterator
				aptrI  = e->GetAssociation().begin();
				aptrI != e->GetAssociation().end();
				aptrI++)
		{
			MCDAssociation* a = const_cast<MCDAssociation*>( *aptrI );

			if ( a == this )
				continue;

			if (a->GetLeftCardinality().IsRelative())
			{
				if (a->GetLeftEntity() == e)
					continue;
			}
			else
			{
				if (a->GetRightEntity().front() == e)
					continue;
			}
			
			p = a->ComputeAndGetPriority();
			if (p > mPriority)
				mPriority = p;
		}
		mPriority++;
	}
	else
		mPriority = 1;

	return mPriority;
}

}
