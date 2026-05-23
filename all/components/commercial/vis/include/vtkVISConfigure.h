/* 
 * Here is where system computed values get stored.
 * These values should only change when the target compile platform changes.
 */

#define VTKVIS_BUILD_SHARED_LIBS
#ifndef VTKVIS_BUILD_SHARED_LIBS
#define VTKVIS_STATIC
#endif

#if defined(_MSC_VER) && !defined(VTKVIS_STATIC)
#pragma warning ( disable : 4275 )
#endif
