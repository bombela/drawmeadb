/*
 * \file
 * \brief MPD Field.
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
 * $Id: MPDField.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MPDFIELD_H_
#define _MPDFIELD_H_

#include <string>

namespace MeriseAcide
{

class MPDField
{
	public:
		MPDField(void):
			mComposedName(false)
			{}

		bool       IsWithoutSqlType(void) const { return mSqlType.empty(); }
		bool       IsComposedName(void)   const { return mComposedName; }
		const bool IsForeingKey(void)     const { return !mFKeyLinkName.empty(); }

		const std::string& GetName(void)       const { return mName; }
		std::string        GetFullName(void)   const;
		const std::string& GetSqlType(void)    const { return mSqlType; }
		std::string        GetFullNameAscii(void) const;
		const std::string& GetFKeyLinkName(void) const { return mFKeyLinkName; }

		void SetName(const std::string& n)        { mName = n; }
		void SetSqlType(const std::string& t)     { mSqlType = t; }
		void SetComposedName(bool cn)             { mComposedName = cn; }
		void SetFKeyLinkName(const std::string n) { mFKeyLinkName = n; }

	private:
		std::string     mName;
		std::string     mSqlType;
		bool            mComposedName;
		std::string	mFKeyLinkName;
};

// Trait specialization for field with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const MPDField &f)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		os << f.GetName();
		if (!f.IsWithoutSqlType())
			os << " " << f.GetSqlType();
	}
	return os;
}

}

#endif /* _MPDFIELD_H_ */
