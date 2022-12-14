#ifndef H_EZDEVSDK_DOMAIN_DEF_H_
#define H_EZDEVSDK_DOMAIN_DEF_H_

#if defined(_WIN32) || defined(_WIN64)

#ifdef GLOBAL_DECL_EXPORTS
#define GLOBAL_API __declspec(dllexport)
#else //GLOBAL_DECL_EXPORTS
#define GLOBAL_API __declspec(dllimport)
#endif //GLOBAL_DECL_EXPORTS

#define GLOBAL_CALLBACK __stdcall

#else
#define GLOBAL_API

#define GLOBAL_CALLBACK
#endif

#endif //H_EZDEVSDK_DOMAIN_DEF_H_