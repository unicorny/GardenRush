/*/*************************************************************************
 *                                                                         *
 * Core, Core-Lib for my programs, Core doesn't need any libraries	   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski                           *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.                                                      *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/
//---------------------------------------------------------
//	hashing header file
// aus dem Buch "Goldene Regeln der Spieleprogrammierung" von Martin Brownlow
//---------------------------------------------------------
#ifndef __DR_CORE2_HASH__
#define __DR_CORE2_HASH__

typedef unsigned int HASH;
typedef unsigned long DHASH;

//---------------------------------------------------------
// sample hash creation functions
//---------------------------------------------------------
inline HASH DRHashRotateLeft( HASH hash, const unsigned int rotateBy )
{
    return (hash<<rotateBy)|(hash>>(32-rotateBy));
}

//---------------------------------------------------------
// create a hash from a string
//---------------------------------------------------------
inline HASH DRMakeStringHash( const char *pString )
{
	HASH			ret = 0;
	char			c;

	if( pString )
	{
		while( (c=*(pString++)) )
			ret = DRHashRotateLeft(ret,7) + c;
	}
	return ret;
}


//---------------------------------------------------------
// create a hash from a filename
//---------------------------------------------------------
inline HASH DRMakeFilenameHash( const char *pString )
{
	HASH			ret = 0;
	char			c;

	if( pString )
	{
		while( (c=*(pString++) ) )
		{
			if( c>='A' && c<='Z' )
				c ^= 32;
			else if( c=='/' )
				c = '\\';
			ret = DRHashRotateLeft(ret,7) + c;
		}
	}
	return ret;
}

//-----------------------------------------------------------------------------------
//einen Hash aus zwei Werten erstellen
inline DHASH DRMakeDoubleHash(const char* pTypeName, const char* pFileName)
{
	HASH hTemp1 = DRMakeStringHash(pTypeName);
	DHASH hTemp2 = DRMakeFilenameHash(pFileName);
	hTemp2 = hTemp2 >>16;
	return ((DHASH)(hTemp1)|(DHASH)(hTemp2));
}
//einen Hash aus 3 Int-Werten generiren
inline DHASH DRMakeThreeIntHash(int i1, int i2, int i3)
{
	DHASH h1 = DRHashRotateLeft(i1, 24);
	h1 = h1 | DRHashRotateLeft(i2, 16);
	return h1 | i3;
}

#endif //__DR_CORE2_HASH__
