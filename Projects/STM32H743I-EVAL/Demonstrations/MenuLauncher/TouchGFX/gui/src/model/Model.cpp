#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#if !defined(SIMULATOR)
extern "C"
{
  extern void GoTo_EmbeddedWizard(void);
  extern void GoTo_STemWin(void);
  extern void GoTo_TouchGFX(void);
}
#endif /* !SIMULATOR */

Model::Model() : modelListener(0)
{
}

void Model::tick()
{
}

void Model::moduleSelected(MODULES value)
{
  switch (value)
  {
    case TOUCHGFX:
#if !defined(SIMULATOR)
      GoTo_TouchGFX();
#endif /* !SIMULATOR */
      break;
    case STEMWIN:
#if !defined(SIMULATOR)
      GoTo_STemWin();
#endif /* !SIMULATOR */
      break;
    case EMBEDDED_WIZARD:
#if !defined(SIMULATOR)
      GoTo_EmbeddedWizard();
#endif /* !SIMULATOR */
      break;
    default:
      break;
  }
}
