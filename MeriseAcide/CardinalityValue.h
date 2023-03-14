/*
 * \file
 * \brief Cardinalitée value.
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
 * Valeur de cardinalitée.
 *
 * $Id: CardinalityValue.h 87 2008-08-13 21:12:29Z bombela $
 */

#ifndef _CARDINALITYVALUE_H_
#define _CARDINALITYVALUE_H_

#include <string>
#include <iostream>
#include <sstream>

namespace MeriseAcide
{

/**
 * \brief Class for store cardinality value.
 * This class is for store one value of cardinality.
 */
class CardinalityValue
{
	public:
		static const unsigned int Invalid  = (-1);
		static const unsigned int Infinite = (-2);

		virtual ~CardinalityValue(void) { }
		CardinalityValue(void): mValue(Invalid) { }
		CardinalityValue(unsigned int v): mValue(v) {}

		bool         IsInfinite(void) const
			{ return (mValue == Infinite); }

		bool         IsInvalid(void)  const
			{ return (mValue == Invalid); }

		unsigned int GetValue(void)   const   { return mValue; }
		void         SetValue(unsigned int u) { mValue = u; }
		void         SetInfinite(void)        { mValue = Infinite; }

		friend bool operator>(const CardinalityValue &a, const CardinalityValue &b)
		{
			if (a.IsInvalid() || b.IsInvalid() || b.IsInfinite())
				return false;
			
			return a.IsInfinite()?true:(a.mValue > b.mValue);
		}

		friend bool operator<(const CardinalityValue &a, const CardinalityValue &b)
		{
			return b > a;
		}
		
		friend bool operator>=(const CardinalityValue &a, const CardinalityValue &b)
		{
			return !(b > a);
		}

		friend bool operator<=(const CardinalityValue &a, const CardinalityValue &b)
		{
			return !(a > b);
		}

	private:
		unsigned int mValue;
};

// Trait specialization for Value with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const CardinalityValue &v)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		if      (v.IsInvalid())
			os << "MeriseAcide::CardinalityValue::Invalid";
		else if (v.IsInfinite())
			os << "n";
		else
			os << v.GetValue();
	}
	return os;
}

}

#endif /* _CARDINALITYVALUE_H_ */
