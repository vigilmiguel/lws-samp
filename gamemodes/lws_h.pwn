/******************************************************************
    LW's Header File 
******************************************************************/


/******************************************************************
    Includes: General/Headers
******************************************************************/
#include <a_samp>
#include <a_mysql>
#include <YSI\y_colours>
#include <YSI\y_dialog>
#include <YSI\y_inline>
#include <a_mysql_yinline>
#include <zcmd>


/******************************************************************
    Constants
******************************************************************/
#define VERSION_MAJOR			        0
#define VERSION_MINOR                   0
#define VERSION_BUILD                   1

#define FREEROAM_WORLD                  0
#define CNR_WORLD                       1
#define BOT_WORLD                       2

#define MYSQL_SUCCESS                   0

#define OLD_HASHPASS_SIZE               16

#define SQLITE_DB                       "lw_lvdm_gm.db"

#define NEGATIVE                        0

#define G_S_TEMPNAME                    "LW_TempName_"
#define G_I_PASSWORD_ATTEMPTS           3
#define G_I_MIN_PASS_LENGTH             6


/******************************************************************
    Global Variables
******************************************************************/
// MySQL Handle.
new MySQL:g_mysqlHandle = MySQL:MYSQL_INVALID_HANDLE;

enum G_E_PLAYER_DATA
{
    e_s_name[MAX_PLAYER_NAME],
    bool:e_b_tempName,
    Cache:e_cache,
    e_s_oldHashPass[OLD_HASHPASS_SIZE + 1],
    e_i_invalidPassRemaining

};
new player[MAX_PLAYERS][G_E_PLAYER_DATA];




/******************************************************************
    Includes: Modules
******************************************************************/

#include "modules\global\player\account\account.pwn"
