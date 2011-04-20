/*
 *  Modulation.h
 *  GulpGulp
 *
 *  Created by Josh Whelchel on 11/19/09.
 *  Copyright 2009 University of Cincinnati. All rights reserved.
 *
 */

#pragma once

class Modulation {
	
public:
	
	Modulation( int numMods );
	~Modulation();
	
	float getModValue( int index );
	void setModValue( int index, float value );
	void incModValue( int index, float mval );
	void mulModValue( int index, float mval );
	
	void step( int index );
	void stepAll();
	
private:
	
	float *m_fModVals;
	int m_nMods;
	
};