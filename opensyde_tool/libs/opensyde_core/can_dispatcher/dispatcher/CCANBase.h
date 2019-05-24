//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Header for C_CAN base class.

   Header to an abstract base class for interfacing with different CAN communication driver architectures.

   \copyright   Copyright 2010 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef CCANBASEH
#define CCANBASEH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stw_can.h"

#include "SCLDynamicArray.h"

namespace stw_can
{

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifndef STWCAN_PACKAGE
#ifdef __BORLANDC__
//maybe we want this module to be put into a VCL package ...
#define STWCAN_PACKAGE __declspec(package)
#else
#define STWCAN_PACKAGE
#endif
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */
///Abstract base class defining fundamental interface for all CAN drivers
//lint -sem(stw_can::C_CAN_Base::m_InitClassBase,initializer)
class STWCAN_PACKAGE C_CAN_Base
{
private:
   void m_InitClassBase(void);

protected:
   stw_types::uint8 mu8_CANOpened;
   stw_types::uint8 mu8_CommChannel;
   bool mq_XTDAvailable;

public:
   C_CAN_Base(void);
   C_CAN_Base(const stw_types::uint8 ou8_CommChannel);
   virtual ~C_CAN_Base(void);

   //functions that need to be implemented by target driver:
   //-----------------------------------------------------------------------------
   /*!
      \brief   Init communication with specified bitrate

      Functions shall init the CAN bus bus with the specified bitrate

      \return
      C_NO_ERR   bus initialized
      else       error at initialization
   */
   //-----------------------------------------------------------------------------
   virtual stw_types::sint32 CAN_Init(void) = 0;

   //-----------------------------------------------------------------------------
   /*!
      \brief   Init communication

      Functions shall init the CAN bus with a default bitrate.
      How and where the bitrate is specified is up to the implementation.

      \param[in]  os32_BitrateKBitS  bitrate to initialize bus with

      \return
      C_NO_ERR   bus initialized
      else       error at initialization
   */
   //-----------------------------------------------------------------------------
   virtual stw_types::sint32 CAN_Init(const stw_types::sint32 os32_BitrateKBitS) = 0;

   //-----------------------------------------------------------------------------
   /*!
      \brief   Close communication

      Functions shall detach from the CAN.
      How and where the bitrate is specified is up to the implementation.

      \return
      C_NO_ERR   bus closed
      else       error
   */
   //-----------------------------------------------------------------------------
   virtual stw_types::sint32 CAN_Exit(void) = 0;

   //-----------------------------------------------------------------------------
   /*!
      \brief   Close and reinit communication

      Functions shall reinit bus communication.
      As a result all TX and RX buffers shall be cleared.

      \return
      C_NO_ERR   bus reset
      else       error
   */
   //-----------------------------------------------------------------------------
   virtual stw_types::sint32 CAN_Reset(void) = 0;

   //-----------------------------------------------------------------------------
   /*!
      \brief   Read one incoming CAN message

      Functions shall read and return one incoming CAN message.

      \param[out]  orc_Message  read message

      \return
      C_NO_ERR   message read
      else       no message read or error
   */
   //-----------------------------------------------------------------------------
   virtual stw_types::sint32 CAN_Read_Msg(T_STWCAN_Msg_RX & orc_Message) = 0;

   //-----------------------------------------------------------------------------
   /*!
      \brief   Send one CAN message

      Functions shall send one CAN message.

      \param[in]  orc_Message  message to send

      \return
      C_NO_ERR   message sent
      else       error trying to send message
   */
   //-----------------------------------------------------------------------------
   virtual stw_types::sint32 CAN_Send_Msg(const T_STWCAN_Msg_TX & orc_Message) = 0;

   //-----------------------------------------------------------------------------
   /*!
      \brief   Check timestamp basis of driver

      Return system time in micro seconds as the driver sees it.
      Can be used by an application to have a time basis for putting the timestamps of received messages into a
       timeframe.

      \param[out]  oru64_SystemTimeUs  system time in micro seconds

      \return
      C_NO_ERR   information read
      else       error trying to get information
   */
   //-----------------------------------------------------------------------------
   virtual stw_types::sint32 CAN_Get_System_Time(stw_types::uint64 & oru64_SystemTimeUs) const = 0;

   void SetCommChannel(const stw_types::uint8 ou8_CommChannel);
   stw_types::uint8 GetCommChannel(void) const;

   bool GetXTDAvailable(void) const;
};

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

}

#endif
