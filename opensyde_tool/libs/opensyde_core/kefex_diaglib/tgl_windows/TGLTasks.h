//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       DiagLib Target Glue Layer: Task system related functionality.

   DiagLib Target Glue Layer module containing task system related functionality.

   \copyright   Copyright 2009 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef TGLTASKSH
#define TGLTASKSH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <windows.h>
#include "stwtypes.h"
#include "SCLDynamicArray.h"

#ifndef TGL_PACKAGE
#ifdef __BORLANDC__
#define TGL_PACKAGE __declspec(package)
#else
#define TGL_PACKAGE
#endif
#endif

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_tgl
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */
///Implements a handler for critical sections
class TGL_PACKAGE C_TGLCriticalSection
{
protected:
   CRITICAL_SECTION mt_CriticalSection; ///< synchronization object

private:
   //this class can not be copied:
   C_TGLCriticalSection(const C_TGLCriticalSection & orc_Source);
   C_TGLCriticalSection & operator = (const C_TGLCriticalSection & orc_Source);

public:
   C_TGLCriticalSection(void);
   virtual ~C_TGLCriticalSection(void);

   void Acquire(void);
   bool TryAcquire(void);
   void Release(void);
};

//----------------------------------------------------------------------------------------------------------------------

typedef void (* PR_TGLTimerEvent)(void * opv_Instance);

///Non-threaded timer. To be triggered by the OS's message queue handler
class TGL_PACKAGE C_TGLTimer
{
private:
   PR_TGLTimerEvent mpr_Callback;
   void * mpv_CallbackInstance;
   bool mq_Enabled;
   stw_types::uint32 u32_Interval;

   UINT_PTR mun_TimerHandle;

   void m_KillTimer(void);

   //callback lookup list:
   static stw_scl::SCLDynamicArray<UINT_PTR> mhc_Handles;
   static stw_scl::SCLDynamicArray<C_TGLTimer *> mhc_Instances;

   static void __stdcall mh_TimerCallback(HWND opv_Hwnd, const UINT oun_Msg, const UINT_PTR oun_TimerHandle,
                                          const DWORD ou32_Time);

   void m_UpdateTimer(void);

   //this class can not be copied:
   C_TGLTimer(const C_TGLTimer & orc_Source);
   C_TGLTimer & operator = (const C_TGLTimer & orc_Source);

public:
   C_TGLTimer(void);
   C_TGLTimer(const PR_TGLTimerEvent opr_Callback, void * const opv_Instance, const stw_types::uint32 ou32_Interval,
              const bool oq_Enabled);
   virtual ~C_TGLTimer(void);

   void SetEventCallback(const PR_TGLTimerEvent opr_Callback, void * const opv_Instance);
   void SetInterval(const stw_types::uint32 ou32_Interval);
   void SetEnabled(const bool oq_Enabled);

   bool GetEnabled(void) const;
   stw_types::uint32 GetInterval(void) const;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
}

#endif
