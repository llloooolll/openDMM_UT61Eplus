#include "blinky.h"
#include "bsp.h"
#include "hc32l13x.h"
#include "io_config.h"
#include "qpn.h"
#include "sysctrl.h"

/* Local-scope objects -----------------------------------------------------*/
static QEvt l_blinkyQSto[10]; /* Event queue storage for Blinky */

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM QF_active[] = {
    {(QActive *)0, (QEvt *)0, 0U},
    {(QActive *)&AO_Blinky, l_blinkyQSto, Q_DIM(l_blinkyQSto)}};

/*..........................................................................*/
int main(void) {
    Blinky_ctor(); /* instantiate all Blinky AO */

    QF_init(Q_DIM(QF_active)); /* initialize the QF-nano framework */
    BSP_init();

    return QF_run(); /* transfer control to QF-nano */
}
