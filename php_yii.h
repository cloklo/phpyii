/*
  +----------------------------------------------------------------------+
  | Yii Framework as PHP extension										 |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,		 |
  | that is bundled with this package in the file LICENSE, and is		 |
  | available through the world-wide-web at the following url:			 |
  | http://www.php.net/license/3_01.txt									 |
  | If you did not receive a copy of the PHP license and are unable to	 |
  | obtain it through the world-wide-web, please send a note to			 |
  | license@php.net so we can mail you a copy immediately.				 |
  +----------------------------------------------------------------------+
  | Author: Baoqiang Su  <zmrnet@qq.com>								 |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_YII_H
#define PHP_YII_H

extern zend_module_entry yii_module_entry;
#define phpext_yii_ptr &yii_module_entry

#ifdef PHP_WIN32
#define PHP_YII_API __declspec(dllexport)
#ifndef _MSC_VER
#define _MSC_VER 1600
#endif
#else
#define PHP_YII_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define YII_G(v) TSRMG(yii_globals_id, zend_yii_globals *, v)
#else
#define YII_G(v) (yii_globals.v)
#endif

#define YII_VERSION						"1.0.0"

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION == 2)) || (PHP_MAJOR_VERSION > 5)
#define Z_SET_REFCOUNT_P(pz, rc)	  (pz)->refcount = rc
#define Z_SET_REFCOUNT_PP(ppz, rc)	  Z_SET_REFCOUNT_P(*(ppz), rc)
#define Z_ADDREF_P	 ZVAL_ADDREF
#define Z_REFCOUNT_P ZVAL_REFCOUNT
#define Z_DELREF_P	 ZVAL_DELREF
#endif

#define yii_capplication_t	zval

#define YII_ME(c, m, a, f) {m, PHP_MN(c), a, (zend_uint) (sizeof(a)/sizeof(struct _zend_arg_info)-1), f},

extern PHPAPI void php_var_dump(zval **struc, int level TSRMLS_DC);
extern PHPAPI void php_debug_zval_dump(zval **struc, int level TSRMLS_DC);

ZEND_BEGIN_MODULE_GLOBALS(yii)
	double		begin_time; 
	zend_bool	debug;
	uint		trace_level;
	zend_bool	enable_exception_handler;
	zend_bool	enable_error_handler;
	char		*path;
	char		*yii_path;
	char		*zii_path;
#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4))
	uint		buf_nesting;
	void		*buffer;
	void		*owrite_handler;
#endif
ZEND_END_MODULE_GLOBALS(yii)

PHP_MINIT_FUNCTION(yii);
PHP_MSHUTDOWN_FUNCTION(yii);
PHP_RINIT_FUNCTION(yii);
PHP_RSHUTDOWN_FUNCTION(yii);
PHP_MINFO_FUNCTION(yii);

extern ZEND_DECLARE_MODULE_GLOBALS(yii);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
