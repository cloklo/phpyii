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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "Zend/zend_alloc.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/php_string.h"

#include "php_yii.h"
#include "yiibase.h"

zend_class_entry *yiibase_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yiibase_getversion_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_createwebapp_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_createconsoleapp_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_createapplication_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, class)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_app_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_setapplication_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, app)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_getframeworkpath_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_createcomponent_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_import_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
	ZEND_ARG_INFO(0, forceInclude)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_getpathofalias_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_setpathofalias_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, alias)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_autoload_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_trace_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, msg)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_log_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, msg)
	ZEND_ARG_INFO(0, level)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_beginprofile_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, token)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_endprofile_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, token)
	ZEND_ARG_INFO(0, category)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_getlogger_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_setlogger_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, logger)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_powered_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_t_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, category)
	ZEND_ARG_INFO(0, message)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_INFO(0, language)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yiibase_registerautoloader_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_INFO(0, append)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ int yiibase_execute_scripts(char *path, int type ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yiibase_execute_scripts(char *path, int type ZEND_FILE_LINE_DC TSRMLS_DC) {
	zend_file_handle file_handle;

	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.opened_path = NULL;
	file_handle.free_filename = 0;
	file_handle.filename = path;

	return zend_execute_scripts(type TSRMLS_CC, NULL, 1, &file_handle);
}
/* }}} */

/** {{{ int yiibase_object_init_ex(zval *obj, zend_class_entry *ce, int argc, zval **argv ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yiibase_object_init_ex(zval *obj, zend_class_entry *ce, int argc, zval **argv ZEND_FILE_LINE_DC TSRMLS_DC) {

	object_init_ex(obj, ce);
	
	if (zend_hash_exists(&Z_OBJCE_P(obj)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
		zval retval, constructor;

		INIT_ZVAL(retval);
		INIT_ZVAL(constructor);

		switch (argc) {
		case 0:
			zend_call_method_with_0_params(&obj, ce, &ce->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL);
			break;
		case 1:
			zend_call_method_with_1_params(&obj, ce, &ce->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, argv[0]);
			break;
		case 2:
			zend_call_method_with_2_params(&obj, ce, &ce->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, argv[0], argv[1]);
			break;
		default:
			ZVAL_STRING(&constructor, ZEND_CONSTRUCTOR_FUNC_NAME, 1);
			if (call_user_function(NULL, &obj, &constructor, &retval, argc, argv TSRMLS_CC) == FAILURE) {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling constructor");
			}
		}

		zval_dtor(&constructor);
		zval_dtor(&retval);
	}

	return SUCCESS;
}
/* }}} */

/** {{{ char *yiibase_t(const char *category, char *message, int message_len, zval *params, char *source, uint source_len, char *language, uint language_len TSRMLS_DC)
 * */
char *yiibase_t(const char *category, char *message, int message_len, zval *params, char *source, uint source_len, char *language, uint language_len TSRMLS_DC) {
	zval *app;

	app = zend_read_static_property(yiibase_ce, ZEND_STRL("_app"), 0 TSRMLS_CC);
	
	if (Z_TYPE_P(app) != IS_NULL) {
		if (source_len == 0) {
			if (strlen(category) == 3 && (strncmp(category, "yii", 3) == 0 || strncmp(category, "zii", 3) == 0)) {
				source_len = spprintf(&source, 0, "coreMessages");
			} else {
				source_len = spprintf(&source, 0, "messages");
			}
		} else {
			
		}
	}
}
/* }}} */

/** {{{ int yiibase_set_path_of_alias(char *alias, uint alias_len, char *path, uint path_len ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yiibase_set_path_of_alias(char *alias, uint alias_len, char *path, uint path_len ZEND_FILE_LINE_DC TSRMLS_DC) {
	zval *aliases;

	aliases = zend_read_static_property(yiibase_ce, ZEND_STRL("_aliases"), 0 TSRMLS_CC);

	if (Z_TYPE_P(aliases) == IS_ARRAY) {
		if (path_len == 0) {
			zend_hash_del(Z_ARRVAL_P(aliases), alias, alias_len);
		} else {
			add_assoc_stringl_ex(aliases, alias, alias_len, path, path_len, 1);
		}
	
		return SUCCESS;
	}

	return FAILURE;
}
/* }}} */

/** {{{ int yiibase_get_path_of_alias(char *alias, int alias_len, char **path, int *path_len ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yiibase_get_path_of_alias(char *alias, int alias_len, char **path, int *path_len ZEND_FILE_LINE_DC TSRMLS_DC) {
	zval *aliases;

	aliases = zend_read_static_property(yiibase_ce, ZEND_STRL("_aliases"), 0 TSRMLS_CC);

	if (Z_TYPE_P(aliases) == IS_ARRAY) {
		zval **ppzval;

		if (zend_hash_find(Z_ARRVAL_P(aliases), alias, alias_len, (void**)&ppzval) == SUCCESS) {
			convert_to_string_ex(ppzval);
			*path_len = Z_STRLEN_PP(ppzval);
			*path = Z_STRVAL_PP(ppzval);

			return SUCCESS;
		}
	}

	return FAILURE;
}
/* }}} */

/** {{{ char *yiibase_get_framework_path(ZEND_FILE_LINE_DC TSRMLS_DC)
*/
char *yiibase_get_framework_path(ZEND_FILE_LINE_DC TSRMLS_DC) {
	zval *path;

	if (YII_G(yii_path)) {
		return YII_G(yii_path); 
	}
	
	if (zend_hash_find(EG(zend_constants), ZEND_STRS("YII_PATH"), (void **) &path) == SUCCESS) {
		convert_to_string(path);
		YII_G(yii_path) = estrndup(Z_STRVAL_P(path), Z_STRLEN_P(path));
		return YII_G(yii_path);
	}

	return YII_G(path);
}
/* }}} */

/** {{{ int yiibase_autoload(char *cname, uint cname_len ZEND_FILE_LINE_DC TSRMLS_DC)
*/
int yiibase_autoload(char *cname, uint cname_len ZEND_FILE_LINE_DC TSRMLS_DC) {
	char *filepath;
	zval *classmap;
	zval *coreclasses;

	classmap = zend_read_static_property(yiibase_ce, ZEND_STRL("classMap"), 0 TSRMLS_CC);

	if (Z_TYPE_P(classmap) == IS_ARRAY) {
		zval **ppzval;

		if (zend_hash_find(Z_ARRVAL_P(classmap), cname, cname_len+1, (void**)&ppzval) == SUCCESS) {
			convert_to_string_ex(ppzval);

			yii_execute_scripts(Z_STRVAL_PP(ppzval), ZEND_INCLUDE);

			return SUCCESS;
		}
	}

	coreclasses = zend_read_static_property(yiibase_ce, ZEND_STRL("_coreClasses"), 0 TSRMLS_CC);

	if (Z_TYPE_P(coreclasses) == IS_ARRAY) {
		zval **ppzval;

		if (zend_hash_find(Z_ARRVAL_P(coreclasses), cname, cname_len+1, (void**)&ppzval) == SUCCESS) {
			convert_to_string_ex(ppzval);

			spprintf(&filepath, 0, "%s%s",  yii_get_framework_path(), Z_STRVAL_PP(ppzval));

			yii_execute_scripts(filepath, ZEND_INCLUDE);

			return SUCCESS;
		}
	}

	if (strstr(cname, "\\")) {
		char *alias;
		uint path_len, alias_len = 0;

		while (*cname == '\\') {
			cname++;
		}

		while (*cname != '\0') {
			if (*cname == '\\') {
				*alias = '.';
			} else {
				*alias =  *cname;
			}
			alias_len++;
			alias++;
			cname++;
		}

		*alias = '\0';
		alias = alias - alias_len;

		if (yii_get_path_of_alias(alias, alias_len, &filepath, &path_len) == SUCCESS) {

			yii_execute_scripts(filepath, ZEND_INCLUDE);

			return SUCCESS;
		}
	} else {
		zval *enableinc;

		enableinc = zend_read_static_property(yiibase_ce, ZEND_STRL("enableIncludePath"), 0 TSRMLS_CC);

		if (Z_TYPE_P(enableinc) == IS_BOOL && Z_BVAL_P(enableinc)) {
			spprintf(&filepath, 0, "%s.php", cname);

			yii_execute_scripts(filepath, ZEND_INCLUDE);

			return SUCCESS;
		} else {
			zval *incpaths;

			incpaths = zend_read_static_property(yiibase_ce, ZEND_STRL("_includePath"), 0 TSRMLS_CC);

			if (Z_TYPE_P(incpaths) == IS_ARRAY) {
				zval **ppzval;
				char *str_key;
				uint str_key_len;
				ulong num_key;
				HashPosition pos;

				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(incpaths), &pos);
				while (zend_hash_get_current_data_ex(Z_ARRVAL_P(incpaths), (void**)&ppzval, &pos) == SUCCESS) {
					spprintf(&filepath, 0, "%s/%s.php", Z_STRVAL_PP(ppzval), cname);

					if (VCWD_ACCESS(filepath, F_OK) == 0) {
						yii_execute_scripts(filepath, ZEND_INCLUDE);

						return SUCCESS;
					}
					zend_hash_move_forward_ex(Z_ARRVAL_P(incpaths), &pos);
				}
			}

		}

	}

	return FAILURE;

}
/* }}} */

/** {{{ char *yiibase_import(char *alias, int alias_len, char force, zval *result ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
char *yiibase_import(char *alias, int alias_len, char force, zval *result ZEND_FILE_LINE_DC TSRMLS_DC) {

}
/* }}} */

/** {{{ int yiibase_create_application(zval *result, char *cname, int cname_len, zval *config ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yiibase_create_application(zval *result, char *cname, int cname_len, zval *config ZEND_FILE_LINE_DC TSRMLS_DC) {
	zend_class_entry **pce = NULL;

	if (cname_len) {
		if (zend_lookup_class(cname, cname_len, &pce TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class %s does not exist", cname);
			return FAILURE;
		}

		object_init_ex(result, *pce);
		
		if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
			zend_call_method_with_1_params(&result, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, config);
		}
	}

	return SUCCESS;
}
/* }}} */

/** {{{ int yiibase_create_component(zval *result, zval *config, int argc, zval ***argv ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yiibase_create_component(zval *result, zval *config, int argc, zval ***argv ZEND_FILE_LINE_DC TSRMLS_DC) {
	zval **value, *ptype;

	if (Z_TYPE_P(config) == IS_STRING) {
		MAKE_STD_ZVAL(ptype);
		ZVAL_ZVAL(ptype, config, 1, 0);
		array_init(config);
	} else if (config && Z_TYPE_P(config) == IS_ARRAY
			&& zend_hash_find(Z_ARRVAL_P(config), "class", 6, (void **)&value) == SUCCESS ){
		MAKE_STD_ZVAL(ptype);
		ZVAL_ZVAL(ptype, *value, 1, 0);
		zend_hash_del(Z_ARRVAL_P(config), "class", 6);
	} else {
		zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Object configuration must be an array containing a \"class\" element.", E_ERROR TSRMLS_CC);
	}

	zend_class_entry **ce;
	uint ltype_len = Z_STRLEN_P(ptype);
	char *ltype = zend_str_tolower_dup(Z_STRVAL_P(ptype), ltype_len);

	if (zend_hash_find(EG(class_table), ltype, ltype_len+1, (void **) &ce) == FAILURE) {
		yii_import(Z_STRVAL_P(ptype), Z_STRLEN_P(ptype), 0, ptype);
	}
	efree(ltype);

	zend_class_entry **pce = NULL;

	if (argc < 5) {
		if (zend_lookup_class(Z_STRVAL_P(ptype), Z_STRLEN_P(ptype), &pce TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class %s does not exist", Z_STRVAL_P(ptype));
			return FAILURE;
		}
		object_init_ex(result, *pce);

		if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
			if (argc > 1) {
				zval *params[3]  = {0};

				if (argc == 2) {
					params[0] = *argv[1];
					zend_call_method_with_1_params(&result, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, params[0]);
				} else if (argc == 3) {
					params[0] = *argv[1];
					params[1] = *argv[2];
					zend_call_method_with_2_params(&result, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, params[0], params[1]);
				} else if (argc == 4) {
					zval retval, constructor;

					params[0] = *argv[1];
					params[1] = *argv[2];
					params[2] = *argv[3];

					INIT_ZVAL(retval);
					INIT_ZVAL(constructor);
					ZVAL_STRING(&constructor, ZEND_CONSTRUCTOR_FUNC_NAME, 1);

					if (call_user_function(NULL, &result, &constructor, &retval, 3, params TSRMLS_CC) == FAILURE) {
						php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling constructor");
					}

					zval_dtor(&constructor);
					zval_dtor(&retval);
				}
			} else {
				zend_call_method_with_0_params(&result, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL);
			}
		}
	} else {
		if (zend_lookup_class(ZEND_STRL("ReflectionClass"), &pce TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class ReflectionClass does not exist");
			return FAILURE;
		}

		zval *pzval, constructor;

		MAKE_STD_ZVAL(pzval);
		object_init_ex(pzval, *pce);

		zend_call_method_with_1_params(&pzval, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, ptype);

		INIT_ZVAL(constructor);
		ZVAL_STRING(&constructor, "newInstance", 1);
		if (call_user_function(NULL, &pzval, &constructor, result, argc, *argv TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling newInstance");
		}

		zval_dtor(&constructor);
		zval_dtor(pzval);
	}

	zval **ppzval;
	char *str_key;
	uint str_key_len;
	ulong num_key;
	HashPosition pos;

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(config), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(config), (void**)&ppzval, &pos) == SUCCESS) {
		if (zend_hash_get_current_key_ex(Z_ARRVAL_P(config), &str_key, &str_key_len, &num_key, 0, &pos) == HASH_KEY_IS_STRING) {
			zend_update_property(*ce, result, str_key, str_key_len, *ppzval TSRMLS_CC);
		}
		zend_hash_move_forward_ex(Z_ARRVAL_P(config), &pos);
	}
}
/* }}} */

/** {{{ proto public static YiiBase::getVersion(void)
*/
PHP_METHOD(yiibase, getVersion) {
	 RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::createWebApplication($config=null)
*/
PHP_METHOD(yiibase, createWebApplication) {
	zval *config;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	if (ZEND_NUM_ARGS() == 0) {
		ZVAL_NULL(config);
	}

	yii_create_application(return_value, "CWebApplication", 15, config);
}
/* }}} */

/** {{{ proto public static YiiBase::createConsoleApplication($config=null)
*/
PHP_METHOD(yiibase, createConsoleApplication) {
	zval *config;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	if (ZEND_NUM_ARGS() == 0) {
		ZVAL_NULL(config);
	}

	yii_create_application(return_value, "CConsoleApplication", 19, config);
}
/* }}} */


/** {{{ proto public static YiiBase::createApplication($class,$config=null)
*/
PHP_METHOD(yiibase, createApplication) {
	char *cname;
	int cname_len;
	zval *config;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &cname, &cname_len, &config) == FAILURE) {
		return;
	}

	yii_create_application(return_value, cname, cname_len, config);
}
/* }}} */

/** {{{ proto public static YiiBase::app(void)
*/
PHP_METHOD(yiibase, app) {
	zval *app;

	app = zend_read_static_property(yiibase_ce, ZEND_STRL("_app"), 0 TSRMLS_CC);

	RETURN_ZVAL(app, 1, 0);
}
/* }}} */

/** {{{ proto public static YiiBase::setApplication($app)
*/
PHP_METHOD(yiibase, setApplication) {
	zval *app;
	zval *zapp;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zapp) == FAILURE) {
		return;
	}

	app = zend_read_static_property(yiibase_ce, ZEND_STRL("_app"), 0 TSRMLS_CC);

	if (Z_TYPE_P(app) == IS_NULL || Z_TYPE_P(zapp) == IS_NULL) {
		zend_update_static_property(yiibase_ce, ZEND_STRL("_app"), zapp TSRMLS_CC);
	} else {
		zend_throw_exception(zend_exception_get_default(TSRMLS_C), "Yii application can only be created once.", E_ERROR TSRMLS_CC);
	}
}
/* }}} */

/** {{{ proto public static YiiBase::getFrameworkPath(void)
*/
PHP_METHOD(yiibase, getFrameworkPath) {
	if(!zend_get_constant(ZEND_STRL("YII_PATH"), return_value TSRMLS_CC)) {
		ZVAL_STRING(return_value, YII_G(path), 1);
	}
}
/* }}} */

/** {{{ proto public static YiiBase::createComponent($config)
*/
PHP_METHOD(yiibase, createComponent) {
	zval ***argv;
	int	argc = ZEND_NUM_ARGS();	

	if (argc < 1) {
		WRONG_PARAM_COUNT;
	}

	argv = (zval ***) safe_emalloc(argc, sizeof(zval **), 0);

	if (zend_get_parameters_array_ex(argc, argv) == FAILURE) {
		efree(argv);
		WRONG_PARAM_COUNT;
	}

	yii_create_component(return_value, *argv[0], argc, argv);
}
/* }}} */

/** {{{ proto public static YiiBase::import($alias,$forceInclude=false)
*/
PHP_METHOD(yiibase, import) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::getPathOfAlias($alias)
*/
PHP_METHOD(yiibase, getPathOfAlias) {
	char *alias, *path;
	uint alias_len, path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &alias, &alias_len) == FAILURE) {
		return;
	}

	if (yii_get_path_of_alias(alias, alias_len, &path, &path_len) == FAILURE) {
		RETURN_EMPTY_STRING();
		return;
	}

	RETURN_STRINGL(path, path_len, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::setPathOfAlias($alias,$path)
*/
PHP_METHOD(yiibase, setPathOfAlias) {
	char *alias, *path;
	uint alias_len, path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &alias, &alias_len, &path, &path_len) == FAILURE) {
		return;
	}

	yii_set_path_of_alias(alias, alias_len, path, path_len);
}
/* }}} */

/** {{{ proto public static YiiBase::autoload($className)
*/
PHP_METHOD(yiibase, autoload) {
	char *cname;
	int cname_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &cname, &cname_len) == FAILURE) {
		return;
	}

	if (yii_autoload(cname, cname_len) == FAILURE) {
		RETURN_FALSE;
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/** {{{ proto public static YiiBase::trace($msg,$category='application')
*/
PHP_METHOD(yiibase, trace) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::log($msg,$level=CLogger::LEVEL_INFO,$category='application')
*/
PHP_METHOD(yiibase, log) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::beginProfile($token,$category='application')
*/
PHP_METHOD(yiibase, beginProfile) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::endProfile($token,$category='application')
*/
PHP_METHOD(yiibase, endProfile) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::getLogger(void)
*/
PHP_METHOD(yiibase, getLogger) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::setLogger($logger)
*/
PHP_METHOD(yiibase, setLogger) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::powered()
*/
PHP_METHOD(yiibase, powered) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::t($category,$message,$params=array(),$source=null,$language=null)
*/
PHP_METHOD(yiibase, t) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ proto public static YiiBase::registerAutoloader($callback, $append=false)
*/
PHP_METHOD(yiibase, registerAutoloader) {
	RETURN_STRING(YII_VERSION, 1);
}
/* }}} */

/** {{{ yiibase_methods[]
*/
zend_function_entry yiibase_methods[] = {
	PHP_ME(yiibase, getVersion, yiibase_getversion_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, createWebApplication, yiibase_createwebapp_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, createConsoleApplication, yiibase_createconsoleapp_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, createApplication, yiibase_createapplication_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, app, yiibase_app_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, setApplication, yiibase_setapplication_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, getFrameworkPath, yiibase_getframeworkpath_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, createComponent, yiibase_createcomponent_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, import, yiibase_import_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, getPathOfAlias, yiibase_getpathofalias_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, setPathOfAlias, yiibase_setpathofalias_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, autoload, yiibase_autoload_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, trace, yiibase_trace_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, log, yiibase_log_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, beginProfile, yiibase_beginprofile_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, endProfile, yiibase_endprofile_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, getLogger, yiibase_getlogger_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, setLogger, yiibase_setlogger_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, powered, yiibase_powered_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, t, yiibase_t_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yiibase, registerAutoloader, yiibase_autoload_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yiibase) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "YiiBase", yiibase_methods);
	yiibase_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_declare_property_null(yiibase_ce, ZEND_STRL("classMap"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_bool(yiibase_ce, ZEND_STRL("enableIncludePath"), 1, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yiibase_ce, ZEND_STRL("_aliases"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yiibase_ce, ZEND_STRL("_imports"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yiibase_ce, ZEND_STRL("_includePaths"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yiibase_ce, ZEND_STRL("_app"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yiibase_ce, ZEND_STRL("_logger"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yiibase_ce, ZEND_STRL("_coreClasses"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(yiibase) {
	zval *classmap;
	zval *aliases;
	zval *imports;
	zval *coreclasses;

	MAKE_STD_ZVAL(classmap);
	array_init(classmap);
	zend_update_static_property(yiibase_ce, ZEND_STRL("classMap"), classmap TSRMLS_CC);
	
	MAKE_STD_ZVAL(aliases);
	array_init(aliases);
	add_assoc_string(aliases, "system", YII_G(path), 0);
	add_assoc_string(aliases, "zii", YII_G(zii_path), 0);
	zend_update_static_property(yiibase_ce, ZEND_STRL("_aliases"), aliases TSRMLS_CC);

	MAKE_STD_ZVAL(imports);
	array_init(imports);
	zend_update_static_property(yiibase_ce, ZEND_STRL("_imports"), imports TSRMLS_CC);

	MAKE_STD_ZVAL(coreclasses);
	array_init(coreclasses);
	add_assoc_string(coreclasses, "CApplication", "/base/CApplication.php", 0);
	add_assoc_string(coreclasses, "CApplicationComponent", "/base/CApplicationComponent.php", 0);
	add_assoc_string(coreclasses, "CBehavior", "/base/CBehavior.php", 0);
	add_assoc_string(coreclasses, "CComponent", "/base/CComponent.php", 0);
	add_assoc_string(coreclasses, "CErrorEvent", "/base/CErrorEvent.php", 0);
	add_assoc_string(coreclasses, "CErrorHandler", "/base/CErrorHandler.php", 0);
	add_assoc_string(coreclasses, "CException", "/base/CException.php", 0);
	add_assoc_string(coreclasses, "CExceptionEvent", "/base/CExceptionEvent.php", 0);
	add_assoc_string(coreclasses, "CHttpException", "/base/CHttpException.php", 0);
	add_assoc_string(coreclasses, "CModel", "/base/CModel.php", 0);
	add_assoc_string(coreclasses, "CModelBehavior", "/base/CModelBehavior.php", 0);
	add_assoc_string(coreclasses, "CModelEvent", "/base/CModelEvent.php", 0);
	add_assoc_string(coreclasses, "CModule", "/base/CModule.php", 0);
	add_assoc_string(coreclasses, "CSecurityManager", "/base/CSecurityManager.php", 0);
	add_assoc_string(coreclasses, "CStatePersister", "/base/CStatePersister.php", 0);
	add_assoc_string(coreclasses, "CApcCache", "/caching/CApcCache.php", 0);
	add_assoc_string(coreclasses, "CCache", "/caching/CCache.php", 0);
	add_assoc_string(coreclasses, "CDbCache", "/caching/CDbCache.php", 0);
	add_assoc_string(coreclasses, "CDummyCache", "/caching/CDummyCache.php", 0);
	add_assoc_string(coreclasses, "CEAcceleratorCache", "/caching/CEAcceleratorCache.php", 0);
	add_assoc_string(coreclasses, "CFileCache", "/caching/CFileCache.php", 0);
	add_assoc_string(coreclasses, "CMemCache", "/caching/CMemCache.php", 0);
	add_assoc_string(coreclasses, "CWinCache", "/caching/CWinCache.php", 0);
	add_assoc_string(coreclasses, "CXCache", "/caching/CXCache.php", 0);
	add_assoc_string(coreclasses, "CZendDataCache", "/caching/CZendDataCache.php", 0);
	add_assoc_string(coreclasses, "CCacheDependency", "/caching/dependencies/CCacheDependency.php", 0);
	add_assoc_string(coreclasses, "CChainedCacheDependency", "/caching/dependencies/CChainedCacheDependency.php", 0);
	add_assoc_string(coreclasses, "CDbCacheDependency", "/caching/dependencies/CDbCacheDependency.php", 0);
	add_assoc_string(coreclasses, "CDirectoryCacheDependency", "/caching/dependencies/CDirectoryCacheDependency.php", 0);
	add_assoc_string(coreclasses, "CExpressionDependency", "/caching/dependencies/CExpressionDependency.php", 0);
	add_assoc_string(coreclasses, "CFileCacheDependency", "/caching/dependencies/CFileCacheDependency.php", 0);
	add_assoc_string(coreclasses, "CGlobalStateCacheDependency", "/caching/dependencies/CGlobalStateCacheDependency.php", 0);
	add_assoc_string(coreclasses, "CAttributeCollection", "/collections/CAttributeCollection.php", 0);
	add_assoc_string(coreclasses, "CConfiguration", "/collections/CConfiguration.php", 0);
	add_assoc_string(coreclasses, "CList", "/collections/CList.php", 0);
	add_assoc_string(coreclasses, "CListIterator", "/collections/CListIterator.php", 0);
	add_assoc_string(coreclasses, "CMap", "/collections/CMap.php", 0);
	add_assoc_string(coreclasses, "CMapIterator", "/collections/CMapIterator.php", 0);
	add_assoc_string(coreclasses, "CQueue", "/collections/CQueue.php", 0);
	add_assoc_string(coreclasses, "CQueueIterator", "/collections/CQueueIterator.php", 0);
	add_assoc_string(coreclasses, "CStack", "/collections/CStack.php", 0);
	add_assoc_string(coreclasses, "CStackIterator", "/collections/CStackIterator.php", 0);
	add_assoc_string(coreclasses, "CTypedList", "/collections/CTypedList.php", 0);
	add_assoc_string(coreclasses, "CTypedMap", "/collections/CTypedMap.php", 0);
	add_assoc_string(coreclasses, "CConsoleApplication", "/console/CConsoleApplication.php", 0);
	add_assoc_string(coreclasses, "CConsoleCommand", "/console/CConsoleCommand.php", 0);
	add_assoc_string(coreclasses, "CConsoleCommandBehavior", "/console/CConsoleCommandBehavior.php", 0);
	add_assoc_string(coreclasses, "CConsoleCommandEvent", "/console/CConsoleCommandEvent.php", 0);
	add_assoc_string(coreclasses, "CConsoleCommandRunner", "/console/CConsoleCommandRunner.php", 0);
	add_assoc_string(coreclasses, "CHelpCommand", "/console/CHelpCommand.php", 0);
	add_assoc_string(coreclasses, "CDbCommand", "/db/CDbCommand.php", 0);
	add_assoc_string(coreclasses, "CDbConnection", "/db/CDbConnection.php", 0);
	add_assoc_string(coreclasses, "CDbDataReader", "/db/CDbDataReader.php", 0);
	add_assoc_string(coreclasses, "CDbException", "/db/CDbException.php", 0);
	add_assoc_string(coreclasses, "CDbMigration", "/db/CDbMigration.php", 0);
	add_assoc_string(coreclasses, "CDbTransaction", "/db/CDbTransaction.php", 0);
	add_assoc_string(coreclasses, "CActiveFinder", "/db/ar/CActiveFinder.php", 0);
	add_assoc_string(coreclasses, "CActiveRecord", "/db/ar/CActiveRecord.php", 0);
	add_assoc_string(coreclasses, "CActiveRecordBehavior", "/db/ar/CActiveRecordBehavior.php", 0);
	add_assoc_string(coreclasses, "CDbColumnSchema", "/db/schema/CDbColumnSchema.php", 0);
	add_assoc_string(coreclasses, "CDbCommandBuilder", "/db/schema/CDbCommandBuilder.php", 0);
	add_assoc_string(coreclasses, "CDbCriteria", "/db/schema/CDbCriteria.php", 0);
	add_assoc_string(coreclasses, "CDbExpression", "/db/schema/CDbExpression.php", 0);
	add_assoc_string(coreclasses, "CDbSchema", "/db/schema/CDbSchema.php", 0);
	add_assoc_string(coreclasses, "CDbTableSchema", "/db/schema/CDbTableSchema.php", 0);
	add_assoc_string(coreclasses, "CMssqlColumnSchema", "/db/schema/mssql/CMssqlColumnSchema.php", 0);
	add_assoc_string(coreclasses, "CMssqlCommandBuilder", "/db/schema/mssql/CMssqlCommandBuilder.php", 0);
	add_assoc_string(coreclasses, "CMssqlPdoAdapter", "/db/schema/mssql/CMssqlPdoAdapter.php", 0);
	add_assoc_string(coreclasses, "CMssqlSchema", "/db/schema/mssql/CMssqlSchema.php", 0);
	add_assoc_string(coreclasses, "CMssqlSqlsrvPdoAdapter", "/db/schema/mssql/CMssqlSqlsrvPdoAdapter.php", 0);
	add_assoc_string(coreclasses, "CMssqlTableSchema", "/db/schema/mssql/CMssqlTableSchema.php", 0);
	add_assoc_string(coreclasses, "CMysqlColumnSchema", "/db/schema/mysql/CMysqlColumnSchema.php", 0);
	add_assoc_string(coreclasses, "CMysqlCommandBuilder", "/db/schema/mysql/CMysqlCommandBuilder.php", 0);
	add_assoc_string(coreclasses, "CMysqlSchema", "/db/schema/mysql/CMysqlSchema.php", 0);
	add_assoc_string(coreclasses, "CMysqlTableSchema", "/db/schema/mysql/CMysqlTableSchema.php", 0);
	add_assoc_string(coreclasses, "COciColumnSchema", "/db/schema/oci/COciColumnSchema.php", 0);
	add_assoc_string(coreclasses, "COciCommandBuilder", "/db/schema/oci/COciCommandBuilder.php", 0);
	add_assoc_string(coreclasses, "COciSchema", "/db/schema/oci/COciSchema.php", 0);
	add_assoc_string(coreclasses, "COciTableSchema", "/db/schema/oci/COciTableSchema.php", 0);
	add_assoc_string(coreclasses, "CPgsqlColumnSchema", "/db/schema/pgsql/CPgsqlColumnSchema.php", 0);
	add_assoc_string(coreclasses, "CPgsqlSchema", "/db/schema/pgsql/CPgsqlSchema.php", 0);
	add_assoc_string(coreclasses, "CPgsqlTableSchema", "/db/schema/pgsql/CPgsqlTableSchema.php", 0);
	add_assoc_string(coreclasses, "CSqliteColumnSchema", "/db/schema/sqlite/CSqliteColumnSchema.php", 0);
	add_assoc_string(coreclasses, "CSqliteCommandBuilder", "/db/schema/sqlite/CSqliteCommandBuilder.php", 0);
	add_assoc_string(coreclasses, "CSqliteSchema", "/db/schema/sqlite/CSqliteSchema.php", 0);
	add_assoc_string(coreclasses, "CChoiceFormat", "/i18n/CChoiceFormat.php", 0);
	add_assoc_string(coreclasses, "CDateFormatter", "/i18n/CDateFormatter.php", 0);
	add_assoc_string(coreclasses, "CDbMessageSource", "/i18n/CDbMessageSource.php", 0);
	add_assoc_string(coreclasses, "CGettextMessageSource", "/i18n/CGettextMessageSource.php", 0);
	add_assoc_string(coreclasses, "CLocale", "/i18n/CLocale.php", 0);
	add_assoc_string(coreclasses, "CMessageSource", "/i18n/CMessageSource.php", 0);
	add_assoc_string(coreclasses, "CNumberFormatter", "/i18n/CNumberFormatter.php", 0);
	add_assoc_string(coreclasses, "CPhpMessageSource", "/i18n/CPhpMessageSource.php", 0);
	add_assoc_string(coreclasses, "CGettextFile", "/i18n/gettext/CGettextFile.php", 0);
	add_assoc_string(coreclasses, "CGettextMoFile", "/i18n/gettext/CGettextMoFile.php", 0);
	add_assoc_string(coreclasses, "CGettextPoFile", "/i18n/gettext/CGettextPoFile.php", 0);
	add_assoc_string(coreclasses, "CChainedLogFilter", "/logging/CChainedLogFilter.php", 0);
	add_assoc_string(coreclasses, "CDbLogRoute", "/logging/CDbLogRoute.php", 0);
	add_assoc_string(coreclasses, "CEmailLogRoute", "/logging/CEmailLogRoute.php", 0);
	add_assoc_string(coreclasses, "CFileLogRoute", "/logging/CFileLogRoute.php", 0);
	add_assoc_string(coreclasses, "CLogFilter", "/logging/CLogFilter.php", 0);
	add_assoc_string(coreclasses, "CLogRoute", "/logging/CLogRoute.php", 0);
	add_assoc_string(coreclasses, "CLogRouter", "/logging/CLogRouter.php", 0);
	add_assoc_string(coreclasses, "CLogger", "/logging/CLogger.php", 0);
	add_assoc_string(coreclasses, "CProfileLogRoute", "/logging/CProfileLogRoute.php", 0);
	add_assoc_string(coreclasses, "CWebLogRoute", "/logging/CWebLogRoute.php", 0);
	add_assoc_string(coreclasses, "CDateTimeParser", "/utils/CDateTimeParser.php", 0);
	add_assoc_string(coreclasses, "CFileHelper", "/utils/CFileHelper.php", 0);
	add_assoc_string(coreclasses, "CFormatter", "/utils/CFormatter.php", 0);
	add_assoc_string(coreclasses, "CMarkdownParser", "/utils/CMarkdownParser.php", 0);
	add_assoc_string(coreclasses, "CPropertyValue", "/utils/CPropertyValue.php", 0);
	add_assoc_string(coreclasses, "CTimestamp", "/utils/CTimestamp.php", 0);
	add_assoc_string(coreclasses, "CVarDumper", "/utils/CVarDumper.php", 0);
	add_assoc_string(coreclasses, "CBooleanValidator", "/validators/CBooleanValidator.php", 0);
	add_assoc_string(coreclasses, "CCaptchaValidator", "/validators/CCaptchaValidator.php", 0);
	add_assoc_string(coreclasses, "CCompareValidator", "/validators/CCompareValidator.php", 0);
	add_assoc_string(coreclasses, "CDateValidator", "/validators/CDateValidator.php", 0);
	add_assoc_string(coreclasses, "CDefaultValueValidator", "/validators/CDefaultValueValidator.php", 0);
	add_assoc_string(coreclasses, "CEmailValidator", "/validators/CEmailValidator.php", 0);
	add_assoc_string(coreclasses, "CExistValidator", "/validators/CExistValidator.php", 0);
	add_assoc_string(coreclasses, "CFileValidator", "/validators/CFileValidator.php", 0);
	add_assoc_string(coreclasses, "CFilterValidator", "/validators/CFilterValidator.php", 0);
	add_assoc_string(coreclasses, "CInlineValidator", "/validators/CInlineValidator.php", 0);
	add_assoc_string(coreclasses, "CNumberValidator", "/validators/CNumberValidator.php", 0);
	add_assoc_string(coreclasses, "CRangeValidator", "/validators/CRangeValidator.php", 0);
	add_assoc_string(coreclasses, "CRegularExpressionValidator", "/validators/CRegularExpressionValidator.php", 0);
	add_assoc_string(coreclasses, "CRequiredValidator", "/validators/CRequiredValidator.php", 0);
	add_assoc_string(coreclasses, "CSafeValidator", "/validators/CSafeValidator.php", 0);
	add_assoc_string(coreclasses, "CStringValidator", "/validators/CStringValidator.php", 0);
	add_assoc_string(coreclasses, "CTypeValidator", "/validators/CTypeValidator.php", 0);
	add_assoc_string(coreclasses, "CUniqueValidator", "/validators/CUniqueValidator.php", 0);
	add_assoc_string(coreclasses, "CUnsafeValidator", "/validators/CUnsafeValidator.php", 0);
	add_assoc_string(coreclasses, "CUrlValidator", "/validators/CUrlValidator.php", 0);
	add_assoc_string(coreclasses, "CValidator", "/validators/CValidator.php", 0);
	add_assoc_string(coreclasses, "CActiveDataProvider", "/web/CActiveDataProvider.php", 0);
	add_assoc_string(coreclasses, "CArrayDataProvider", "/web/CArrayDataProvider.php", 0);
	add_assoc_string(coreclasses, "CAssetManager", "/web/CAssetManager.php", 0);
	add_assoc_string(coreclasses, "CBaseController", "/web/CBaseController.php", 0);
	add_assoc_string(coreclasses, "CCacheHttpSession", "/web/CCacheHttpSession.php", 0);
	add_assoc_string(coreclasses, "CClientScript", "/web/CClientScript.php", 0);
	add_assoc_string(coreclasses, "CController", "/web/CController.php", 0);
	add_assoc_string(coreclasses, "CDataProvider", "/web/CDataProvider.php", 0);
	add_assoc_string(coreclasses, "CDataProviderIterator", "/web/CDataProviderIterator.php", 0);
	add_assoc_string(coreclasses, "CDbHttpSession", "/web/CDbHttpSession.php", 0);
	add_assoc_string(coreclasses, "CExtController", "/web/CExtController.php", 0);
	add_assoc_string(coreclasses, "CFormModel", "/web/CFormModel.php", 0);
	add_assoc_string(coreclasses, "CHttpCookie", "/web/CHttpCookie.php", 0);
	add_assoc_string(coreclasses, "CHttpRequest", "/web/CHttpRequest.php", 0);
	add_assoc_string(coreclasses, "CHttpSession", "/web/CHttpSession.php", 0);
	add_assoc_string(coreclasses, "CHttpSessionIterator", "/web/CHttpSessionIterator.php", 0);
	add_assoc_string(coreclasses, "COutputEvent", "/web/COutputEvent.php", 0);
	add_assoc_string(coreclasses, "CPagination", "/web/CPagination.php", 0);
	add_assoc_string(coreclasses, "CSort", "/web/CSort.php", 0);
	add_assoc_string(coreclasses, "CSqlDataProvider", "/web/CSqlDataProvider.php", 0);
	add_assoc_string(coreclasses, "CTheme", "/web/CTheme.php", 0);
	add_assoc_string(coreclasses, "CThemeManager", "/web/CThemeManager.php", 0);
	add_assoc_string(coreclasses, "CUploadedFile", "/web/CUploadedFile.php", 0);
	add_assoc_string(coreclasses, "CUrlManager", "/web/CUrlManager.php", 0);
	add_assoc_string(coreclasses, "CWebApplication", "/web/CWebApplication.php", 0);
	add_assoc_string(coreclasses, "CWebModule", "/web/CWebModule.php", 0);
	add_assoc_string(coreclasses, "CWidgetFactory", "/web/CWidgetFactory.php", 0);
	add_assoc_string(coreclasses, "CAction", "/web/actions/CAction.php", 0);
	add_assoc_string(coreclasses, "CInlineAction", "/web/actions/CInlineAction.php", 0);
	add_assoc_string(coreclasses, "CViewAction", "/web/actions/CViewAction.php", 0);
	add_assoc_string(coreclasses, "CAccessControlFilter", "/web/auth/CAccessControlFilter.php", 0);
	add_assoc_string(coreclasses, "CAuthAssignment", "/web/auth/CAuthAssignment.php", 0);
	add_assoc_string(coreclasses, "CAuthItem", "/web/auth/CAuthItem.php", 0);
	add_assoc_string(coreclasses, "CAuthManager", "/web/auth/CAuthManager.php", 0);
	add_assoc_string(coreclasses, "CBaseUserIdentity", "/web/auth/CBaseUserIdentity.php", 0);
	add_assoc_string(coreclasses, "CDbAuthManager", "/web/auth/CDbAuthManager.php", 0);
	add_assoc_string(coreclasses, "CPhpAuthManager", "/web/auth/CPhpAuthManager.php", 0);
	add_assoc_string(coreclasses, "CUserIdentity", "/web/auth/CUserIdentity.php", 0);
	add_assoc_string(coreclasses, "CWebUser", "/web/auth/CWebUser.php", 0);
	add_assoc_string(coreclasses, "CFilter", "/web/filters/CFilter.php", 0);
	add_assoc_string(coreclasses, "CFilterChain", "/web/filters/CFilterChain.php", 0);
	add_assoc_string(coreclasses, "CHttpCacheFilter", "/web/filters/CHttpCacheFilter.php", 0);
	add_assoc_string(coreclasses, "CInlineFilter", "/web/filters/CInlineFilter.php", 0);
	add_assoc_string(coreclasses, "CForm", "/web/form/CForm.php", 0);
	add_assoc_string(coreclasses, "CFormButtonElement", "/web/form/CFormButtonElement.php", 0);
	add_assoc_string(coreclasses, "CFormElement", "/web/form/CFormElement.php", 0);
	add_assoc_string(coreclasses, "CFormElementCollection", "/web/form/CFormElementCollection.php", 0);
	add_assoc_string(coreclasses, "CFormInputElement", "/web/form/CFormInputElement.php", 0);
	add_assoc_string(coreclasses, "CFormStringElement", "/web/form/CFormStringElement.php", 0);
	add_assoc_string(coreclasses, "CGoogleApi", "/web/helpers/CGoogleApi.php", 0);
	add_assoc_string(coreclasses, "CHtml", "/web/helpers/CHtml.php", 0);
	add_assoc_string(coreclasses, "CJSON", "/web/helpers/CJSON.php", 0);
	add_assoc_string(coreclasses, "CJavaScript", "/web/helpers/CJavaScript.php", 0);
	add_assoc_string(coreclasses, "CJavaScriptExpression", "/web/helpers/CJavaScriptExpression.php", 0);
	add_assoc_string(coreclasses, "CPradoViewRenderer", "/web/renderers/CPradoViewRenderer.php", 0);
	add_assoc_string(coreclasses, "CViewRenderer", "/web/renderers/CViewRenderer.php", 0);
	add_assoc_string(coreclasses, "CWebService", "/web/services/CWebService.php", 0);
	add_assoc_string(coreclasses, "CWebServiceAction", "/web/services/CWebServiceAction.php", 0);
	add_assoc_string(coreclasses, "CWsdlGenerator", "/web/services/CWsdlGenerator.php", 0);
	add_assoc_string(coreclasses, "CActiveForm", "/web/widgets/CActiveForm.php", 0);
	add_assoc_string(coreclasses, "CAutoComplete", "/web/widgets/CAutoComplete.php", 0);
	add_assoc_string(coreclasses, "CClipWidget", "/web/widgets/CClipWidget.php", 0);
	add_assoc_string(coreclasses, "CContentDecorator", "/web/widgets/CContentDecorator.php", 0);
	add_assoc_string(coreclasses, "CFilterWidget", "/web/widgets/CFilterWidget.php", 0);
	add_assoc_string(coreclasses, "CFlexWidget", "/web/widgets/CFlexWidget.php", 0);
	add_assoc_string(coreclasses, "CHtmlPurifier", "/web/widgets/CHtmlPurifier.php", 0);
	add_assoc_string(coreclasses, "CInputWidget", "/web/widgets/CInputWidget.php", 0);
	add_assoc_string(coreclasses, "CMarkdown", "/web/widgets/CMarkdown.php", 0);
	add_assoc_string(coreclasses, "CMaskedTextField", "/web/widgets/CMaskedTextField.php", 0);
	add_assoc_string(coreclasses, "CMultiFileUpload", "/web/widgets/CMultiFileUpload.php", 0);
	add_assoc_string(coreclasses, "COutputCache", "/web/widgets/COutputCache.php", 0);
	add_assoc_string(coreclasses, "COutputProcessor", "/web/widgets/COutputProcessor.php", 0);
	add_assoc_string(coreclasses, "CStarRating", "/web/widgets/CStarRating.php", 0);
	add_assoc_string(coreclasses, "CTabView", "/web/widgets/CTabView.php", 0);
	add_assoc_string(coreclasses, "CTextHighlighter", "/web/widgets/CTextHighlighter.php", 0);
	add_assoc_string(coreclasses, "CTreeView", "/web/widgets/CTreeView.php", 0);
	add_assoc_string(coreclasses, "CWidget", "/web/widgets/CWidget.php", 0);
	add_assoc_string(coreclasses, "CCaptcha", "/web/widgets/captcha/CCaptcha.php", 0);
	add_assoc_string(coreclasses, "CCaptchaAction", "/web/widgets/captcha/CCaptchaAction.php", 0);
	add_assoc_string(coreclasses, "CBasePager", "/web/widgets/pagers/CBasePager.php", 0);
	add_assoc_string(coreclasses, "CLinkPager", "/web/widgets/pagers/CLinkPager.php", 0);
	add_assoc_string(coreclasses, "CListPager", "/web/widgets/pagers/CListPager.php", 0);
	zend_update_static_property(yiibase_ce, ZEND_STRL("_coreClasses"), coreclasses TSRMLS_CC);

	// spl_autoload_register
	zval function, retval, *param[1];

	INIT_ZVAL(function);
	ZVAL_STRING(&function, "spl_autoload_register", 0);
	MAKE_STD_ZVAL(param[0]);
	array_init(param[0]);
	add_next_index_string(param[0], yiibase_ce->name, 0);
	add_next_index_string(param[0], "autoload", 0);
	call_user_function(CG(function_table), NULL, &function, &retval, 1, param TSRMLS_CC);
	FREE_ZVAL(param[0]);
	
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
