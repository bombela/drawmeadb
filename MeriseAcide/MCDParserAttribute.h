/*
 * \file
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
 * $Id: MCDParserAttribute.h 87 2008-08-13 21:12:29Z bombela $
 */

#ifndef _MCDPARSERATTRIBUTE_H_
#define _MCDPARSERATTRIBUTE_H_

#include <string>
#include <sstream>

namespace MeriseAcide
{

// Anticipate declaration.
class MCDParserAttributeContainer;

class MCDParserAttribute
{
	public:
		enum composition_style { None, Left, Right };

		MCDParserAttribute(const MCDParserAttributeContainer* p):
			mComposition(None), mParent(p)
			{}

		static bool Check(const std::string& Line);

		bool LoadFromString(const std::string& Line);

		bool IsWithoutSqlType(void) const { return mSqlType.empty(); }
		bool IsField(void)          const { return !mKey; }
		bool IsKey(void)            const { return mKey; }
		bool IsComposed(void)       const { return mComposition != None; }

		bool IsComposedField(void)
			const { return IsField() && IsComposed(); }

		bool IsComposedKey(void)
			const { return IsKey() && IsComposed(); }

		std::string        GetName(void)    const;
		const std::string& GetSqlType(void) const { return mSqlType; }

		std::string GetErrorMsg(void) const { return mErrorMsg; }

	private:
		std::string       mErrorMsg;
		std::string       mName;
		std::string       mSqlType;
		bool              mKey;         //* Is a Key ?
		composition_style mComposition; //* Composition style.

		const MCDParserAttributeContainer* mParent; //* Parent Entity/Association.

		// Helper function.
		std::string GetTypeName(void);
};

// Trait specialization for Entity with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MCDParserAttribute &f)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		if (f.IsKey())
			os << '+';

		os << f.GetName();

		if (!f.IsWithoutSqlType())
			os << " " << f.GetSqlType();
	}
	return os;
}

}

#endif /* _MCDPARSERATTRIBUTE_H_ */
