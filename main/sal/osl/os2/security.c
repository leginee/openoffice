/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/




#include "system.h"

#include <osl/security.h>
#include <osl/diagnose.h>
#include <osl/module.h>

#include "osl/thread.h"
#include "osl/file.h"

#ifdef SOLARIS
#include <crypt.h>
#endif

#include "secimpl.h"

#ifndef PAM_BINARY_MSG
#define PAM_BINARY_MSG 6
#endif

extern oslModule SAL_CALL osl_psz_loadModule(const sal_Char *pszModuleName, sal_Int32 nRtldMode);
extern void* SAL_CALL osl_psz_getSymbol(oslModule hModule, const sal_Char* pszSymbolName);
extern oslSecurityError SAL_CALL 
osl_psz_loginUser(const sal_Char* pszUserName, const sal_Char* pszPasswd,
                  oslSecurity* pSecurity);
sal_Bool SAL_CALL osl_psz_getUserIdent(oslSecurity Security, sal_Char *pszIdent, sal_uInt32 nMax);
sal_Bool SAL_CALL osl_psz_getUserName(oslSecurity Security, sal_Char* pszName, sal_uInt32  nMax);
sal_Bool SAL_CALL osl_psz_getHomeDir(oslSecurity Security, sal_Char* pszDirectory, sal_uInt32 nMax);
sal_Bool SAL_CALL osl_psz_getConfigDir(oslSecurity Security, sal_Char* pszDirectory, sal_uInt32 nMax);



oslSecurity SAL_CALL osl_getCurrentSecurity()
{
	
	oslSecurityImpl *pSecImpl = (oslSecurityImpl*) malloc(sizeof(oslSecurityImpl));
	struct passwd	*pPasswd  = getpwuid(getuid());

	if (pPasswd)
	{
		memcpy(&pSecImpl->m_pPasswd, pPasswd, sizeof(pSecImpl->m_pPasswd));
		pSecImpl->m_isValid = sal_True;
	}
	else
	{
		/* Some UNIX-OS don't implement getpwuid, e.g. NC OS (special NetBSD) 1.2.1 */
		/* so we have to catch this in this else branch */
		pSecImpl->m_pPasswd.pw_name		= getenv("USER");
		pSecImpl->m_pPasswd.pw_dir		= getenv("HOME");
		if (pSecImpl->m_pPasswd.pw_name && pSecImpl->m_pPasswd.pw_dir)
			pSecImpl->m_isValid				= sal_True;
		else 
		{
			pSecImpl->m_pPasswd.pw_name		= "unknown";
			pSecImpl->m_pPasswd.pw_dir		= "/tmp";
			pSecImpl->m_isValid				= sal_False;
		}
		pSecImpl->m_pPasswd.pw_passwd	= NULL;
		pSecImpl->m_pPasswd.pw_uid		= getuid();
		pSecImpl->m_pPasswd.pw_gid		= getgid();
		pSecImpl->m_pPasswd.pw_gecos	= "unknown";
		pSecImpl->m_pPasswd.pw_shell	= "unknown";
	}


	return ((oslSecurity)pSecImpl);
}


oslSecurityError SAL_CALL osl_loginUser(
    rtl_uString *ustrUserName,
	rtl_uString *ustrPassword,
	oslSecurity *pSecurity
	)
{
	oslSecurityError ret;

	*pSecurity = osl_getCurrentSecurity();
	ret = osl_Security_E_None;

    return ret;
}

    

oslSecurityError SAL_CALL osl_loginUserOnFileServer(
    rtl_uString *strUserName,
	rtl_uString *strPasswd,
	rtl_uString *strFileServer,
	oslSecurity *pSecurity
	)
{
	oslSecurityError erg;
	return erg = osl_Security_E_UserUnknown;   
}


oslSecurityError SAL_CALL osl_psz_loginUserOnFileServer( const sal_Char*  pszUserName,
                                                     const sal_Char*  pszPasswd,
                                                     const sal_Char*  pszFileServer,
                                                     oslSecurity*     pSecurity )
{
	oslSecurityError erg;
	return erg = osl_Security_E_UserUnknown;
}

sal_Bool SAL_CALL osl_getUserIdent(oslSecurity Security, rtl_uString **ustrIdent)
{
    sal_Bool bRet=sal_False;
    sal_Char pszIdent[1024];

    pszIdent[0] = '\0';
    
    bRet = osl_psz_getUserIdent(Security,pszIdent,sizeof(pszIdent));

    rtl_string2UString( ustrIdent, pszIdent, rtl_str_getLength( pszIdent ), osl_getThreadTextEncoding(), OUSTRING_TO_OSTRING_CVTFLAGS );
    OSL_ASSERT(*ustrIdent != NULL);
    
    return bRet;    
}


sal_Bool SAL_CALL osl_psz_getUserIdent(oslSecurity Security, sal_Char *pszIdent, sal_uInt32 nMax)
{
	sal_Char  buffer[32];
    sal_Int32 nChr;

	oslSecurityImpl *pSecImpl = (oslSecurityImpl *)Security;

    if (pSecImpl == NULL)
	    return sal_False;

	nChr = snprintf(buffer, sizeof(buffer), "%u", pSecImpl->m_pPasswd.pw_uid);
    if ( nChr < 0 || nChr >= sizeof(buffer) || nChr >= nMax )
        return sal_False; /* leave *pszIdent unmodified in case of failure */

	memcpy(pszIdent, buffer, nChr+1);
    return sal_True;
}

sal_Bool SAL_CALL osl_getUserName(oslSecurity Security, rtl_uString **ustrName)
{
    sal_Bool bRet=sal_False;
    sal_Char pszName[1024];

    pszName[0] = '\0';
    
    bRet = osl_psz_getUserName(Security,pszName,sizeof(pszName));
    
    rtl_string2UString( ustrName, pszName, rtl_str_getLength( pszName ), osl_getThreadTextEncoding(), OUSTRING_TO_OSTRING_CVTFLAGS );
    OSL_ASSERT(*ustrName != NULL);
    
    return bRet;
}


 
sal_Bool SAL_CALL osl_psz_getUserName(oslSecurity Security, sal_Char* pszName, sal_uInt32  nMax)
{
	oslSecurityImpl *pSecImpl = (oslSecurityImpl *)Security;

    if ((pSecImpl == NULL) || (! pSecImpl->m_isValid))
	    return sal_False;

	strncpy(pszName, pSecImpl->m_pPasswd.pw_name, nMax);

    return sal_True;
}

sal_Bool SAL_CALL osl_getHomeDir(oslSecurity Security, rtl_uString **pustrDirectory)
{
    sal_Bool bRet=sal_False;
    sal_Char pszDirectory[PATH_MAX];

    pszDirectory[0] = '\0';
    
    bRet = osl_psz_getHomeDir(Security,pszDirectory,sizeof(pszDirectory));

    if ( bRet == sal_True )
    {
        rtl_string2UString( pustrDirectory, pszDirectory, rtl_str_getLength( pszDirectory ), osl_getThreadTextEncoding(), OUSTRING_TO_OSTRING_CVTFLAGS );
        OSL_ASSERT(*pustrDirectory != NULL);
        osl_getFileURLFromSystemPath( *pustrDirectory, pustrDirectory );
    }
    
    return bRet;
}


sal_Bool SAL_CALL osl_psz_getHomeDir(oslSecurity Security, sal_Char* pszDirectory, sal_uInt32 nMax)
{
	oslSecurityImpl *pSecImpl = (oslSecurityImpl *)Security;

    if (pSecImpl == NULL)
	    return sal_False;

	/* if current user, check also environment for HOME */
	if (getuid() == pSecImpl->m_pPasswd.pw_uid)
	{
		sal_Char *pStr = NULL;
#ifdef SOLARIS
		char	buffer[8192];

		struct passwd pwd;
		struct passwd *ppwd;

#ifdef _POSIX_PTHREAD_SEMANTICS
		if ( 0 != getpwuid_r(getuid(), &pwd, buffer, sizeof(buffer), &ppwd ) )
			ppwd = NULL;
#else
		ppwd = getpwuid_r(getuid(), &pwd, buffer, sizeof(buffer) );
#endif

		if ( ppwd )
			pStr = ppwd->pw_dir;
#else
		pStr = getenv("HOME");
#endif

		if ((pStr != NULL) && (strlen(pStr) > 0) &&
		    (access(pStr, 0) == 0))
			strncpy(pszDirectory, pStr, nMax);
		else
    		if (pSecImpl->m_isValid)
				strncpy(pszDirectory, pSecImpl->m_pPasswd.pw_dir, nMax);
			else
				return sal_False;
	}
	else
		strncpy(pszDirectory, pSecImpl->m_pPasswd.pw_dir, nMax);

	return sal_True;
}

sal_Bool SAL_CALL osl_getConfigDir(oslSecurity Security, rtl_uString **pustrDirectory)
{
    sal_Bool bRet = sal_False;
    sal_Char pszDirectory[PATH_MAX];

    pszDirectory[0] = '\0';
    
    bRet = osl_psz_getConfigDir(Security,pszDirectory,sizeof(pszDirectory));

    if ( bRet == sal_True )
    {
        rtl_string2UString( pustrDirectory, pszDirectory, rtl_str_getLength( pszDirectory ), osl_getThreadTextEncoding(), OUSTRING_TO_OSTRING_CVTFLAGS );
        OSL_ASSERT(*pustrDirectory != NULL);
        osl_getFileURLFromSystemPath( *pustrDirectory, pustrDirectory );
    }

    return bRet;
}


sal_Bool SAL_CALL osl_psz_getConfigDir(oslSecurity Security, sal_Char* pszDirectory, sal_uInt32 nMax)
{
	return (osl_psz_getHomeDir(Security, pszDirectory, nMax));
}

sal_Bool SAL_CALL osl_isAdministrator(oslSecurity Security)
{
	oslSecurityImpl *pSecImpl = (oslSecurityImpl *)Security;

    if (pSecImpl == NULL)
	    return sal_False;

    if (pSecImpl->m_pPasswd.pw_uid != 0)
		return (sal_False);

	return (sal_True);
}

void SAL_CALL osl_freeSecurityHandle(oslSecurity Security)
{
	if (Security)
		free ((oslSecurityImpl*)Security);
}


sal_Bool SAL_CALL osl_loadUserProfile(oslSecurity Security)
{
    return sal_False;
}

void SAL_CALL osl_unloadUserProfile(oslSecurity Security)
{
    return;
}


