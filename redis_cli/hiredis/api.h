#ifndef __API_H
#define __API_H

#ifdef WIN32
#ifdef HIREDIS_EXPORTS
#define HIREDIS_API __declspec(dllexport)
#else
#define HIREDIS_API __declspec(dllimport)
#endif
#else
#define HIREDIS_API
#endif

#endif
