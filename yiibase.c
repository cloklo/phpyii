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

#include "php_yii.h"
#include "yiibase.h"

zend_class_entry *yiibase_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yiibase_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaf_loader_getinstance_arginfo, 0, 0, 0)
    ZEND_ARG_INFO(0, local_library_path)
    ZEND_ARG_INFO(0, global_library_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaf_loader_autoloader_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, class_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaf_loader_regnamespace_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name_prefix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaf_loader_islocalname_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, class_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaf_loader_import_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, file)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaf_loader_setlib_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, library_path)
    ZEND_ARG_INFO(0, is_global)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaf_loader_getlib_arginfo, 0, 0, 0)
    ZEND_ARG_INFO(0, is_global)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ proto public static YiiBase::createConsoleApplication($config=null)
*/
PHP_METHOD(yiibase, getVersion) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::createConsoleApplication($config=null)
*/
PHP_METHOD(yiibase, createConsoleApplication) {
	char *path = "yiix.php";

	zend_file_handle file_handle;
	zend_op_array 	*op_array;
	char realpath[MAXPATHLEN];

	if (!VCWD_REALPATH(path, realpath)) {
	//	return 0;
	}

	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.filename = path;
	file_handle.opened_path = NULL;
	file_handle.free_filename = 0;

	zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC, NULL, 1, &file_handle);

}
/* }}} */

/** {{{ yiibase_methods
*/
zend_function_entry yiibase_methods[] = {
	PHP_ME(yiibase, getVersion, yiibase_void_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, createConsoleApplication, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yiibase) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "YiiBase", yiibase_methods);
	yiibase_ce = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
