#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "php_tair.h"
#include "defined.h"
#include "tair_client_capi.hpp"

ZEND_DECLARE_MODULE_GLOBALS(tair)

/* True global resources - no need for thread safety here */
static int le_tair;
static int last_rst;

static void tair_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC) { 
	tair_handler t = rsrc->ptr;
	tair_deinit(t);
}


/* {{{ tair_functions[]
 *
 * Every user visible function must have an entry in tair_functions[].
 */
const zend_function_entry tair_functions[] = {
	PHP_FE(tair_get,	NULL)
		PHP_FE(tair_init,	NULL)
		PHP_FE(tair_put,	NULL)
		PHP_FE(tair_remove,	NULL)
		PHP_FE(tair_incr,	NULL)
		PHP_FE(tair_decr,	NULL)
		PHP_FE(tair_last_msg,	NULL)
		{NULL, NULL, NULL}	/* Must be the last line in tair_functions[] */
};
/* }}} */


zval * tair_pconnect(const *master_addr, char *slave_addr, char *group_name) /* {{{ */
{
	tair_handler tc;
	zend_rsrc_list_entry *le;
	char *hash_key;
	int hash_key_len;
    int rsrc_id;
	hash_key_len = spprintf(&hash_key, 0, "tair__%s@%s:%s", group_name,master_addr, slave_addr);

	if (zend_hash_find(&EG(persistent_list), hash_key, hash_key_len+1, (void **) &le) == FAILURE) {

        tc = tair_init();
        last_rst = tair_begin(tc,master_addr,slave_addr,group_name);
        if(last_rst != TAIR_RETURN_SUCCESS){
            return -1;
        }
        
        zend_rsrc_list_entry new_le;
    	new_le.type = le_tair;
		new_le.ptr  = tc;
		
		/* register new persistent connection */
		if (zend_hash_update(&EG(persistent_list), hash_key, hash_key_len+1, (void *) &new_le, sizeof(zend_rsrc_list_entry), NULL) == FAILURE) {
            tair_deinit(tc);
		} else {
            rsrc_id = zend_list_insert(tc,le_tair);
		}
	}else if (le->type != le_tair || le->ptr == NULL) {
		zend_hash_del(&EG(persistent_list), hash_key, hash_key_len+1);

        tc = tair_init();
        last_rst = tair_begin(tc,master_addr,slave_addr,group_name);
        if(last_rst != TAIR_RETURN_SUCCESS){
            return -1;
        }
        
        zend_rsrc_list_entry new_le;
    	new_le.type = le_tair;
		new_le.ptr  = tc;

		if (zend_hash_update(&EG(persistent_list), hash_key, hash_key_len+1, (void *) &new_le, sizeof(zend_rsrc_list_entry), NULL) == FAILURE) {
            tair_deinit(tc);
		} else {
            rsrc_id = zend_list_insert(tc,le_tair);
		}
	}else {
        tc = (tair_handler)le->ptr;
        rsrc_id = zend_list_insert(tc,le_tair);
	}

	efree(hash_key);
    return rsrc_id;
}

/* {{{ tair_module_entry
 */
zend_module_entry tair_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"tair",
	tair_functions,
	PHP_MINIT(tair),
	PHP_MSHUTDOWN(tair),
	NULL,		/* Replace with NULL if there's nothing to do at request start */
	NULL,	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(tair),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TAIR
ZEND_GET_MODULE(tair)
#endif

	/* {{{ PHP_INI */
PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("tair.log_level", "info", PHP_INI_ALL,OnUpdateString,log_level,zend_tair_globals,tair_globals)
PHP_INI_END()

static void php_tair_init_globals(zend_tair_globals *tair_globals)
{
	tair_set_loglevel(tair_globals->log_level);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(tair)
{
	REGISTER_INI_ENTRIES();
	tair_set_loglevel(tair_globals.log_level);
	le_tair = zend_register_list_destructors_ex(NULL,tair_dtor,"Tair session", module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(tair)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(tair)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "tair support", "enabled");
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
}
/* }}} */

PHP_FUNCTION(tair_init){
    char * master_addr, * slave_addr, * group_name;
    int master_len ,slave_len , group_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss"
	        , &master_addr, &master_len
	        , &slave_addr, &slave_len
	        , &group_name, &group_len
	        ) == FAILURE) {
		return;
	}
    
    int rs_id = tair_pconnect(master_addr,slave_addr,group_name);
    if(rs_id>0){
        RETURN_RESOURCE(rs_id);    
    }else{
        RETURN_BOOL(0);
    }
}

PHP_FUNCTION(tair_get) {

	zval *r;
	tair_handler tsession;
	tair_data_pair key_data,value_data;
	value_data.len=0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &r, &key_data.data, &key_data.len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(tsession, tair_handler, &r, -1, "Tair session", le_tair);

	last_rst = tair_get(tsession, PHP_TAIR_AREA, &key_data, &value_data);
	if(last_rst ==TAIR_RETURN_SUCCESS){
    	RETURN_STRINGL(value_data.data,value_data.len,1);    
	}else{
        RETURN_BOOL(0);
	}
}

PHP_FUNCTION(tair_put) {

	zval *r;
	tair_handler tsession;
	tair_data_pair key_data,value_data;
	int arg_expire=0;
	int arg_version=0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss|ll",
				&r,
				&key_data.data, &key_data.len,
				&value_data.data, &value_data.len,
				&arg_expire,
				&arg_version
				) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(tsession, tair_handler, &r, -1, "Tair session", le_tair);

	last_rst = tair_put(tsession, PHP_TAIR_AREA, &key_data, &value_data,arg_expire, arg_version );
	RETURN_BOOL(last_rst==TAIR_RETURN_SUCCESS ? 1 : 0);
}

/*
   int tair_remove(tair_handler handler, int area, tair_data_pair* key);
   int tair_decr(tair_handler handler, int area, tair_data_pair* key, int count, int* ret_count, int init_value, int expire);
 */

PHP_FUNCTION(tair_remove) { 
	zval *r;
	tair_handler tsession;
	tair_data_pair key_data;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",&r, &key_data.data, &key_data.len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(tsession, tair_handler, &r, -1, "Tair session", le_tair);

	last_rst = tair_remove(tsession, PHP_TAIR_AREA, &key_data);
	RETURN_BOOL(last_rst==TAIR_RETURN_SUCCESS ? 1 : 0);
}

//int tair_incr(tair_handler handler, int area, tair_data_pair* key, int count, int* ret_count, int init_value, int expire);
PHP_FUNCTION(tair_incr) {
	zval *r;
	tair_handler tsession;
	tair_data_pair key_data;
    int count = 1;
    int init_value=0;
    int expire = 0;
	int ret_count = 0;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|lll"
				,&r
				,&key_data.data, &key_data.len
				,&count
				,&init_value
				,&expire
				) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(tsession, tair_handler, &r, -1, "Tair session", le_tair);

	last_rst = tair_incr(tsession, PHP_TAIR_AREA, &key_data, count, &ret_count, init_value , expire);
	if(last_rst == TAIR_RETURN_SUCCESS){
    	RETURN_LONG(ret_count);    
	}else{
        RETURN_BOOL(0);
	}
}

PHP_FUNCTION(tair_decr) {
	zval *r;
	tair_handler tsession;
	tair_data_pair key_data;
	int ret_count;
    int count = 1;
    int init_value=0;
    int expire = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|lll"
				,&r
				,&key_data.data, &key_data.len
				,&count
				,&init_value
				,&expire
				) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(tsession, tair_handler, &r, -1, "Tair session", le_tair);

	last_rst = tair_decr(tsession, PHP_TAIR_AREA, &key_data, count, &ret_count, init_value , expire);
	if(last_rst == TAIR_RETURN_SUCCESS){
    	RETURN_LONG(ret_count);    
	}else{
        RETURN_BOOL(0);
	}
}

#include "message.c"

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
