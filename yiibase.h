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

#ifndef PHP_YIIBASE_H
#define PHP_YIIBASE_H

#define yii_import(alias, alias_len, force, result) \
	yiibase_import((alias), (alias_len), (force), (result) ZEND_FILE_LINE_CC TSRMLS_CC)

#define yii_execute_scripts(path, type) \
	yiibase_execute_scripts((path), (type) ZEND_FILE_LINE_CC TSRMLS_CC)

#define yii_get_framework_path() \
	yiibase_get_framework_path(ZEND_FILE_LINE_CC TSRMLS_CC)

#define yii_autoload(cname, cname_len) \
	yiibase_autoload((cname), (cname_len) ZEND_FILE_LINE_CC TSRMLS_CC)

#define yii_set_path_of_alias(alias, alias_len, path, path_len) \
	yiibase_set_path_of_alias((alias), (alias_len), (path), (path_len) ZEND_FILE_LINE_CC TSRMLS_CC)

#define yii_get_path_of_alias(alias, alias_len, path, path_len) \
	yiibase_get_path_of_alias((alias), (alias_len), (path), (path_len) ZEND_FILE_LINE_CC TSRMLS_CC)

#define yii_create_application(result, cname, cname_len, config) \
	yiibase_create_application((result), (cname), (cname_len), (config) ZEND_FILE_LINE_CC TSRMLS_CC)

#define yii_create_component(result, config, argc, argv) \
	yiibase_create_component((result), (config), (argc), (argv) ZEND_FILE_LINE_CC TSRMLS_CC)

extern zend_class_entry *yiibase_ce;

PHP_MINIT_FUNCTION(yiibase);
#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
