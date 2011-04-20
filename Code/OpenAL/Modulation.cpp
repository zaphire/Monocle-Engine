/*
 *  Modulation.cpp
 *  GulpGulp
 *
 *  Created by Josh Whelchel on 11/19/09.
 *  Copyright 2009 jwmusic.org. All rights reserved.
 *
 */

#include "Modulation.h"

Modulation::Modulation( int nNumVals )
{
	m_nMods = (nNumVals>=0)?nNumVals:0;
	m_fModVals = (float*)new float[m_nMods];
}

Modulation::~Modulation()
{
	delete m_fModVals;
}

float Modulation::getModValue( int index )
{
	return (index<m_nMods&&index>=0)?m_fModVals[index]:0.0;
}

void Modulation::setModValue( int index, float value )
{
	if (index>=m_nMods||index<0) return;	
	m_fModVals[index]=value;
}

void Modulation::incModValue( int index, float mval )
{
	if (index>=m_nMods||index<0) return;	
	m_fModVals[index] += mval;
}

void Modulation::mulModValue( int index, float mval )
{
	if (index>=m_nMods||index<0) return;	
	m_fModVals[index] *= mval;
}

void Modulation::step( int index )
{
	if (index>=m_nMods||index<0) return;	
}

void Modulation::stepAll()
{
	for (int i=0;i<m_nMods;i++)
	{
		step(i);
	}
}
