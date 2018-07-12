#include "animatorOperations.h"


#include "animation/animator.h"
#include "drawables/drawableCage.h"
#include "skinning/cageSkinning.h"
#include "GUI/glCanvas.h"
#include "GUI/mainWindow.h"
#include "controller.h"
#include <QInputDialog>


void initializeAnimator()
{
   Controller * c = Controller::get();

   if(c->isCageLoaded)
   {

      Animator * animator = new Animator(c->cage);
      c->animator = animator;

      //animator->populateKeyframesDebug((c->cage->getNumVertices())*3);
      c->isAnimatorInitialized = true;

   }
}

void setNextKeyframe()
{
   Controller * c = Controller::get();

   if(c->isAnimatorInitialized)
   {
      c->animator->setNextPose();

      if(c->isCageSkinningInitialized)
      {
         c->cageSkinning->deform();
         c->cage->updateNormals();
      }

      c->glCanvas->refreshScene();
   }

}

void addKeyframe()
{
   Controller * c = Controller::get();
   if(c->isCageSkinningInitialized)
   {

      bool ok;
      double t = QInputDialog::getDouble(c->mainWindow, "Keyframe time (in seconds)",
                                        "Second: ", 0, 0, 2147483647, 1, &ok);
      if (ok && t>0)
      {

         const std::vector<double> & restCage = c->cage->getRestPoseVerticesVector();
         const std::vector<double> & actualCage = c->cage->getVerticesVector();
         std::vector<double> offsets(restCage.size());
         for(int i=0; i<restCage.size(); ++i)
         {
            offsets[i] = actualCage[i] - restCage[i];
         }

         c->animator->addKeyframe(t, offsets);
      }
   }
}