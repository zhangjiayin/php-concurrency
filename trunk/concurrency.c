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

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_concurrency.h"
#include "string.h"

static size_t concurrency_curl_write();

/* If you declare any globals in php_concurrency.h uncomment this:
   ZEND_DECLARE_MODULE_GLOBALS(concurrency)
   */

/* True global resources - no need for thread safety here */


#define PHP_CONCURRENCY_CLASSNAME "Concurrency"
#define PHP_CONCURRENCY_REQUEST_CLASSNAME "ConcurrencyRequest"
#define PHP_CONCURRENCY_RESPONSE_CLASSNAME "ConcurrencyResponse"


zend_class_entry *concurrency_class_entry;
zend_class_entry *concurrency_request_class_entry;
zend_class_entry *concurrency_response_class_entry;

PHP_METHOD(Concurrency, request);
PHP_METHOD(Concurrency, addRequest);
PHP_METHOD(Concurrency, __construct);
PHP_METHOD(Concurrency, test);

	static const zend_function_entry concurrency_class_methods[] = {
		PHP_ME(Concurrency, request, NULL, 0)
			PHP_ME(Concurrency, addRequest, NULL, 0)
			PHP_ME(Concurrency, __construct, NULL, 0)
			//		PHP_ME(Concurrency, test, NULL, 0)
			{NULL, NULL, NULL}
	};

static const zend_function_entry concurrency_request_class_methods[] = {
	{NULL, NULL, NULL}
};


static const zend_function_entry concurrency_response_class_methods[] = {
	{NULL, NULL, NULL}
};

/* {{{ concurrency_functions[]
 *
 * Every user visible function must have an entry in concurrency_functions[].
 */
	const zend_function_entry concurrency_functions[] = {
		ZEND_FE(confirm_concurrency_compiled,	NULL)		/* For testing, remove later. */
		{NULL, NULL, NULL}	/* Must be the last line in concurrency_functions[] */
	};
/* }}} */

/* {{{ concurrency_module_entry
*/
zend_module_entry concurrency_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"concurrency",
	concurrency_functions,
	PHP_MINIT(concurrency),
	PHP_MSHUTDOWN(concurrency),
	PHP_RINIT(concurrency),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(concurrency),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(concurrency),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CONCURRENCY
ZEND_GET_MODULE(concurrency)
#endif

	/* {{{ PHP_INI
	*/
	/* Remove comments and fill if you need to have entries in php.ini
	   PHP_INI_BEGIN()
	   STD_PHP_INI_ENTRY("concurrency.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_concurrency_globals, concurrency_globals)
	   STD_PHP_INI_ENTRY("concurrency.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_concurrency_globals, concurrency_globals)
	   PHP_INI_END()
	   */
	/* }}} */

	/* {{{ php_concurrency_init_globals
	*/
	/* Uncomment this function if you have INI entries
	   static void php_concurrency_init_globals(zend_concurrency_globals *concurrency_globals)
	   {
	   concurrency_globals->global_value = 0;
	   concurrency_globals->global_string = NULL;
	   }
	   */
	/* }}} */

	/* {{{ PHP_MINIT_FUNCTION
	*/
PHP_MINIT_FUNCTION(concurrency)
{
	/* If you have INI entries, uncomment these lines 
	   REGISTER_INI_ENTRIES();
	   */
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, PHP_CONCURRENCY_CLASSNAME, concurrency_class_methods);
	concurrency_class_entry = zend_register_internal_class(&ce TSRMLS_CC);

	//zend_declare_property_null(concurrency_class_entry, "requests", strlen("requests"), ZEND_ACC_PUBLIC);
	//zend_declare_property_null(concurrency_class_entry, "responses", strlen("responses"), ZEND_ACC_PUBLIC);


	INIT_CLASS_ENTRY(ce, PHP_CONCURRENCY_REQUEST_CLASSNAME, concurrency_request_class_methods);
	concurrency_request_class_entry = zend_register_internal_class(&ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, PHP_CONCURRENCY_RESPONSE_CLASSNAME, concurrency_response_class_methods);
	concurrency_response_class_entry = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(concurrency)
{
	/* uncomment this line if you have INI entries
	   UNREGISTER_INI_ENTRIES();
	   */
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(concurrency)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(concurrency)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(concurrency)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "concurrency support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	   DISPLAY_INI_ENTRIES();
	   */
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_concurrency_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_concurrency_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "concurrency", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
   */


PHP_METHOD(Concurrency, request)
{
	if(zend_parse_parameters_none() == FAILURE) {
		return;
	}

	zval *request,*requests;
	zval *this;

	this = getThis();

	requests = zend_read_property(concurrency_class_entry, this, "requests", strlen("requests"), 1 TSRMLS_CC);

	if ( Z_TYPE_P(requests) != IS_ARRAY) {
		return;
	}

	HashTable *arr_hash;
	zval **element;
	HashPosition pos;
	int arr_length;
	int arr_index = 0;

	arr_hash = Z_ARRVAL_P(requests);
	arr_length = zend_hash_num_elements(arr_hash);
	concurrency_request_handler *handler[arr_length];

	CURLM *multi_handle = NULL;
	multi_handle = curl_multi_init();

	int still_running; /* keep number of running handles */

	for (zend_hash_internal_pointer_reset_ex(arr_hash, &pos);
		 zend_hash_get_current_data_ex(arr_hash, (void **) &element, &pos) == SUCCESS;
		 zend_hash_move_forward_ex(arr_hash, &pos)
		) {

		long	 index = 0;
		uint key_len=0;
		char *key = NULL;

		if(!instanceof_function(Z_OBJCE_P(*element), concurrency_request_class_entry TSRMLS_CC)){
			continue;
		}

		zend_hash_get_current_key_ex(arr_hash, &key, &key_len, &index, 0, &pos);

		//if (zend_hash_get_current_key_ex(arr_hash, &key, &key_len, &index, 0, &pos) == HASH_KEY_IS_STRING) {
		//PHPWRITE(key, key_len);
		//} else {
		//php_printf("%ld", index);
		//}
		zval *url;

		url = zend_read_property(concurrency_request_class_entry, *element, "url", strlen("url"), 1 TSRMLS_CC);

		if(Z_TYPE_P(url) != IS_STRING) {
			continue;
		}

		CURL *http_handle;
		int  wr_error;

		http_handle = curl_easy_init();

		handler[arr_index] = emalloc(sizeof(concurrency_request_handler));
		handler[arr_index]->buf_index = 0;
		handler[arr_index]->object = *element;
		handler[arr_index]->curl = http_handle;
		handler[arr_index]->buf = 0;
		TSRMLS_SET_CTX(handler[arr_index]->thread_ctx);

		/* set the options (I left out a few, you'll get the point anyway) */
		curl_easy_setopt(http_handle, CURLOPT_URL, Z_STRVAL_P(url));
		curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION, concurrency_curl_write);
		curl_easy_setopt(http_handle, CURLOPT_WRITEDATA, (void *) handler[arr_index]);

		//	curl_easy_setopt(http_handle, CURLOPT_VERBOSE, 1L);

		curl_multi_add_handle(multi_handle, http_handle);

		arr_index++;
	}

	while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &still_running));

	while(still_running) {
		struct timeval timeout;
		int rc; /* select() return code */ 

		fd_set fdread;
		fd_set fdwrite;
		fd_set fdexcep;
		int maxfd = -1;

		long curl_timeo = -1;

		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcep);

		/* set a suitable timeout to play around with */ 
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		curl_multi_timeout(multi_handle, &curl_timeo);
		if(curl_timeo >= 0) {
			timeout.tv_sec = curl_timeo / 1000;
			if(timeout.tv_sec > 1)
				timeout.tv_sec = 1;
			else
				timeout.tv_usec = (curl_timeo % 1000) * 1000;
		}

		curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

		rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);


		switch(rc) {
		case -1:
			/* select error */ 
			still_running = 0;
			printf("select() returns error, this is badness\n");
			break;
		case 0:
		default:
			/* timeout or readable/writable sockets */ 
			while(CURLM_CALL_MULTI_PERFORM ==
				  curl_multi_perform(multi_handle, &still_running));
			break;
		}
	}



	int i = 0;
	for(i=0;i< arr_length; i++) {

		TSRMLS_FETCH_FROM_CTX(handler[i]->thread_ctx);

		if(handler[i]->buf != NULL) {
			add_property_string_ex(handler[i]->object, "result", sizeof("result") ,
								   (char *)handler[i]->buf, 1 TSRMLS_CC);
		} else {
			add_property_null(handler[i]->object, "result");
		}

		curl_easy_cleanup(handler[i]->curl);
		if(handler[i]->buf != NULL) {
			efree(handler[i]->buf);
		}
		efree(handler[i]);
	}

	curl_multi_cleanup(multi_handle);

}


PHP_METHOD(Concurrency, addRequest)
{
	zval *request,*requests;
	zval *this;

	char *key = NULL;
	int key_len = 0; 

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|s", &request,concurrency_request_class_entry, &key, &key_len) == FAILURE) {
		return;
	}

	this = getThis();

	requests = zend_read_property(concurrency_class_entry, this, "requests", strlen("requests"), 1 TSRMLS_CC);

	if ( Z_TYPE_P(requests) != IS_ARRAY) {
		MAKE_STD_ZVAL(requests);
		array_init(requests);
	}

	if(!key || !key_len) {
		add_next_index_zval(requests, request);
	} else {
		add_assoc_zval(requests, key, request);
	}

	zend_update_property(concurrency_class_entry, this, "requests", strlen("requests"), requests TSRMLS_CC);
}

PHP_METHOD(Concurrency, __construct)
{
	zval *this;
	this = getThis();

	zval *requests;
	MAKE_STD_ZVAL(requests);
	array_init(requests);

	add_property_zval_ex(this, "requests", sizeof("requests"), requests);
}

size_t concurrency_curl_write(void *ptr, size_t size, size_t nmemb, void *h)
{	


	size_t length = size * nmemb;

	if (length == 0) {
		return 0;
	}

	TSRMLS_FETCH_FROM_CTX(ch->thread_ctx);

	concurrency_request_handler *handler = (concurrency_request_handler *)h; 

	if(handler->buf == NULL) {
		handler->buf = (char *)emalloc(handler->buf_index + 2 + length);
		handler->buf_index += length ; 
		strncpy(handler->buf, (char *)ptr, length);
		handler->buf[handler->buf_index] = 0;
	} else {
		handler->buf = (char *)erealloc(handler->buf, handler->buf_index + 2 + length);
		memmove(handler->buf + handler->buf_index, (char *)ptr, (int)length);
		handler->buf_index += length; 
		handler->buf[handler->buf_index] = 0;
	}

	return length; 
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
