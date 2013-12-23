
#include "stringlight.h"

void _strncpySpace(char* pStr1, const char* pStr2, unsigned long nCount)
{
// customized strncpy() function to fill remaing destination string part with spaces

    while (*pStr2 && nCount)
    {
        *pStr1++ = *pStr2++; // copy strings
        nCount--;
    }

    while (nCount--)
        *pStr1++ = ' '; // fill remaining space with spaces

}

// safe, copies bufsize-1 max and adds terminator
void _strlcpy(char* dst, const char* src, unsigned long bufsize)
{
  unsigned long srclen =strlen(src);
  unsigned long result =srclen; /* Result is always the length of the src string */
  if(bufsize>0) {
    if(srclen>=bufsize)
       srclen=bufsize-1;
    if(srclen>0)
       memcpy(dst,src,srclen);
    dst[srclen]='\0';
  }
  //return result;
}

int _strnicmp(const char *pS1, const char *pS2, unsigned long n)
{
    char c1, c2;
    int v;

    do
    {
        c1 = *pS1++;
        c2 = *pS2++;
        v = (unsigned int)tolower(c1) - (unsigned int)tolower(c2);
    }
    while ((v == 0) && (c1 != '\0') && (c2 != '\0') && (--n > 0));

    return v;
}

unsigned int _htoi (const char *pStr)
{
  unsigned int value = 0;
  char ch = *pStr;

  while (ch == ' ' || ch == '\t')
    ch = *(++pStr);

  // remove 0x
  if (ch == '0') ch = *(++pStr);
  if (ch == 'x' || ch == 'X') ch = *(++pStr);

  for (;;) {
    if (ch >= '0' && ch <= '9')
       value = (value << 4) + (ch - '0');
    else if (ch >= 'A' && ch <= 'F')
       value = (value << 4) + (ch - 'A' + 10);
    else if (ch >= 'a' && ch <= 'f')
       value = (value << 4) + (ch - 'a' + 10);
    else
       return value;
    ch = *(++pStr);
  }
}

void FileDisplayName(char *name, uint16_t name_len, char *path) // name_len includes /0
{
  uint16_t i = (uint16_t) strlen(path);

  while(i != 0) {
    if(path[i] == '\\' || path[i] == '/') {
      break;
    }
    i--;
  }
  _strncpySpace(name, (path + i + 1), name_len);
  name[name_len-1] = '\0';
}

// maybe not the best place for this - from FF demo
void FF_ExpandPath(char *acPath) {

    char        *pRel           = 0;
    char        *pRelStart      = 0;
    char        *pRelEnd        = 0;

    int         charRef         = 0;
    int         lenPath         = 0;
    int         lenRel          = 0;
    int         i                       = 0;
    int         remain          = 0;


    lenPath = strlen(acPath);
    pRel = strstr(acPath, "..");

    while(pRel) {       // Loop removal of Relativity
        charRef = pRel - acPath;

        /*
            We have found some relativity in the Path,
        */

        // Where it ends:

        if(pRel[2] == '\\' || pRel[2] == '/') {
            pRelEnd = pRel + 3;
        } else {
            pRelEnd = pRel + 2;
        }

        // Where it Starts:

        if(charRef == 1) {      // Relative Path comes after the root /
            return;     // Fixed, returns false appropriately, as in the TODO: above!
        } else {
            for(i = (charRef - 2); i >= 0; i--) {
                if(acPath[i] == '\\' || acPath[i] == '/') {
                    pRelStart = (acPath + (i + 1));
                    break;
                }
            }
        }

        // The length of the relativity
        lenRel = pRelEnd - pRelStart;

        remain = lenPath - (pRelEnd - acPath);  // Remaining Chars on the end of the path

        if(lenRel) {

            strncpy(pRelStart, pRelEnd, remain);
            pRelStart[remain] = '\0';
        }

        lenPath -= lenRel;
        pRel = strstr(acPath, "..");
    }
}


