#ifdef VS_WIN8
#include <SDL_main.h>
#else
#include <SDL/SDL_main.h>
#endif

#include <wrl.h>

/* Prevent MSVC++ from warning about threading models when defining our
custom WinMain.  The threading model will instead be set via a direct
call to Windows::Foundation::Initialize (rather than via an attributed
function).

To note, this warning (C4447) does not seem to come up unless this file
is compiled with C++/CX enabled (via the /ZW compiler flag).
*/
#ifdef _MSC_VER
#pragma warning(disable:4447)
#endif

/* Make sure the function to initialize the Windows Runtime gets linked in. */
#ifdef _MSC_VER
#pragma comment(lib, "runtimeobject.lib")
#endif
