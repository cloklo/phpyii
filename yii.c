/*
  +----------------------------------------------------------------------+
  | Yii Framework as PHP extension                                       |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Baoqiang Su  <zmrnet@qq.com>                                 |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "Zend/zend_alloc.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "php_yii.h"
#include "yiibase.h"

#define MICRO_IN_SEC 1000000.00

zend_class_entry *yii_ce;

ZEND_DECLARE_MODULE_GLOBALS(yii);

/* {{{ yii_functions[]
*/
zend_function_entry yii_functions[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION(yii)
{
	yii_globals->debug			= 0;
	yii_globals->trace_level		= 0;
	yii_globals->enable_exception_handler	= 1;
	yii_globals->enable_error_handler	= 1;
}
/* }}} */

/** {{{ yii_methods
*/
zend_function_entry yii_methods[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yii)
{
	REGISTER_STRINGL_CONSTANT("YII_VERSION", YII_VERSION, 	sizeof(YII_VERSION) - 1, 	CONST_PERSISTENT | CONST_CS);

	/* startup components */
	PHP_MINIT(yiibase)(INIT_FUNC_ARGS_PASSTHRU);

	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Yii", yii_methods);
	yii_ce = zend_register_internal_class_ex(&ce, yiibase_ce, NULL TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(yii)
{
	if (YII_G(path)) {
		zend_hash_destroy(YII_G(path));
		pefree(YII_G(path), 1);
	}

	if (YII_G(zii_path)) {
		zend_hash_destroy(YII_G(zii_path));
		pefree(YII_G(zii_path), 1);
	}

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(yii)
{
	YII_G(path)			= NULL;
	YII_G(zii_path)			= NULL;
#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4))
	YII_G(buffer)			= NULL;
	YII_G(owrite_handler)		= NULL;
	YII_G(buf_nesting)		= 0;
#endif

	struct timeval tp = {0};
	if (gettimeofday(&tp, NULL)) {
		YII_G(begin_time)	= 0;	
	} else {
		YII_G(begin_time)	= (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC);
	}

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(yii)
{
	if (YII_G(path)) {
		efree(YII_G(path));
	}

	if (YII_G(zii_path)) {
		efree(YII_G(zii_path));
	}

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(yii)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yii support", "enabled");
	php_info_print_table_row(2, "Version", YII_VERSION);
	//php_info_print_table_row(2, "Supports", YII_SUPPORT_URL);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/** {{{ DL support
 */
#ifdef COMPILE_DL_YII
ZEND_GET_MODULE(yii)
#endif
/* }}} */

/** {{{ module depends
 */
#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep yii_deps[] = {
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("pcre")
	ZEND_MOD_OPTIONAL("session")
	{NULL, NULL, NULL}
};
#endif
/* }}} */

/** {{{ yii_module_entry
*/
zend_module_entry yii_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
	STANDARD_MODULE_HEADER_EX, NULL,
	yii_deps,
#else
	STANDARD_MODULE_HEADER,
#endif
	"yii",
	yii_functions,
	PHP_MINIT(yii),
	PHP_MSHUTDOWN(yii),
	PHP_RINIT(yii),
	PHP_RSHUTDOWN(yii),
	PHP_MINFO(yii),
	YII_VERSION,
	PHP_MODULE_GLOBALS(yii),
	PHP_GINIT(yii),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
