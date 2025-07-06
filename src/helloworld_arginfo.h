/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e906f73aa59cb7c43c26ed2a899eb9830d092553 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test1, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test2, 0, 0, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, str, IS_STRING, 0, "\"\"")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test3, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, ref_file, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, edit_file, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(test1);
ZEND_FUNCTION(test2);
ZEND_FUNCTION(test3);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(test1, arginfo_test1)
	ZEND_FE(test2, arginfo_test2)
	ZEND_FE(test3, arginfo_test3)
	ZEND_FE_END
};
