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
 * $Id: MCDDatabase.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "MCDDatabase.h"

#include <iostream>
#include <iterator>
#include <algorithm>

#include "MCDEntity.h"
#include "MCDParserEntity.h"
#include "MCDAssociation.h"
#include "MCDParserAssociation.h"

using namespace std;

namespace MeriseAcide
{

MCDAttribute MCDDatabase::MCDParserFIeldToMCDAttribute(const MCDParserAttribute& f)
{
	MCDAttribute r;

	r.SetName(f.GetName());
	r.SetSqlType(f.GetSqlType());
	r.SetComposedName(f.IsComposed());

	return r;
}


bool MCDDatabase::BuildFromMCDParser(const MCDParser& Parser, bool Verbose/*false*/)
{
	mErrorMsg.str("");

	if (Verbose)
		cout << "--> MCDDatabase::BuildFromMCDParser" << endl;

	// Copy Entity.
	const MCDParserAttributeContainerPtrList&
		ParserEntityList = Parser.GetEntityList();

	for (MCDParserAttributeContainerPtrList::const_iterator
			i = ParserEntityList.begin();
			i != ParserEntityList.end();
			i++)
	{
		MCDParserEntity*
			ParserEntity = dynamic_cast<MCDParserEntity*> (*i);

		if ( FindEntityByName(ParserEntity->GetName()) )
		{
			mErrorMsg
				<< "Entity redefined: "
				<< *ParserEntity;
			return false;
		}
		else
		{
			const MCDParserAttributeList&
			       ParserEntityFieldList = ParserEntity->GetFieldList();

			MCDEntity Entity;
			Entity.SetName(ParserEntity->GetName());

			for (MCDParserAttributeList::const_iterator
					pfI  = ParserEntityFieldList.begin();
					pfI != ParserEntityFieldList.end();
					pfI++)
			{
				if (pfI->IsKey())
					Entity.AddKey(
						MCDParserFIeldToMCDAttribute(*pfI));
				else
					Entity.AddField(
						MCDParserFIeldToMCDAttribute(*pfI));
			}
			
			if (Verbose)
			{
				cout << "Entity: " << Entity << endl;

				for (MCDAttributeList::const_iterator
						j  = Entity.GetKeyList().begin();
						j != Entity.GetKeyList().end();
						j++)
					cout << "   Key: " << *j << endl;

				for (MCDAttributeList::const_iterator
						j  = Entity.GetFieldList().begin();
						j != Entity.GetFieldList().end();
						j++)
					cout << " Field: " << *j << endl;
			}

			mEntityList.push_back(Entity);
		}
	}

	// Link association.
	const MCDParserAttributeContainerPtrList&
		ParserAssocList = Parser.GetAssociationList();

	for (MCDParserAttributeContainerPtrList::const_iterator
			i = ParserAssocList.begin();
			i != ParserAssocList.end();
			i++)
	{
		MCDParserAssociation*
			ParserAssoc = dynamic_cast<MCDParserAssociation*> (*i);

		if (FindAssociationByName(ParserAssoc->GetName()))
		{
			mErrorMsg
				<< "Association redefined: "
				<< *ParserAssoc;
			return false;
		}

		mAssocList.push_back( MCDAssociation() );
		MCDAssociation& Assoc = mAssocList.back();

		Assoc.SetName(ParserAssoc->GetName());
		Assoc.SetLeftCardinality(ParserAssoc->GetLeftCardinality());
		Assoc.SetRightCardinality(ParserAssoc->GetRightCardinality());

		// Copy Association Fields
		for (MCDParserAttributeList::const_iterator
				fI  = ParserAssoc->GetFieldList().begin();
				fI != ParserAssoc->GetFieldList().end();
				fI++)
			Assoc.AddField(MCDParserFIeldToMCDAttribute(*fI));

		// Copy link informations
		MCDEntity*
			LeftEntityPtr = FindEntityByName(ParserAssoc->GetLeftEntity());

		if (LeftEntityPtr)
		{
			Assoc.SetLeftEntity(LeftEntityPtr);
			LeftEntityPtr->AddAssociation(&Assoc);
		}
		else
		{
			mErrorMsg << "Impossible to resolve association:\n"
				<< *ParserAssoc << "\nLeft entity not found";
			return false;
		}

		for (TabMCDParserEntity::const_iterator
				j  = ParserAssoc->GetRightEntity().begin();
				j != ParserAssoc->GetRightEntity().end();
				j++)
		{
			MCDEntity* RightEntityPtr = FindEntityByName(*j);
			if (RightEntityPtr)
			{
				Assoc.AddRightEntity(RightEntityPtr);
				RightEntityPtr->AddAssociation(&Assoc);
			}
			else
			{
				mErrorMsg << "Impossible to return association:\n"
					<< *ParserAssoc
					<< "\nRight(s) entity not found";
				return false;
			}
		}

		if (Verbose)
			cout << " Assoc : " << Assoc << endl;
	}

	if (Verbose)
		cout << "Reordering association by priority..." << endl;

	mAssocList.sort(greater<MCDAssociation>());

	return true;
}

MCDEntity* MCDDatabase::FindEntityByName(const std::string& Name)
{
	for (MCDEntityList::iterator
			i  = mEntityList.begin();
			i != mEntityList.end();
			i++)
	{
		if ( (*i).GetName() == Name )
			return &(*i);
	}

	return 0;
}

MCDAssociation* MCDDatabase::FindAssociationByName(const std::string& Name)
{
	for (MCDAssociationList::iterator
			i  = mAssocList.begin();
			i != mAssocList.end();
			i++)
	{
		if ( (*i).GetName() == Name )
			return &(*i);
	}

	return 0;
}

bool MCDDatabase::FillGraph(Graphviz& gv, bool PrintType)
{
	// All Antity.
	for (MCDEntityList::iterator
		eI  = mEntityList.begin();
		eI != mEntityList.end();
		eI++)
	{
		const Agnode_t* n =
			gv.AddTable(eI->GetName(), TSEntity);

		// All Key
		for (MCDAttributeList::const_iterator
			kE  = eI->GetKeyList().begin();
			kE != eI->GetKeyList().end();
			kE++)
		{
			string l = kE->GetName();

			if (PrintType)
				l.append(" " + kE->GetSqlType());

			gv.AddKey(n, l);
		}
	
		// All Field
		for (MCDAttributeList::const_iterator
			fE  = eI->GetFieldList().begin();
			fE != eI->GetFieldList().end();
			fE++)
		{
			string l = fE->GetName();

			if (PrintType)
				l.append(" " + fE->GetSqlType());

			gv.AddField(n, l);
		}

		eI->SetGraphvizNode(n);
	}

	// All Association.
	for (MCDAssociationList::const_iterator
		aI  = mAssocList.begin();
		aI != mAssocList.end();
		aI++)
	{
		const Agnode_t* n =
			gv.AddTable(aI->GetName(), TSAssoc);

		// All Field
		for (MCDAttributeList::const_iterator
			fE  = aI->GetFieldList().begin();
			fE != aI->GetFieldList().end();
			fE++)
		{
			string l = fE->GetName();

			if (PrintType)
				l.append(" " + fE->GetSqlType());

			gv.AddField(n, l);
		}

		// Left Link
		ostringstream Card;
		Card << aI->GetLeftCardinality();
		gv.AddLink(aI->GetLeftEntity()->GetGraphvizNode(), n, Card.str() );

		Card.str("");
		Card << aI->GetRightCardinality();

		// All Right Link
		for (MCDEntityPtrTabConst::const_iterator
				fI  = aI->GetRightEntity().begin();
				fI != aI->GetRightEntity().end();
				fI++)
		{
			gv.AddLink( (*fI)->GetGraphvizNode(), n, Card.str() );
		}
	}

	return true;
}

}

