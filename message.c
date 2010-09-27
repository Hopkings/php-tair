
PHP_FUNCTION(tair_last_msg){
    char * msgstr;
    switch(last_rst){
        case TAIR_RETURN_SUCCESS:
            msgstr = "TAIR_RETURN_SUCCESS";
        break;

        case TAIR_RETURN_PROXYED:
            msgstr = "TAIR_RETURN_PROXYED";
        break;

        case TAIR_RETURN_FAILED:
            msgstr = "TAIR_RETURN_FAILED";
        break;

        case TAIR_RETURN_DATA_NOT_EXIST:
            msgstr = "TAIR_RETURN_DATA_NOT_EXIST";
        break;

        case TAIR_RETURN_VERSION_ERROR:
            msgstr = "TAIR_RETURN_VERSION_ERROR";
        break;

        case TAIR_RETURN_TYPE_NOT_MATCH:
            msgstr = "TAIR_RETURN_TYPE_NOT_MATCH";
        break;

        case TAIR_RETURN_PLUGIN_ERROR:
            msgstr = "TAIR_RETURN_PLUGIN_ERROR";
        break;

        case TAIR_RETURN_SERIALIZE_ERROR:
            msgstr = "TAIR_RETURN_SERIALIZE_ERROR";
        break;

        case TAIR_RETURN_ITEM_EMPTY:
            msgstr = "TAIR_RETURN_ITEM_EMPTY";
        break;

        case TAIR_RETURN_OUT_OF_RANGE:
            msgstr = "TAIR_RETURN_OUT_OF_RANGE";
        break;

        case TAIR_RETURN_ITEMSIZE_ERROR:
            msgstr = "TAIR_RETURN_ITEMSIZE_ERROR";
        break;

        case TAIR_RETURN_SEND_FAILED:
            msgstr = "TAIR_RETURN_SEND_FAILED";
        break;

        case TAIR_RETURN_TIMEOUT:
            msgstr = "TAIR_RETURN_TIMEOUT";
        break;

        case TAIR_RETURN_DATA_EXPIRED:
            msgstr = "TAIR_RETURN_DATA_EXPIRED";
        break;

        case TAIR_RETURN_SERVER_CAN_NOT_WORK:
            msgstr = "TAIR_RETURN_SERVER_CAN_NOT_WORK";
        break;

        case TAIR_RETURN_WRITE_NOT_ON_MASTER:
            msgstr = "TAIR_RETURN_WRITE_NOT_ON_MASTER";
        break;

        case TAIR_RETURN_DUPLICATE_BUSY:
            msgstr = "TAIR_RETURN_DUPLICATE_BUSY";
        break;

        case TAIR_RETURN_MIGRATE_BUSY:
            msgstr = "TAIR_RETURN_MIGRATE_BUSY";
        break;

        case TAIR_RETURN_PARTIAL_SUCCESS:
            msgstr = "TAIR_RETURN_PARTIAL_SUCCESS";
        break;

        case TAIR_RETURN_INVALID_ARGUMENT:
            msgstr = "TAIR_RETURN_INVALID_ARGUMENT";
        break;

        case TAIR_RETURN_CANNOT_OVERRIDE:
            msgstr = "TAIR_RETURN_CANNOT_OVERRIDE";
        break;
    }

array_init(return_value);
add_index_long(return_value,0,last_rst);
add_index_string(return_value,1,msgstr,1);
    
}