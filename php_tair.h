/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifndef PHP_TAIR_H
#define PHP_TAIR_H

extern zend_module_entry tair_module_entry;
#define phpext_tair_ptr &tair_module_entry

#ifdef PHP_WIN32
#	define PHP_TAIR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TAIR_API __attribute__ ((visibility("default")))
#else
#	define PHP_TAIR_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "tair_client_capi.hpp"

PHP_MINIT_FUNCTION(tair);
PHP_MSHUTDOWN_FUNCTION(tair);
//PHP_RINIT_FUNCTION(tair);
//PHP_RSHUTDOWN_FUNCTION(tair);
PHP_MINFO_FUNCTION(tair);

PHP_FUNCTION(tair_init);
PHP_FUNCTION(tair_get);
PHP_FUNCTION(tair_put);
PHP_FUNCTION(tair_remove);
PHP_FUNCTION(tair_incr);
PHP_FUNCTION(tair_decr);
PHP_FUNCTION(tair_last_msg);

ZEND_BEGIN_MODULE_GLOBALS(tair)
	char *log_level;
ZEND_END_MODULE_GLOBALS(tair)


#define PHP_TAIR_AREA 0

/* In every utility function you add that needs to use variables 
   in php_tair_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as TAIR_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define TAIR_G(v) TSRMG(tair_globals_id, zend_tair_globals *, v)
#else
#define TAIR_G(v) (tair_globals.v)
#endif

#endif	/* PHP_TAIR_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
