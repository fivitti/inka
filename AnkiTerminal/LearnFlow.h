#ifndef LearnFlow_h
#define LearnFlow_h

#include <SdFat.h>
#include "Drawer.h"
#include "AcceptDrawFrame.h"
#include "Session.h"
#include "Summary.h"
#include "Config.h"

#define OK_ACTION 0
#define NEEDED_DRAW DELINE_ACTION
#define NOT_NEEDED_DRAW ACCEPT_ACTION

namespace LearnFlow
{ 
  byte isNeededDrawCards(SdFat * sd)
  {
    if (FileTools::isExistApplicationFile(sd, SESSION_SET_FILENAME))
    {
      SimplyAcceptFrame restoreSession(F(" Kontynuacja?"));
      restoreSession.show();
      
      if (restoreSession.getSelectedAction() == DELINE_ACTION)
      {
        sd->remove(SESSION_SET_FILENAME);
      }

      return restoreSession.getSelectedAction();
    }

    return NEEDED_DRAW;
  }
  
  byte drawCards(SdFat * sd)
  {
    Drawer drawer(sd);
    AcceptDrawFrame adf;
    byte drawAction = 0;
    do
    {
      LcdTools::writeFullscreenMessage(F("  Losuję..."));
      if (!drawer.readConfiguration())
        return SHUTDOWN_ACTION;
      adf.setToAcceptNumber(drawer.draw());
      adf.show();
      drawAction = adf.getSelectedAction();

      RETURN_WHEN_SHUTDOWN(drawAction);
    }
    while (drawAction != ACCEPT_ACTION);

    return drawAction;
  }

  byte ankiMain(SdFat * sd)
  { 
    byte isNeededDraw = LearnFlow::isNeededDrawCards(sd); 
    RETURN_WHEN_SHUTDOWN(isNeededDraw);
  
    if (isNeededDraw == NEEDED_DRAW)
      RETURN_WHEN_SHUTDOWN(LearnFlow::drawCards(sd)); 
  
    Session session(sd);
    session.configure();
    RETURN_WHEN_SHUTDOWN(session.executeSession());
  
    Summary summary(sd);
    summary.execute();
  
	return OK_ACTION;
  }
}

#undef OK_ACTION
#undef NEEDED_DRAW
#undef NOT_NEEDED_DRAW

#endif //LearnFlow_h
