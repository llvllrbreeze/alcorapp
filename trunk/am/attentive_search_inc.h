#ifndef attention_machine_inc_H_INCLUDED
#define attention_machine_inc_H_INCLUDED
//###########################################################################
namespace all { namespace am { 
//###########################################################################
  namespace events{
    typedef enum tag_{
      STOP,
      START,
      TRACKSETUP,
      TRACK,
      IDLE,
      DEBUG,
      RESET,
      RESETPANTILT
    }tag;
  }
//---------------------------------------------------------------------------
  namespace states{
    typedef enum tag_{
      IDLE,
      RUNNING,
      PROCESSING
    }tag;
  }
//###########################################################################
}}
//###########################################################################

#endif //attention_machine_inc_H_INCLUDED


