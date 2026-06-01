#include "RestButtons.h"
#include "ButtonFactory.h"

Button RestButtons::Rest()
{
    return ButtonFactory::Box(ButtonId::Rest, 22, 23, 21, "휴식", "green");
}

Button RestButtons::AbilityUpgrade()
{
    return ButtonFactory::Box(ButtonId::AbilityUpgrade, 78, 23, 21, "능력 강화", "cyan");
}
