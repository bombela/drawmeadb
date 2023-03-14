/*
 * \file
 * \brief Wrapper graphviz.
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
 * Wrapper pour la lib graphviz.
 *
 * $Id: Graphviz.cpp 86 2008-08-09 20:07:04Z bombela $
 */

#include <iostream>
#include <sstream>

#include "Graphviz.h"

using namespace std;

namespace MeriseAcide
{

const char* mColorList[] =
{
	"blueviolet",
	"brown4",
	"burlywood4",
	"cadetblue4",
	"chartreuse4",
	"coral4",
	"orange",
	"cornsilk3",
	"cyan2",
	"darkgoldenrod2",
	"pink",
	"black",
	"darkolivegreen",
	"darkorange",
	"darksalmon",
	"darkseagreen4",
	"violet",
	"darkslategray3",
	"deeppink",
	"cyan",
	"deepskyblue",
	"dimgray",
	"dodgerblue3",
	"firebrick3",
	"magenta",
	0
};

const char* Graphviz::NextColor(void)
{
	if (mColorList[mCurIdxColor] == 0)
		mCurIdxColor = 0;
	return mColorList[mCurIdxColor++];
}

Graphviz::Graphviz(void):
	mGVC(0),
	mGraph(0),
	mRender(RNEATO),
	mMaxIter(1000),
	mKVal(0.9),
	mComputed(false),
	mUseColor(true),
	mCurIdxColor(0),
	mEdgeLen(1.0),
	mColorEntity("gray65"),
	mColorAssoc("indianred1"),
	mColorTable("#b8f196")
{
}

Graphviz::~Graphviz(void)
{
	Reset_Free();	
}

void Graphviz::Reset(ERender r/*RFDP*/)
{
	Reset_Free();
	Reset_Make(r);
}

void Graphviz::Reset_Free(void)
{
	ResetCompute();
	if (mGraph)
		agclose(mGraph);
	if (mGVC)
		gvFreeContext(mGVC);
}

void Graphviz::ResetCompute()
{
	if (mComputed)
	{
		mComputed = false;
		gvFreeLayout(mGVC, mGraph);
	}
}

void Graphviz::Reset_Make(ERender r)
{
	mRender = r;
	mGVC = gvContext();
	mGraph = agopen(const_cast<char*>("G"), AGDIGRAPH);

	// Set default graph attribute...
	
	// FIXME
	// I don't want use this...
	//char* toto[] = { "dot", "-o/dev/null", 0};
	//gvParseArgs(mGVC, 2, toto);
	
	// FIXME Fuck... maxiter is not used
	// if i not call gvParseArgs before !
	// Yes, i can use directly
	// gvParseArgs but it have many
	// secondary effect...
	// Bug ?
	SetGraphDefaultValue("maxiter", mMaxIter);

	//SetGraphDefaultValue("K", mKVal);
	SetGraphDefaultValue("overlap", "compressed");

	switch (mRender)
	{
		case RDOT:
			SetGraphDefaultValue("splines", "polyline");
			break;

		case RFDP:
		case RNEATO:
		case RCIRCO:
		case RTWOPI:
			SetGraphDefaultValue("splines", "false");
			break;
	}

	//SetGraphDefaultValue("sep", ".1");
	SetGraphDefaultValue("rankdir", "LR");

	// Set default node attribute.
	SetNodeDefaultValue("fontsize", 10);
	SetNodeDefaultValue("shape", "record");
	
	if (mUseColor)
		SetNodeDefaultValue("style", "filled");

	// Se default edge attribute;
	SetEdgeDefaultValue("fontsize", 9);
	SetEdgeDefaultValue("arrowsize", 0.6);
	SetEdgeDefaultValue("arrowhead", "normal");
	SetEdgeDefaultValue("len", mEdgeLen);

	if (mUseColor)
		SetEdgeDefaultValue("fontcolor", "blue");
}

void Graphviz::SetGraphDefaultValue(const std::string& vn, float v)
{
	ostringstream oss;
	oss << fixed << v;

	agraphattr(mGraph,
		const_cast<char*>(vn.c_str()),
		const_cast<char*>( oss.str().c_str() )
		);
}

void Graphviz::SetGraphDefaultValue(const std::string& vn,
		const std::string& v)
{
	agraphattr(mGraph,
		const_cast<char*>(vn.c_str()),
		const_cast<char*>( v.c_str() )
		);
}

void Graphviz::SetNodeDefaultValue(const std::string& vn, float v)
{
	ostringstream oss;
	oss << fixed << v;

	agnodeattr(mGraph,
		const_cast<char*>(vn.c_str()),
		const_cast<char*>( oss.str().c_str() )
		);
}

void Graphviz::SetNodeDefaultValue(const std::string& vn,
		const std::string& v)
{
	agnodeattr(mGraph,
		const_cast<char*>(vn.c_str()),
		const_cast<char*>( v.c_str() )
		);
}

void Graphviz::SetEdgeDefaultValue(const std::string& vn, float v)
{
	ostringstream oss;
	oss << fixed << v;

	agedgeattr(mGraph,
		const_cast<char*>(vn.c_str()),
		const_cast<char*>( oss.str().c_str() )
		);
}

void Graphviz::SetEdgeDefaultValue(const std::string& vn,
		const std::string& v)
{
	agedgeattr(mGraph,
		const_cast<char*>(vn.c_str()),
		const_cast<char*>( v.c_str() )
		);
}

bool Graphviz::Compute(void)
{
	string render;

	switch (mRender)
	{
		case RFDP:   render = "fdp";   break;
		case RNEATO: render = "neato"; break;
		case RDOT:   render = "dot";   break;
		case RCIRCO: render = "circo"; break;
		case RTWOPI: render = "twopi"; break;
	}

	if ( gvLayout(mGVC, mGraph, const_cast<char*>(render.c_str())) )
	{
		gvFreeLayout(mGVC, mGraph);
		mErrorMsg = "Layout creation failed.";
		mComputed = false;
		return false;
	}
	else
	{
		mComputed = true;
		return true;
	}
}

bool Graphviz::Render(const std::string& fmt, const std::string& filename)
{
	if (!mComputed)
	{
		mErrorMsg = "Not computed !";
		return false;
	}

	// According to Graphviz documentation,
	// the string will never be modified.
	int Result = gvRenderFilename(mGVC, mGraph,
		const_cast<char*>(fmt.c_str()),
		const_cast<char*>(filename.c_str())
		);

	if (Result)
	{
		mErrorMsg = "Render failed.";
		return false;
	}
	else
	{
		return true;
	}
}

const Agnode_t* Graphviz::AddTable(const std::string& n,
		ETableStyle s/*Entity*/)
{
	// According to Graphviz documentation,
	// the string will never be modified.
	Agnode_t* node = agnode(mGraph, const_cast<char*>(n.c_str()) );

	string label, S, E;
	
	switch (mRender)
	{
		case RFDP:
		case RNEATO:
		case RCIRCO:
		case RTWOPI:
			S = "{";
			E = "}";
			break;

		case RDOT:
			S = "{{";
			E = "}}";
			break;
	}

	// Set style.
	switch (s)
	{
		case TSEntity:
				label = S + n + "|" + E;
				agsafeset(node, const_cast<char*>("width"), const_cast<char*>("1.5"), const_cast<char*>(""));
				agsafeset(node, const_cast<char*>("fillcolor"),
					const_cast<char*>(mColorEntity.c_str()), const_cast<char*>(""));
				break;

		case TSAssoc:
				label = S + n + "|" + E;
				agsafeset(node, const_cast<char*>("shape"), const_cast<char*>("Mrecord"), const_cast<char*>(""));
				agsafeset(node, const_cast<char*>("fontsize"), const_cast<char*>("8"), const_cast<char*>(""));
				agsafeset(node, const_cast<char*>("width"), const_cast<char*>("1"), const_cast<char*>(""));
				agsafeset(node, const_cast<char*>("fillcolor"),
					const_cast<char*>(mColorAssoc.c_str()), const_cast<char*>(""));
				break;

		case TSTable:
				label = S + n + E;
				agsafeset(node, const_cast<char*>("width"), const_cast<char*>("1.5"), const_cast<char*>(""));
				agsafeset(node, const_cast<char*>("fillcolor"),
					const_cast<char*>(mColorTable.c_str()), const_cast<char*>(""));
				break;
	}

	agsafeset(node, const_cast<char*>("label"),
			const_cast<char*>(label.c_str()),
			const_cast<char*>(""));

	return node;
}

void Graphviz::AddKey(const Agnode_t* n, const std::string& k)
{
	string label(
			agget(const_cast<Agnode_t*>(n), const_cast<char*>("label"))
			);

	string e;
	switch (mRender)
	{
		case RFDP:
		case RNEATO:
		case RCIRCO:
		case RTWOPI:
			e = "}";
			label.erase(label.size()-1);
			break;

		case RDOT:
			e = "}}";
			label.erase(label.size()-2);
			break;
	}

	label.append("[u]" + k + "[/u]\\l%" + e);

	agsafeset(const_cast<Agnode_t*>(n),
			const_cast<char*>("label"),
			const_cast<char*>(label.c_str()),
			const_cast<char*>(""));
}

void Graphviz::AddKey(const Agnode_t* n, const std::string& k, const std::string& id)
{
	string label(
			agget(const_cast<Agnode_t*>(n), const_cast<char*>("label"))
			);

	string e;
	switch (mRender)
	{
		case RNEATO:
		case RFDP:
		case RCIRCO:
		case RTWOPI:
			e = "}";
			label.erase(label.size()-1);
			break;

		case RDOT:
			e = "}}";
			label.erase(label.size()-2);
			break;
	}

	label.append("|<" + id + ">[u]" + k + "[/u]\\l%" + e);

	agsafeset(const_cast<Agnode_t*>(n),
			const_cast<char*>("label"),
			const_cast<char*>(label.c_str()),
			const_cast<char*>(""));
}

void Graphviz::AddField(const Agnode_t* n, const std::string& f)
{
	string label(
			agget(const_cast<Agnode_t*>(n), const_cast<char*>("label"))
			);
	
	string e;
	switch (mRender)
	{
		case RNEATO:
		case RFDP:
		case RCIRCO:
		case RTWOPI:
			e = "}";
			label.erase(label.size()-1);
			break;

		case RDOT:
			e = "}}";
			label.erase(label.size()-2);
			break;
	}

	label.append(f + "\\l" + e);

	agsafeset(const_cast<Agnode_t*>(n),
			const_cast<char*>("label"),
			const_cast<char*>(label.c_str()),
			const_cast<char*>(""));
}

void Graphviz::AddField(const Agnode_t* n, const std::string& f, const std::string& id)
{
	string label(
			agget(const_cast<Agnode_t*>(n), const_cast<char*>("label"))
			);
	
	string e;
	switch (mRender)
	{
		case RNEATO:
		case RFDP:
		case RCIRCO:
		case RTWOPI:
			e = "}";
			label.erase(label.size()-1);
			break;

		case RDOT:
			e = "}}";
			label.erase(label.size()-2);
			break;
	}

	label.append("|<" + id + ">" + f + "\\l%" + e);

	agsafeset(const_cast<Agnode_t*>(n),
			const_cast<char*>("label"),
			const_cast<char*>(label.c_str()),
			const_cast<char*>(""));
}

void Graphviz::AddLink(
		const Agnode_t* Start,
		const Agnode_t* Dest,
		const std::string& l)
{
	// Yes, Graphviz will be probably modify
	// Start and Dest. Why use const ?
	// Simply because Graphviz:: client must not be
	// modifie node !
	Agedge_t* e = agedge(mGraph,
			const_cast<Agnode_t*>(Start),
			const_cast<Agnode_t*>(Dest)
			);

	agsafeset(e, const_cast<char*>("label"), const_cast<char*>(l.c_str()), const_cast<char*>(""));
	agsafeset(e, const_cast<char*>("arrowhead"), const_cast<char*>("none"), const_cast<char*>(""));
}

void Graphviz::AddLink(const Agnode_t* Start,
		const Agnode_t* Dest,
		const std::string& tp,
		const std::string& hp)
{
	// Yes, Graphviz will be probably modify
	// Start and Dest. Why use const ?
	// Simply because Graphviz:: client must not be
	// modifie node !
	Agedge_t* e = agedge(mGraph,
			const_cast<Agnode_t*>(Start),
			const_cast<Agnode_t*>(Dest)
			);

	string s;

	s = tp + ":e";
	agsafeset(e, const_cast<char*>("tailport"), const_cast<char*>(s.c_str()), const_cast<char*>(""));

	s = hp + ":w";
	agsafeset(e, const_cast<char*>("headport"), const_cast<char*>(s.c_str()), const_cast<char*>(""));
	
	if (mUseColor)
		agsafeset(e, const_cast<char*>("color"), const_cast<char*>(NextColor()), const_cast<char*>(""));
	
	agsafeset(e, const_cast<char*>("arrowtail"), const_cast<char*>("dot"), const_cast<char*>(""));
}

void Graphviz::SetColor(ETableStyle s, const std::string& c)
{
	switch (s)
	{
		case TSEntity:
			mColorEntity = c;
			break;

		case TSAssoc:
			mColorAssoc = c;
			break;

		case TSTable:
			mColorTable = c;
			break;
	}
}

}
