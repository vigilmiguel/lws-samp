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
#include <YSI\y_timers>
#include <a_mysql_yinline>
#include <zcmd>


/******************************************************************
    Constants
******************************************************************/
#define VERSION_MAJOR                   0
#define VERSION_MINOR                   0
#define VERSION_BUILD                   4

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
#define G_I_MAX_PASS_LENGTH             30
#define G_I_SALT_SIZE                   21
#define G_I_HASH_SIZE                   65


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
    e_s_hash[G_I_HASH_SIZE + 1],
    e_s_salt[G_I_SALT_SIZE + 1],
    e_i_invalidPassRemaining,
    e_i_kills,
    e_i_deaths,
    e_i_bank,
    e_i_wallet

};
new player[MAX_PLAYERS][G_E_PLAYER_DATA];




/******************************************************************
    Includes: Modules
******************************************************************/

#include "modules\global\public.inc"
#include "modules\global\player\account\account.pwn"
