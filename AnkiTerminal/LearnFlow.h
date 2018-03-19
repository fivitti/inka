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

extern SdFat g_sd;

/*
 * These methods encapsulate main stages programs.
 * The learn flow in @performSessionLearn method:
 * 1. User select dictionary
 * 2. If progress file not exist or user want reset
 *    progress then generate new progress file
 * 3. If not exist session file or user want start
 *    new session then algorithm draw cards (generate
 *    session file) until user accept number of drawing
 *    cards.
 * 4. Start session. At end save progress.
 *
 * Note: At any time user can press MENU button. After
 *       press program should be stopped and retun to menu.
 */
namespace LearnFlow
{   
  // New progress file should be created when:
  // - progress file not exist
  // - user want reset progress
  // Return true if progress file should be create or false if not.
  bool shouldCreateNewProgressFile(const char * fileName)
  {
    if (!FileTools::isExistApplicationFile(fileName))
    {
      return true;
    }

    YesNoAcceptFrame deleteProgress(F(LANG_STR_CONTINUE_PROGRESS_POPUP));
    deleteProgress.show();
    if (deleteProgress.getSelectedAction() == DELINE_ACTION)
    {
      g_sd.remove(fileName);
      return true;
    }
    else
    {
      return false;
    }

  }

  // Cards should be drawn when:
  // - Session file not exist
  // - User want discard session
  byte isNeededDrawCards()
  {
    if (!FileTools::isExistApplicationFile(SESSION_SET_FILENAME))
    {
      return NEEDED_DRAW;
    }

    YesNoAcceptFrame restoreSession(F(LANG_STR_CONTINUE_SESSION_POPUP));
    restoreSession.show();
      
    if (restoreSession.getSelectedAction() == DELINE_ACTION)
    {
      g_sd.remove(SESSION_SET_FILENAME);
    }

    return restoreSession.getSelectedAction();
  }
  
  // Algorithm should draw so much cards as @CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION
  // value in configuration.
  // If draw number is less then maximum then should be draw until user don't accept
  // number of drawed cards.
  byte drawCards()
  {
    Drawer drawer;
    AcceptDrawFrame adf;
    byte drawAction = 0;

    // Return false when user don't select dictionary.
    if (!drawer.readConfiguration())
      return SHUTDOWN_ACTION;

    do
    {
      LcdTools::writeFullscreenMessage(F(LANG_STR_DRAW_IN_PROGRESS_MESSAGE));

      byte numberOfDrawnCards = drawer.draw();

      if (numberOfDrawnCards == drawer.getMaximumToDraw())
        return ACCEPT_ACTION;

      //Draw number cards less then maximum
      adf.setToAcceptNumber(numberOfDrawnCards);
      adf.show();
      drawAction = adf.getSelectedAction();

      RETURN_WHEN_SHUTDOWN(drawAction);
    }
    while (drawAction != ACCEPT_ACTION);

    return drawAction;
  }

  // Algorithm:
  // 1. Draw cards
  // 2. Learn cards
  // 3. Save progres
  byte performSessionLearn()
  { 
    byte isNeededDraw = LearnFlow::isNeededDrawCards(); 
    RETURN_WHEN_SHUTDOWN(isNeededDraw);
  
    if (isNeededDraw == NEEDED_DRAW)
      RETURN_WHEN_SHUTDOWN(LearnFlow::drawCards()); 
  
    Session session;
    session.configure();
    RETURN_WHEN_SHUTDOWN(session.executeSession());
  
    Summary summary;
    summary.execute();
  
	return OK_ACTION;
  }
}

#undef OK_ACTION
#undef NEEDED_DRAW
#undef NOT_NEEDED_DRAW

#endif //LearnFlow_h
