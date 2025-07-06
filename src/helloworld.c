/* helloworld extension for PHP */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_helloworld.h"
#include "include/file1.h"
#include "include/file2.h"
#include "helloworld_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

PHP_FUNCTION(test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "helloworld");
}

PHP_FUNCTION(test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}

PHP_FUNCTION(test3)
{
	zend_string *ref_file;
	zval *edit_file;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(ref_file)
		Z_PARAM_ZVAL(edit_file)
	ZEND_PARSE_PARAMETERS_END();

	int ret = 0;

	// ret = func_1_1(ret);
	// ret = func_1_2(ret);
	// ret = func_2_1(ret);
	// ret = func_2_2(ret);

	char *resultString = "aaaaa";
	size_t resultString_length = 5;

	zend_try_assign_typed_ref_stringl(edit_file->value.ref, resultString, resultString_length);

	__clear_cache;

	RETURN_LONG(ret);
}

PHP_RINIT_FUNCTION(helloworld)
{
#if defined(ZTS) && defined(COMPILE_DL_HELLOWORLD)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}

PHP_MINFO_FUNCTION(helloworld)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "helloworld support", "enabled");
	php_info_print_table_end();
}

zend_module_entry helloworld_module_entry = {
	STANDARD_MODULE_HEADER,
	"helloworld",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(helloworld),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(helloworld),			/* PHP_MINFO - Module info */
	PHP_HELLOWORLD_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HELLOWORLD
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(helloworld)
#endif
