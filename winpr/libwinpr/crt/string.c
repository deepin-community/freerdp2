/**
 * WinPR: Windows Portable Runtime
 * String Manipulation (CRT)
 *
 * Copyright 2012 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <stdio.h>
#include <wctype.h>

#include <winpr/crt.h>
#include <winpr/assert.h>
#include <winpr/endian.h>

/* String Manipulation (CRT): http://msdn.microsoft.com/en-us/library/f0151s4x.aspx */

#include "../log.h"
#define TAG WINPR_TAG("crt")

BOOL winpr_str_append(const char* what, char* buffer, size_t size, const char* separator)
{
	const size_t used = strnlen(buffer, size);
	const size_t add = strnlen(what, size);
	const size_t sep_len = separator ? strnlen(separator, size) : 0;
	const size_t sep = (used > 0) ? sep_len : 0;

	if (used + add + sep >= size)
		return FALSE;

	if ((used > 0) && (sep_len > 0))
		strncat(buffer, separator, sep_len);

	strncat(buffer, what, add);
	return TRUE;
}

#ifndef _WIN32

char* _strdup(const char* strSource)
{
	char* strDestination;

	if (strSource == NULL)
		return NULL;

	strDestination = strdup(strSource);

	if (strDestination == NULL)
		WLog_ERR(TAG, "strdup");

	return strDestination;
}

WCHAR* _wcsdup(const WCHAR* strSource)
{
	size_t len = _wcslen(strSource);
	WCHAR* strDestination;

	strDestination = calloc(len + 1, sizeof(WCHAR));

	if (strDestination != NULL)
		memcpy(strDestination, strSource, len * sizeof(WCHAR));

	if (strDestination == NULL)
		WLog_ERR(TAG, "wcsdup");

	return strDestination;
}

int _stricmp(const char* string1, const char* string2)
{
	return strcasecmp(string1, string2);
}

int _strnicmp(const char* string1, const char* string2, size_t count)
{
	return strncasecmp(string1, string2, count);
}

/* _wcscmp -> wcscmp */

int _wcscmp(const WCHAR* string1, const WCHAR* string2)
{
	WINPR_ASSERT(string1);
	WINPR_ASSERT(string2);

	while (TRUE)
	{
		const WCHAR w1 = *string1++;
		const WCHAR w2 = *string2++;

		if (w1 != w2)
			return (int)w1 - w2;
		else if ((w1 == '\0') || (w2 == '\0'))
			return (int)w1 - w2;
	}

	return 0;
}

int _wcsncmp(const WCHAR* string1, const WCHAR* string2, size_t count)
{
	WINPR_ASSERT(string1);
	WINPR_ASSERT(string2);

	for (size_t x = 0; x < count; x++)
	{
		const WCHAR a = string1[x];
		const WCHAR b = string2[x];

		if (a != b)
			return (int)a - b;
		else if ((a == '\0') || (b == '\0'))
			return (int)a - b;
	}
	return 0;
}

/* _wcslen -> wcslen */

size_t _wcslen(const WCHAR* str)
{
	const WCHAR* p = (const WCHAR*)str;

	WINPR_ASSERT(p);

	while (*p)
		p++;

	return (size_t)(p - str);
}

/* _wcsnlen -> wcsnlen */

size_t _wcsnlen(const WCHAR* str, size_t max)
{
	size_t x;

	WINPR_ASSERT(str);

	for (x = 0; x < max; x++)
	{
		if (str[x] == 0)
			return x;
	}

	return x;
}

/* _wcsstr -> wcsstr */

WCHAR* _wcsstr(const WCHAR* str, const WCHAR* strSearch)
{
	WINPR_ASSERT(str);
	WINPR_ASSERT(strSearch);

	if (strSearch[0] == '\0')
		return str;

	const size_t searchLen = _wcslen(strSearch);
	while (*str)
	{
		if (_wcsncmp(str, strSearch, searchLen) == 0)
			return str;
		str++;
	}
	return NULL;
}

/* _wcschr -> wcschr */

WCHAR* _wcschr(const WCHAR* str, WCHAR c)
{
	WCHAR* p = (WCHAR*)str;
	WCHAR value;
	Data_Write_UINT16(&value, c);

	while (*p && (*p != value))
		p++;

	return ((*p == value) ? p : NULL);
}

/* _wcsrchr -> wcsrchr */

WCHAR* _wcsrchr(const WCHAR* str, WCHAR c)
{
	WCHAR* p;
	WCHAR ch;

	if (!str)
		return NULL;

	for (p = (WCHAR*)0; (ch = *str); str++)
		if (ch == c)
			p = (WCHAR*)str;

	return p;
}

char* strtok_s(char* strToken, const char* strDelimit, char** context)
{
	return strtok_r(strToken, strDelimit, context);
}

WCHAR* wcstok_s(WCHAR* strToken, const WCHAR* strDelimit, WCHAR** context)
{
	WCHAR* nextToken;
	WCHAR value;

	if (!strToken)
		strToken = *context;

	Data_Read_UINT16(strToken, value);

	while (*strToken && _wcschr(strDelimit, value))
	{
		strToken++;
		Data_Read_UINT16(strToken, value);
	}

	if (!*strToken)
		return NULL;

	nextToken = strToken++;
	Data_Read_UINT16(strToken, value);

	while (*strToken && !(_wcschr(strDelimit, value)))
	{
		strToken++;
		Data_Read_UINT16(strToken, value);
	}

	if (*strToken)
		*strToken++ = 0;

	*context = strToken;
	return nextToken;
}

#endif

#if !defined(_WIN32) || defined(_UWP)

/* Windows API Sets - api-ms-win-core-string-l2-1-0.dll
 * http://msdn.microsoft.com/en-us/library/hh802935/
 */

#include "casing.c"

LPSTR CharUpperA(LPSTR lpsz)
{
	size_t i;
	size_t length;

	if (!lpsz)
		return NULL;

	length = strlen(lpsz);

	if (length < 1)
		return (LPSTR)NULL;

	if (length == 1)
	{
		char c = *lpsz;

		if ((c >= 'a') && (c <= 'z'))
			c = c - 32;

		*lpsz = c;
		return lpsz;
	}

	for (i = 0; i < length; i++)
	{
		if ((lpsz[i] >= 'a') && (lpsz[i] <= 'z'))
			lpsz[i] = lpsz[i] - 32;
	}

	return lpsz;
}

LPWSTR CharUpperW(LPWSTR lpsz)
{
	WLog_ERR(TAG, "CharUpperW unimplemented!");
	return (LPWSTR)NULL;
}

DWORD CharUpperBuffA(LPSTR lpsz, DWORD cchLength)
{
	DWORD i;

	if (cchLength < 1)
		return 0;

	for (i = 0; i < cchLength; i++)
	{
		if ((lpsz[i] >= 'a') && (lpsz[i] <= 'z'))
			lpsz[i] = lpsz[i] - 32;
	}

	return cchLength;
}

DWORD CharUpperBuffW(LPWSTR lpsz, DWORD cchLength)
{
	DWORD i;
	WCHAR value;

	for (i = 0; i < cchLength; i++)
	{
		Data_Read_UINT16(&lpsz[i], value);
		value = WINPR_TOUPPERW(value);
		Data_Write_UINT16(&lpsz[i], value);
	}

	return cchLength;
}

LPSTR CharLowerA(LPSTR lpsz)
{
	size_t i;
	size_t length;

	if (!lpsz)
		return (LPSTR)NULL;

	length = strlen(lpsz);

	if (length < 1)
		return (LPSTR)NULL;

	if (length == 1)
	{
		char c = *lpsz;

		if ((c >= 'A') && (c <= 'Z'))
			c = c + 32;

		*lpsz = c;
		return lpsz;
	}

	for (i = 0; i < length; i++)
	{
		if ((lpsz[i] >= 'A') && (lpsz[i] <= 'Z'))
			lpsz[i] = lpsz[i] + 32;
	}

	return lpsz;
}

LPWSTR CharLowerW(LPWSTR lpsz)
{
	WLog_ERR(TAG, "CharLowerW unimplemented!");
	return (LPWSTR)NULL;
}

DWORD CharLowerBuffA(LPSTR lpsz, DWORD cchLength)
{
	DWORD i;

	if (cchLength < 1)
		return 0;

	for (i = 0; i < cchLength; i++)
	{
		if ((lpsz[i] >= 'A') && (lpsz[i] <= 'Z'))
			lpsz[i] = lpsz[i] + 32;
	}

	return cchLength;
}

DWORD CharLowerBuffW(LPWSTR lpsz, DWORD cchLength)
{
	DWORD i;
	WCHAR value;

	for (i = 0; i < cchLength; i++)
	{
		Data_Read_UINT16(&lpsz[i], value);
		value = WINPR_TOLOWERW(value);
		Data_Write_UINT16(&lpsz[i], value);
	}

	return cchLength;
}

BOOL IsCharAlphaA(CHAR ch)
{
	if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
		return 1;
	else
		return 0;
}

BOOL IsCharAlphaW(WCHAR ch)
{
	WLog_ERR(TAG, "IsCharAlphaW unimplemented!");
	return 0;
}

BOOL IsCharAlphaNumericA(CHAR ch)
{
	if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) ||
	    ((ch >= '0') && (ch <= '9')))
		return 1;
	else
		return 0;
}

BOOL IsCharAlphaNumericW(WCHAR ch)
{
	WLog_ERR(TAG, "IsCharAlphaNumericW unimplemented!");
	return 0;
}

BOOL IsCharUpperA(CHAR ch)
{
	if ((ch >= 'A') && (ch <= 'Z'))
		return 1;
	else
		return 0;
}

BOOL IsCharUpperW(WCHAR ch)
{
	WLog_ERR(TAG, "IsCharUpperW unimplemented!");
	return 0;
}

BOOL IsCharLowerA(CHAR ch)
{
	if ((ch >= 'a') && (ch <= 'z'))
		return 1;
	else
		return 0;
}

BOOL IsCharLowerW(WCHAR ch)
{
	WLog_ERR(TAG, "IsCharLowerW unimplemented!");
	return 0;
}

int lstrlenA(LPCSTR lpString)
{
	return (int)strlen(lpString);
}

int lstrlenW(LPCWSTR lpString)
{
	LPWSTR p;

	if (!lpString)
		return 0;

	p = (LPWSTR)lpString;

	while (*p)
		p++;

	return (int)(p - lpString);
}

int lstrcmpA(LPCSTR lpString1, LPCSTR lpString2)
{
	return strcmp(lpString1, lpString2);
}

int lstrcmpW(LPCWSTR lpString1, LPCWSTR lpString2)
{
	WCHAR value1, value2;

	while (*lpString1 && (*lpString1 == *lpString2))
	{
		lpString1++;
		lpString2++;
	}

	Data_Read_UINT16(lpString1, value1);
	Data_Read_UINT16(lpString2, value2);
	return value1 - value2;
}

#endif

int ConvertLineEndingToLF(char* str, int size)
{
	int status;
	char* end;
	char* pInput;
	char* pOutput;
	end = &str[size];
	pInput = pOutput = str;

	while (pInput < end)
	{
		if ((pInput[0] == '\r') && (pInput[1] == '\n'))
		{
			*pOutput++ = '\n';
			pInput += 2;
		}
		else
		{
			*pOutput++ = *pInput++;
		}
	}

	status = (int)(pOutput - str);
	return status;
}

char* ConvertLineEndingToCRLF(const char* str, int* size)
{
	int count;
	char* newStr;
	char* pOutput;
	const char* end;
	const char* pInput;
	end = &str[*size];
	count = 0;
	pInput = str;

	while (pInput < end)
	{
		if (*pInput == '\n')
			count++;

		pInput++;
	}

	newStr = (char*)malloc(*size + (count * 2) + 1);

	if (!newStr)
		return NULL;

	pInput = str;
	pOutput = newStr;

	while (pInput < end)
	{
		if ((*pInput == '\n') && ((pInput > str) && (pInput[-1] != '\r')))
		{
			*pOutput++ = '\r';
			*pOutput++ = '\n';
		}
		else
		{
			*pOutput++ = *pInput;
		}

		pInput++;
	}

	*size = (int)(pOutput - newStr);
	return newStr;
}

char* StrSep(char** stringp, const char* delim)
{
	char* start = *stringp;
	char* p;
	p = (start != NULL) ? strpbrk(start, delim) : NULL;

	if (!p)
		*stringp = NULL;
	else
	{
		*p = '\0';
		*stringp = p + 1;
	}

	return start;
}

INT64 GetLine(char** lineptr, size_t* size, FILE* stream)
{
#if defined(_WIN32)
	char c;
	char* n;
	size_t step = 32;
	size_t used = 0;

	if (!lineptr || !size)
	{
		errno = EINVAL;
		return -1;
	}

	do
	{
		if (used + 2 >= *size)
		{
			*size += step;
			n = realloc(*lineptr, *size);

			if (!n)
			{
				return -1;
			}

			*lineptr = n;
		}

		c = fgetc(stream);

		if (c != EOF)
			(*lineptr)[used++] = c;
	} while ((c != '\n') && (c != '\r') && (c != EOF));

	(*lineptr)[used] = '\0';
	return used;
#elif !defined(ANDROID) && !defined(IOS)
	return getline(lineptr, size, stream);
#else
	return -1;
#endif
}
