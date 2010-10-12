/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2008 The PHP Group                                |
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

/*for curl*/
#include <stdio.h>
#include <string.h>

/* somewhat unix-specific */
#include "ext/standard/php_smart_str.h"
#include <sys/time.h>
#include <unistd.h>
#include <curl/curl.h>
/* curl stuff */

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifndef PHP_CONCURRENCY_H
#define PHP_CONCURRENCY_H

extern zend_module_entry concurrency_module_entry;
#define phpext_concurrency_ptr &concurrency_module_entry

#ifdef PHP_WIN32
#	define PHP_CONCURRENCY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_CONCURRENCY_API __attribute__ ((visibility("default")))
#else
#	define PHP_CONCURRENCY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(concurrency);
PHP_MSHUTDOWN_FUNCTION(concurrency);
PHP_RINIT_FUNCTION(concurrency);
PHP_RSHUTDOWN_FUNCTION(concurrency);
PHP_MINFO_FUNCTION(concurrency);

PHP_FUNCTION(confirm_concurrency_compiled);	/* For testing, remove later. */

/* 
   Declare any global variables you may need between the BEGIN
   and END macros here:     

   ZEND_BEGIN_MODULE_GLOBALS(concurrency)
   long  global_value;
   char *global_string;
   ZEND_END_MODULE_GLOBALS(concurrency)
   */

/* In every utility function you add that needs to use variables 
   in php_concurrency_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as CONCURRENCY_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
   */

#ifdef ZTS
#define CONCURRENCY_G(v) TSRMG(concurrency_globals_id, zend_concurrency_globals *, v)
#else
#define CONCURRENCY_G(v) (concurrency_globals.v)
#endif

#endif	/* PHP_CONCURRENCY_H */

struct  concurrency_request_struct {
	int buf_index;
	zval *object;
	CURL *curl;
	char *buf;
	void ***thread_ctx;
}; 

typedef struct concurrency_request_struct concurrency_request_handler;


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
