/*
============================================================================
 Name        : PlatformLib.cpp
 Author      : yanbo
 Version     :
 Copyright   : Your copyright notice
 Description : PlatformLib.cpp - source file
============================================================================
*/

// INCLUDE FILES
// Class include
#include "PlatformLib.h"

// ================= MEMBER FUNCTIONS =======================
namespace util
{
    const double PI = 3.1415926535897931;

	static LUint8 *LItoa(LInt value, LUint8 *str, LInt radix)
	{ 
		static LUint8 szMap[] = { 
			'0', '1', '2', '3', '4', '5', 
			'6', '7', '8', '9', 'a', 'b', 
			'c', 'd', 'e', 'f', 'g', 'h', 
			'i', 'j', 'k', 'l', 'm', 'n', 
			'o', 'p', 'q', 'r', 's', 't', 
			'u', 'v', 'w', 'x', 'y', 'z'
		}; 
		LInt nCount = -1, nIndex = 0; 
		LUint8 *pStr = str, nTemp = 0; 
		if (radix >= 2 && radix <= 36) 
		{   
			if (value < 0 && radix == 10) 
			{   
				*pStr++ = '-';
				value = -1 * value;
			} 

			LUint nValue = (LUint)value;
			do
			{
				pStr[++nCount] = szMap[nValue % radix];
				nValue /= radix; 
			} while(nValue > 0);

			nIndex = (nCount + 1) / 2;
			while (nIndex-- > 0)
			{
				nTemp = pStr[nIndex]; 
				pStr[nIndex] = pStr[nCount - nIndex]; 
				pStr[nCount - nIndex] = nTemp; 
			} 
		} 
		pStr[nCount + 1] = '\0';
		return str;
	} 

    LInt LStrlen(const LUint8* aStr)
	{
		LInt nLen = 0;
		LUint8* p = (LUint8*)aStr;
        while (*p != '\0')
        {
            p++;
			nLen++;
        }
		
		return nLen;
	}
    
    LInt LStrcmp(const LCharA* src, const LCharA* cmpStr)
    {
    	LCharA* src1 = (LCharA*)src;
    	LCharA* cmpStr1 = (LCharA*)cmpStr;
    	
        LInt srcLen = LStrlen((const LUint8*)src);
        LInt cmpStrLen = LStrlen((const LUint8*)cmpStr);
        
        LInt min = LMin(srcLen, cmpStrLen);        
        LInt ret = 0;
        for (LInt i=0; i < min; i++)
        {
            if (*(src1+i) > *(cmpStr1+i))
            {
                ret = 1;
                break;
            }
            else if (*(src1+i) < *(cmpStr1+i))
            {            
                ret = -1;
                break;
            }
        }
        
        if (ret == 0)
        {
            if(srcLen > cmpStrLen)
            {
                ret = 1;
            }
            
            if(srcLen < cmpStrLen)
            {
                ret = -1;
            }
        }
        
        return ret;
    }
    
    LInt LStrncmp(const LCharA* s1, const LCharA* s2, LInt n)
    {	
    	LCharA* ss1 = (LCharA*)s1;
    	LCharA* ss2 = (LCharA*)s2;
    	if (!n)
    	{
    	    return 0;
    	}

        while (--n && *ss1 && *ss1 == *ss2)
        {
            ss1++,
            ss2++;
        }
    	return (*(LUint8 *)ss1 - *(LUint8 *)ss2);
    }
    
    // case-insensitive
    LInt LStrincmp(const LCharA* src, const LCharA* cmpStr, LInt maxlen)
    {
    	LCharA ch1,ch2; 
        for (int i = 0; i < maxlen; i++)
        { 
            ch1 = src[i];
            
            if (ch1 >= 'a' && ch1 <= 'z')
            {
                ch1 -= 'a' - 'A';
            }
            
            ch2 = cmpStr[i];
            if (ch2 >= 'a' && ch2 <= 'z')
            {
                ch2 -= 'a' - 'A'; 
            }
                    
            if (ch1 != ch2)
            { 
                 if (ch1 > ch2)
                     return 1; 
                 else   
                     return -1; 
            }
            else 
            { 
                 if (ch1 == 0)
                    return 0; 
                 else   
                    continue; 
            } 
        }
        
        return 0;
    }

	LInt8* LStrcpy(LInt8* aDest, const LInt8* aSrc)
	{
		//LInt8 *d = aDest;
		//LInt8 *s = aSrc;
		while (*aSrc != '\0')
        {
			*(aDest++) = *(aSrc++);
        }

		*aDest = '\0';
		return aDest;
	}

	LVoid* LMemset(LVoid* aDest, LInt aValue, LInt aLength)
	{
        LUint8* p = (LUint8*)aDest;
        for(LInt i = 0; i<aLength; i++,p++)
		{
            *p = (LUint8)aValue;
		}

		return aDest;
	}

	LVoid* LMemcpy(LVoid* aDest, const LVoid* aSrc, LInt aLength)
	{
		LUint8 *d = (LUint8*)aDest;
		LUint8 *s = (LUint8*)aSrc;
        for (LInt i = 0; i < aLength; i++)
        {
            *d++ = *s++;
        }

		return aDest;
	}
	
	LVoid* LMemmove(LVoid* dest, const LVoid* source, LInt count)
	{
		LUint8 *d = (LUint8*)dest;
		LUint8 *s = (LUint8*)source;
	    if (d <= s || d >= (s + count))
	    {
	         //Non-Overlapping Buffers
	         //copy from lower addresses to higher addresses
	         while (count--)
	               *d++ = *s++;
	     }
	     else
	     {
	       //Overlapping Buffers
	       //copy from higher addresses to lower addresses
	       d += count - 1;
	       s += count - 1;
	       while (count--)
	             *d-- = *s--;

	     }
	    
	     return dest;
	}

	LVoid LStrA2U(const LUint8* aStrA, LUint16* aStrW)
	{
		LUint8 *strA = (LUint8*)aStrA;
		LUint16 *strW = aStrW;
		while (*strA != '\0')
		{
			*(strW++) = *(strA++);
		}

		*strW = 0;
	}

	LVoid LStrU2A(LUint8* aStrA, const LUint16* aStrW)
	{
		LUint8 *strA = aStrA;
		LUint16 *strW = (LUint16*)aStrW;
		while (*strW != 0)
		{
			*strA++ = (LUint8)((*strW++)&0xFF);
		}
	}

	LUint8 *LInt2Str(LInt value, LUint8 *str, LInt radix)
	{
        return LItoa(value, str, radix);
	}
	
	LBool LIsDigit(LInt ch)
	{
	    if (ch <= '9' && ch >= '0')
		{
	        return LTrue;
		}

	    return LFalse;
	}
	
	// only to judge the hex
	LBool LIsBigChar(LInt ch)
	{
	    if (ch <= 'F' && ch >= 'A')
	    {
	        return LTrue;
	    }
	    
	    return LFalse;
	}
	
	LBool LIsMinChar(LInt ch)
	{
	    if (ch <= 'f' && ch >= 'a')
	    {
	        return LTrue;
	    }
	    
	    return LFalse;
	}

	LInt LStr2Int(const LUint8* str, LInt radix)
	{
		LUint8 *p = (LUint8*)str;
		LInt total = 0;
		LInt sign = 1;

		if (*str == '-')
		{
			sign = -1; 
			p++;
		}
		else if (*p == '+')
		{
			p++;
		}

		while (*p)
		{
		    LInt ch = 0;
		    if (LIsDigit(*p))
		    {
		        ch = *p - '0';
		    }
		    else if (LIsBigChar(*p))
		    {
		        ch = *p - 'A' + 10;
		    }
		    else if (LIsMinChar(*p))
		    {
		        ch = *p - 'a' + 10;
		    }

		    total = total * radix + ch;
		    p++;
		}

		return total * sign;
	}
	
	LBool LIsSpace(LInt ch)
	{
		//	if(ch == ' ' || ch == '\t'
        //			|| ch == '\r' || ch == '\n'
        //			|| ch == '\v' || ch == '\f')
        //	{
        //	    return LTrue;
        //	}
        //
        //	return LFalse;
		return (unsigned int)(ch - 9) < 5  ||  ch == ' ';	
	}
	
	const LChar* LSkipSpace(const LChar* str)
	{
	    while (*str && LIsSpace((LInt)(*str)))
	    {
	        ++str;
	    }

	    return str;
	}

	LChar* LSkipString(LChar* src, const LChar* skip)
	{
        int i = 0;

		while (skip[i])
		{
			if (!src[i])
			{
				i = 0;
				break;
			}

			if (src[i] != skip[i])
			{
                i = 0;
				break;
			}

			i++;
		}

		src = src + i;
		return src;
	}
	
	LChar* LStrchr(const LChar *s, LChar c)
	{
		LChar* ptr = (LChar*)s;
		while (*ptr)
		{
		    if ((*ptr) == c)
		    {
		        return ptr;
		    }		
		}
		
		return NULL;
	}
	
	LInt LMemcmp(const LVoid *buffer1,const LVoid *buffer2,LInt count)
    {
        if (!count)
            return (0);

        while (--count && *(LInt8 *)buffer1 == *(LInt8 *)buffer2)
        {
           buffer1 = (LInt8 *)buffer1 + 1;
           buffer2 = (LInt8 *)buffer2 + 1;
        }

        return(*((LUint8 *)buffer1) - *((LUint8 *)buffer2));
     }

	LInt LCeil(LReal64 f)
	{
		if (f > 0.0f)
		{
			return (LInt)f + 1;
		}
		else
		{
			return (LInt)f;
		}
	}

	LInt LFloor(LReal64 f)
	{
		if(f > 0.0f)
		{
			return (LInt)f;
		}
		else
		{
			return (LInt)f - 1;
		}
	}

	LReal64 LRound(LReal64 num)
    {
        LReal64 integer = LCeil(num);
        if (num > 0)
            return integer - num > 0.5 ? integer - 1.0 : integer;
        return integer - num >= 0.5 ? integer - 1.0 : integer;
    }

	LReal toRadians(LReal angle)
	{
		return (angle*PI)/180;
	}

	LReal LFabs(LReal f)
	{
		return (f>0) ? f:(-f);
	}

	LBool LIsAlpha(LInt ch)
	{
		if (ch >= 'a' && ch <= 'z')
		{
			return LTrue;
		}

		if (ch >= 'A' && ch <= 'Z')
		{
			return LTrue;
		}

		return LFalse;
	}
}
// End of File
