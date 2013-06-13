/*
  +----------------------------------------------------------------------+
  | Yii Framework                                                        |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "Zend/zend_alloc.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include "php_yii.h"

ZEND_DECLARE_MODULE_GLOBALS(yii);

/* {{{ yii_functions[]
*/
zend_function_entry yii_functions[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_INI_MH(OnUpdateSeparator)
 */
PHP_INI_MH(OnUpdateSeparator) {
	YII_G(name_separator) = new_value; 
	YII_G(name_separator_len) = new_value_length;
	return SUCCESS;
}
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION(yii)
{
	yii_globals->autoload_started   = 0;
}
/* }}} */

/** {{{ PHP_INI
 */
PHP_INI_BEGIN()

PHP_INI_END();
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yii)
{
	REGISTER_INI_ENTRIES();

		REGISTER_STRINGL_CONSTANT("YII_VERSION", YII_VERSION, 	sizeof(YII_VERSION) - 1, 	CONST_PERSISTENT | CONST_CS);

	/* startup components */
	/* YII_STARTUP(application); */

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(yii)
{
	UNREGISTER_INI_ENTRIES();

	if (YII_G(configs)) {
		zend_hash_destroy(YII_G(configs));
		pefree(YII_G(configs), 1);
	}

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(yii)
{
	YII_G(running)			= 0;
#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4))
	YII_G(buffer)			= NULL;
	YII_G(owrite_handler)		= NULL;
	YII_G(buf_nesting)		= 0;
#endif

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(yii)
{
	if (YII_G(directory)) {
		efree(YII_G(directory));
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
	php_info_print_table_row(2, "Supports", YII_SUPPORT_URL);
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