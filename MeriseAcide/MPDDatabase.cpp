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
 * $Id: MPDDatabase.cpp 87 2008-08-13 21:12:29Z bombela $
 */

#include "MPDDatabase.h"

#include <iostream>

#include "MCDDatabase.h"
#include "MPDTable.h"

using namespace std;

namespace MeriseAcide
{

bool MPDDatabase::BuildFromMCDDatabase(const MCDDatabase& Db, bool Verbose/*false*/)
{
	mErrorMsg.str("");

	if (Verbose)
		cout << "--> Converting MCD to MPD..." << endl;

	// First, copy all entities to table.	
	BuildTableFromMCD(Db, Verbose);

	// Second, analyse each association,
	// make and link table.
	if (!BuildLinkFromMCD(Db, Verbose))
		return false;

	return true;
}

void MPDDatabase::PrintFieldList(const string& Msg, const MPDFieldList& fl)
{
	for (MPDFieldList::const_iterator
			i  = fl.begin();
			i != fl.end();
			i++)
	{
		cout << Msg << (*i) << endl;
	}
}

MPDField MPDDatabase::MCDAttributeToMPDField(const MCDAttribute& f)
{
	MPDField r;

	r.SetName(f.GetName());
	r.SetSqlType(f.GetSqlType());
	r.SetComposedName(f.IsComposedName());

	return r;
}

void MPDDatabase::BuildTableFromMCD(const MCDDatabase& Db, bool Verbose/*false*/)
{
	if (Verbose)
		cout << "--> Copy all MCD entity to MPD table..." << endl;

	const MCDEntityList& EntityList = Db.GetEntityList();

	// For each entities...
	for (MCDEntityList::const_iterator
			i  = EntityList.begin();
			i != EntityList.end();
			i++)
	{
		const MCDEntity& Entity (*i);
		MPDTable   Table;

		// Copy name.
		Table.SetName(Entity.GetName());

		// Copy key.
		for (MCDAttributeList::const_iterator
				j  = Entity.GetKeyList().begin();
				j != Entity.GetKeyList().end();
				j++)
			Table.AddKey(MCDAttributeToMPDField(*j));

		// Copy field.
		for (MCDAttributeList::const_iterator
				j  = Entity.GetFieldList().begin();
				j != Entity.GetFieldList().end();
				j++)
			Table.AddField(MCDAttributeToMPDField(*j));

		mTableList.push_back(Table);

		if (Verbose)
			PrintTableAndContent(Table);
	}
}

void MPDDatabase::PrintTableAndContent(const MPDTable& t)
{
	cout << "Table  : " << t << endl;
	PrintFieldList("   Key : ", t.GetKeyList());
	PrintFieldList("  fKey : ", t.GetForeingKeyList());
	PrintFieldList(" Field : ", t.GetFieldList());
}

bool MPDDatabase::BuildLinkFromMCD(const MCDDatabase& Db, bool Verbose/*false*/)
{
	const MCDAssociationList AssocList = Db.GetAssociationList();

	if (Verbose)
		cout << "--> Building link from MCD..." << endl;


	// Read all association.
	for (MCDAssociationList::const_iterator
			AssocI  = AssocList.begin();
			AssocI != AssocList.end();
			AssocI++)
	{
		const MCDAssociation&  Assoc = *AssocI;
		const Cardinality&     LCard = Assoc.GetLeftCardinality();
		const Cardinality&     RCard = Assoc.GetRightCardinality();

		MPDTable* LTable
		       = FindTableByName(Assoc.GetLeftEntity()->GetName());
		
		MPDTable* RTable = FindTableByName(
				Assoc.GetRightEntity().front()->GetName());

		if ( (!LTable) || (!RTable) )
		{
			mErrorMsg << "INTERNAL ERROR : " << __FILE__
				<< ":" << __LINE__;
			return false;
		}


		if      ( (LCard.GetMax() > 1) && (RCard.GetMax() > 1) )
		{
			// ...,[2-n] ; ...,[2-n]
			// Link crossed.
			
			/*
			 *  -----------                         ------------
			 * | LeftTable |   <- Association ->   | RightTable |
			 *  -----------                         ------------
			 *
			 *               CONVERT MCD ASSOCATION TO MPD.
			 *
			 *  -----------      -------------      ------------
			 * | LeftTable | -> | CenterTable | <- | RightTable |
			 *  -----------      -------------      ------------
			 *   ^           ^        ^         ^              ^
			 *  Existant | New link | New table | New link | Existant 
			 *
			 * 1: Make center table.
			 * 2: Convert and add all LeftTable key to CenterTable FKey.
			 * 3: Make left link with all information get above.
			 * 4: Convert and add all RightTable key to CenterTable FKey.
			 * 5: Make right link with all information get above.
			 */

			if (Verbose)
				cout << "Cross-link : " << Assoc << endl;

			// 1: Make center table.
			mTableList.push_back(Assoc.GetName());
			MPDTable& CTable = mTableList.back();
			CTable.SetLinkTable(true);

			// Add all field Association to Table
			for (MCDAttributeList::const_iterator
				faI  = Assoc.GetFieldList().begin();
				faI != Assoc.GetFieldList().end();
				faI++)
				CTable.AddField(MCDAttributeToMPDField(*faI));

			// 2: Convert and add all LeftTable key to CenterTable FKey.
			MPDKeyMapTab LKeyMapTab;

			for (MPDFieldList::const_iterator
					KeyI  = LTable->GetKeyList().begin();
					KeyI != LTable->GetKeyList().end();
					KeyI++)
			{
				CTable.AddKey(KeyToFKey(*KeyI, LTable->GetName(),
							Assoc.GetName()) );
				MPDKeyMap LKeyMap(
						&(*KeyI),
						&CTable.GetKeyList().back()
						);
				LKeyMapTab.push_back( LKeyMap );
			}

			// 3: Make left link with all information get above.
			MPDLink LLink;
			LLink.SetStartTable( LTable );
			LLink.SetEndTable( &CTable );
			LLink.SetKeyMapTab( LKeyMapTab );
			mLinkList.push_back( LLink );

			if (Verbose)
			{
				cout << "L.Link : " << LLink << endl;
				PrintTableAndContent(CTable);
			}

			// For each right entity
			for (MCDEntityPtrTabConst::const_iterator
					Entity  = Assoc.GetRightEntity().begin();
					Entity != Assoc.GetRightEntity().end();
					Entity++)
			{
				RTable = FindTableByName( (*Entity)->GetName() );

				// 4: Convert and add all RightTable key
				// to CenterTable FKey.
				MPDKeyMapTab RKeyMapTab;

				for (MPDFieldList::const_iterator
						KeyI  = RTable->GetKeyList().begin();
						KeyI != RTable->GetKeyList().end();
						KeyI++)
				{
					CTable.AddKey(
						KeyToFKey(*KeyI, RTable->GetName(),
							Assoc.GetName()) );

					MPDKeyMap RKeyMap(
							&(*KeyI),
							&CTable.GetKeyList().back()
							);

					RKeyMapTab.push_back( RKeyMap );
				}

				// 5: Make right link with all information get above.
				MPDLink RLink;
				RLink.SetStartTable( RTable );
				RLink.SetEndTable( &CTable );
				RLink.SetKeyMapTab( RKeyMapTab );
				mLinkList.push_back( RLink );
				
				if (Verbose)
					cout << "R.Link : " << RLink << endl;
			}
		}
		else if ( (LCard.GetMax() >= 1) && (RCard.GetMax() >= 1) )
		{
			bool Relative;

			if (LCard.GetMax() > 1) 
			{
				// ...,[2-n] ; ...,[1]
				// Left linked to right.
				if (Verbose)
					cout << "Left-to-right link : " << Assoc << endl;

				Relative = RCard.IsRelative(); 
			}
			else
			{	
				// ...,[1]   ; ...,[2,n]
				// Right linked to left.
				if (Verbose)
					cout << "Right-to-left link : " << Assoc << endl;

				Relative = LCard.IsRelative(); 

				// Use same as left linked.
				MPDTable* TTable = LTable;
				LTable = RTable;
				RTable = TTable;
			}

			// If more than one rigth entity, error !
			// It's not possible.
			if (Assoc.GetRightEntity().size() > 1)
			{
				mErrorMsg << "Too many entity linked"
					<< " with cardinality ?,1 :\n"
					<< Assoc;
				return false;
			}

			// Put left key to right fkey.
			MPDKeyMapTab KeyMapTab;

			for (MPDFieldList::const_iterator
					KeyI  = LTable->GetKeyList().begin();
					KeyI != LTable->GetKeyList().end();
					KeyI++)
			{
				const MPDField* rf;
				if (Relative)
				{
					RTable->AddKey(
						KeyToFKey(*KeyI, LTable->GetName(),
							Assoc.GetName()) );
					rf = &RTable->GetKeyList().back();
				}
				else
				{
					RTable->AddForeingKey(
						KeyToFKey(*KeyI, LTable->GetName(),
							Assoc.GetName()) );
					rf = &RTable->GetForeingKeyList().back();
				}

				MPDKeyMap KeyMap( &(*KeyI), rf );
				KeyMapTab.push_back( KeyMap );
			}

			// Make link.
			MPDLink Link;
			Link.SetStartTable( LTable );
			Link.SetEndTable( RTable );
			Link.SetKeyMapTab( KeyMapTab );
			mLinkList.push_back( Link );

			if (Verbose)
			{
				cout << "Link : " << Link << endl;
				PrintTableAndContent( (*RTable) );
			}
		}
		else
		{
			mErrorMsg << "Invalid association \n:"
				<< Assoc;
			return false;
		}
	}

	return true;
}

MPDTable* MPDDatabase::FindTableByName(const std::string& Name)
{
	for (MPDTableList::iterator
			i  = mTableList.begin();
			i != mTableList.end();
			i++)
	{
		if ( (*i).GetName() == Name )
			return &(*i);
	}

	return 0;
}

// Convert Key to Fkey, adapt name if needed.
MPDField MPDDatabase::KeyToFKey(const MPDField& k, const std::string& tn,
		const std::string& ln)
{
	MPDField Result(k);

	if ( !k.IsComposedName() )
		Result.SetName( k.GetName() + "_" + tn );

	Result.SetComposedName(true);
	Result.SetFKeyLinkName(ln);
	return Result;
}

MPDFieldList MPDDatabase::KeyListToFKeyList(
		const MPDFieldList& kl,
		const std::string& tn,
		const std::string& ln)
{
	MPDFieldList Result;
	for (MPDFieldList::const_iterator
			i  = kl.begin();
			i != kl.end();
			i++)
		Result.push_back( KeyToFKey(*i, tn, ln) );

	return Result;
}

MPDFieldConstPtrTab MPDDatabase::FieldListToFieldConstPtrTab(const MPDFieldList& fl)
{
	MPDFieldConstPtrTab Result;

	for (MPDFieldList::const_iterator
			i  = fl.begin();
			i != fl.end();
			i++)
	{
		Result.push_back( &(*i) );
	}
	return Result;
}

bool MPDDatabase::PrintToSQL(SQLPrint& SQL, bool FKeyConstraintDisabled/*false*/)
{
	SQL.DisableForeingKeyCheck();
	SQL.BreakLine();

	// Remove all table.
	for (MPDTableList::const_iterator
			TableI  = mTableList.begin();
			TableI != mTableList.end();
			TableI++)
		SQL.DropTableIfExist( TableI->GetName() );
	
	SQL.BreakLine();

	// Add all table.
	for (MPDTableList::const_iterator
			TableI  = mTableList.begin();
			TableI != mTableList.end();
			TableI++)
	{
		SQL.CreateTable(TableI->GetName());

		// Add all key.
		StringList KeyList;
		for (MPDFieldList::const_iterator
				KeyI  = TableI->GetKeyList().begin();
				KeyI != TableI->GetKeyList().end();
				KeyI++)
		{
			KeyList.push_back(KeyI->GetFullName());
			
			// Don't print if PRIMARY FOREING KEY
			if (!KeyI->IsForeingKey())
			{
				SQL.AddKey(
					KeyI->GetName(),
					KeyI->GetSqlType(),
					!TableI->IsLinkTable()
					);
			}
		}

		// Add all fkey
		// Search all link with the current table for destination.
		for (MPDLinkList::const_iterator
				LinkI  = mLinkList.begin();
				LinkI != mLinkList.end();
				LinkI++)
		{
			if ( LinkI->GetEndTable() == &(*TableI) )
			{
				// For each KeyMap, add the FKey and
				// it destination type. (The type of referenced key).
				for (MPDKeyMapTab::const_iterator
						KeyMapI  = LinkI->GetKeyMapTab().begin();
						KeyMapI != LinkI->GetKeyMapTab().end();
						KeyMapI++)
				{
					SQL.AddFKey(
						KeyMapI->ForeingKey->GetFullName(),
						KeyMapI->Key->GetSqlType()
						);
				}
			}
		}

		// Add all field.
		for (MPDFieldList::const_iterator
				FieldI  = TableI->GetFieldList().begin();
				FieldI != TableI->GetFieldList().end();
				FieldI++)
		{
			SQL.AddField(FieldI->GetName(), FieldI->GetSqlType());
		}

		// Add primary key.
		SQL.PrimaryKey(KeyList);

		if (!FKeyConstraintDisabled)
		{
			// Add all FKey constraints.
			// Search all link with the current table for destination.
			for (MPDLinkList::const_iterator
					LinkI  = mLinkList.begin();
					LinkI != mLinkList.end();
					LinkI++)
			{
				if ( LinkI->GetEndTable() == &(*TableI) )
				{
					string FKey;
					string Key;
					int    i = 0;

					// For each KeyMap, add the constraint.
					for (MPDKeyMapTab::const_iterator
							KeyMapI  = LinkI->GetKeyMapTab().begin();
							KeyMapI != LinkI->GetKeyMapTab().end();
							KeyMapI++)
					{
						if (i++ == 0)
						{
							FKey = KeyMapI->ForeingKey->GetFullName();
							Key  = KeyMapI->Key->GetFullName();
						}
						else
						{
							FKey += ", " + KeyMapI->
								ForeingKey->GetFullName();
							Key  += ", " + KeyMapI->
								Key->GetFullName();
						}
					}
					
					SQL.AddConstraint(FKey,
							LinkI->GetStartTable()->GetName(),
							Key);
				}
			}
		}

		SQL.CreateTable();
		SQL.BreakLine();
	}

	SQL.EnableForeingKeyCheck();

	return true;
}

bool MPDDatabase::FillGraph(Graphviz& gv, bool PrintType/*false*/)
{
	// All table.
	for (MPDTableList::iterator
			tI  = mTableList.begin();
			tI != mTableList.end();
			tI++)
	{
		const Agnode_t* n = gv.AddTable(tI->GetName(), TSTable);

		// All key.
		for (MPDFieldList::const_iterator
				fI  = tI->GetKeyList().begin();
				fI != tI->GetKeyList().end();
				fI++)
		{
			string label(fI->GetFullName());
			
			if (PrintType)
				label.append(" " + fI->GetSqlType());
			
			gv.AddKey(n, label, fI->GetFullNameAscii());
		}

		// All fkey.
		for (MPDFieldList::const_iterator
				fI  = tI->GetForeingKeyList().begin();
				fI != tI->GetForeingKeyList().end();
				fI++)
		{
			string label(fI->GetFullName()
					+ " (" + fI->GetFKeyLinkName() + ")");
			
			if (PrintType)
				label.append(" " + fI->GetSqlType());
			
			gv.AddField(n, label, fI->GetFullNameAscii());
		}

		// All field.
		for (MPDFieldList::const_iterator
				fI  = tI->GetFieldList().begin();
				fI != tI->GetFieldList().end();
				fI++)
		{
			string label(fI->GetFullName());
			
			if (PrintType)
				label.append(" " + fI->GetSqlType());
			
			gv.AddField(n, label, fI->GetFullNameAscii());
		}

		tI->SetGraphvizNode(n);
	}
	
	// All link.
	for (MPDLinkList::const_iterator
			lI  = mLinkList.begin();
			lI != mLinkList.end();
			lI++)
	{
		for (MPDKeyMapTab::const_iterator
				kmI  = lI->GetKeyMapTab().begin();
				kmI != lI->GetKeyMapTab().end();
				kmI++)
		{
			const string& sf = kmI->Key->GetFullNameAscii();
			const string& ef = kmI->ForeingKey->GetFullNameAscii();

			// Link Fk to Key
			gv.AddLink(lI->GetEndTable()->GetGraphvizNode(),
					lI->GetStartTable()->GetGraphvizNode(),
					ef, sf);
		}
	}

	return true;
}

}
