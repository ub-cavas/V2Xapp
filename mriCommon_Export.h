
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl mriCommon
// ------------------------------
#ifndef MRICOMMON_EXPORT_H
#define MRICOMMON_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (MRICOMMON_HAS_DLL)
#  define MRICOMMON_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && MRICOMMON_HAS_DLL */

#if !defined (MRICOMMON_HAS_DLL)
#  define MRICOMMON_HAS_DLL 1
#endif /* ! MRICOMMON_HAS_DLL */

#if defined (MRICOMMON_HAS_DLL) && (MRICOMMON_HAS_DLL == 1)
#  if defined (MRICOMMON_BUILD_DLL)
#    define mriCommon_Export ACE_Proper_Export_Flag
#    define MRICOMMON_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MRICOMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MRICOMMON_BUILD_DLL */
#    define mriCommon_Export ACE_Proper_Import_Flag
#    define MRICOMMON_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MRICOMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MRICOMMON_BUILD_DLL */
#else /* MRICOMMON_HAS_DLL == 1 */
#  define mriCommon_Export
#  define MRICOMMON_SINGLETON_DECLARATION(T)
#  define MRICOMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MRICOMMON_HAS_DLL == 1 */

// Set MRICOMMON_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (MRICOMMON_NTRACE)
#  if (ACE_NTRACE == 1)
#    define MRICOMMON_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define MRICOMMON_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !MRICOMMON_NTRACE */

#if (MRICOMMON_NTRACE == 1)
#  define MRICOMMON_TRACE(X)
#else /* (MRICOMMON_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define MRICOMMON_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (MRICOMMON_NTRACE == 1) */

#endif /* MRICOMMON_EXPORT_H */

// End of auto generated file.
