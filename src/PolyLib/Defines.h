#pragma once

#if defined WIN32
# if defined POLYLIB_EXPORTS
#   define PolyIntersection_EXPORT __declspec( dllexport )
# else
#   define PolyIntersection_EXPORT __declspec( dllimport )
# endif
#else
# define PolyIntersection_EXPORT
#endif
