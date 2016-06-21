/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
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
 
/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_simple_guard.h"

#define  HEAD_SIGN "simple_guard"
static int file_encrypt(FILE *fpread, FILE *fpwrite, const char *key)/*{{{*/
{
	int keylen = strlen(key);
	char *buffer = (char *)emalloc(keylen + 1);
	int readlen = 0,i;

	while (!feof(fpread)){
		readlen = fread(buffer, 1, keylen, fpread);
		for (i = 0; i < readlen; i++){
			fputc(buffer[i] ^ key[i], fpwrite);
		}
	}
	efree(buffer);
	return SUCCESS;
}/*}}}*/

/* If you declare any globals in php_simple_guard.h uncomment this:
*/
ZEND_DECLARE_MODULE_GLOBALS(simple_guard)


zend_op_array *(*org_compile_file)(zend_file_handle *file_handle, int type TSRMLS_DC);

static zend_op_array *simple_guard_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)/*{{{*/
{
	char buf[12];	
	FILE *target = NULL, *source = NULL;
	do{
			source = fopen(file_handle->filename, "rb");
			if (!source){
				break;
			}
			fread(buf,sizeof(buf),1,source);
			if(memcmp(buf,HEAD_SIGN,sizeof(HEAD_SIGN)) !=0){
					fclose(source);
					break;
			}
			target =  tmpfile();
			if (file_encrypt(source, target, SIMPLE_GUARD_G(secert_key)) == FAILURE){
					fclose(source);
					fclose(target);
					break;
			}
			rewind(target);
			if (file_handle->type == ZEND_HANDLE_FP) {
				fclose(file_handle->handle.fp);
			}
			if (file_handle->type == ZEND_HANDLE_FD){
				close(file_handle->handle.fd);
			}
			if(file_handle->opened_path){
					efree(file_handle->opened_path);
			}
			file_handle->handle.fp = target;
			file_handle->type = ZEND_HANDLE_FP;
			file_handle->opened_path = expand_filepath(file_handle->filename, NULL TSRMLS_CC);
			fclose(source);
	} while (0);
	return org_compile_file(file_handle, type TSRMLS_CC);
}/*}}}*/

/* True global resources - no need for thread safety here */
static int le_simple_guard;

ZEND_BEGIN_ARG_INFO_EX(arginfo_simple_guard_encode,0,0,2)
ZEND_ARG_INFO(0,source_file)
ZEND_ARG_INFO(0,target_file)
ZEND_END_ARG_INFO()
/* {{{ simple_guard_functions[]
 *
 * Every user visible function must have an entry in simple_guard_functions[].
 */
const zend_function_entry simple_guard_functions[] = {
	PHP_FE(simple_guard_encode,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in simple_guard_functions[] */
};
/* }}} */

/* {{{ simple_guard_module_entry
 */
zend_module_entry simple_guard_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"simple_guard",
	simple_guard_functions,
	PHP_MINIT(simple_guard),
	PHP_MSHUTDOWN(simple_guard),
	PHP_RINIT(simple_guard),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(simple_guard),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(simple_guard),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SIMPLE_GUARD_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIMPLE_GUARD
ZEND_GET_MODULE(simple_guard)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
*/
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("simple_guard.key", "simple_guard", PHP_INI_SYSTEM, OnUpdateString, secert_key, zend_simple_guard_globals, simple_guard_globals)
PHP_INI_END()

/* }}} */

/* {{{ php_simple_guard_init_globals
 */
/* Uncomment this function if you have INI entries
*/
static void php_simple_guard_init_globals(zend_simple_guard_globals *simple_guard_globals)
{
	simple_guard_globals->secert_key = NULL;
}

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(simple_guard)
{
	/* If you have INI entries, uncomment these lines 
	*/
	REGISTER_INI_ENTRIES();
	org_compile_file = zend_compile_file;
	zend_compile_file = simple_guard_compile_file;
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(simple_guard)
{
	/* uncomment this line if you have INI entries
	*/
	UNREGISTER_INI_ENTRIES();
	zend_compile_file = org_compile_file;
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(simple_guard)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(simple_guard)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(simple_guard)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "simple_guard support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_simple_guard_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(simple_guard_encode)
{
	char *source_file = NULL,*target_file=NULL;
	int source_file_len,target_file_len, len;
	FILE *source,*target;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &source_file, &source_file_len,&target_file,&target_file_len) == FAILURE) {
		return;
	}

	source = fopen(source_file, "rb");
	if(!source){
		php_error_docref(NULL TSRMLS_CC,E_NOTICE,"open file %s failed\n",source_file);
		RETURN_FALSE;
	}
	target = fopen(target_file, "wb");
	if(!target){
		fclose(source);
		php_error_docref(NULL TSRMLS_CC,E_NOTICE,"open file %s failed\n",target_file);
		RETURN_FALSE;
	}
	fwrite((void *)HEAD_SIGN,sizeof(HEAD_SIGN)-1,1,target);
	if (file_encrypt(source, target, SIMPLE_GUARD_G(secert_key))==SUCCESS){
		RETVAL_BOOL(1);
	}
	else{
		RETVAL_BOOL(0);
	}
	fclose(source);
	fclose(target);
}
/* }}} */

/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
