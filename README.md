# php-tair
Automatically exported from code.google.com/p/php-tair

Tair is a distributed, high performance key/value storage system.
Here is the client for php.


安装
 pacman -Sy php-tair
 
 AUR: http://aur.archlinux.org/packages.php?ID=41157 
 
 
 
使用方法（http://code.google.com/p/php-tair/wiki/Usage）：

1、tair_init
初始化tair链接, 并返回资源描述符. 注: 该函数永远是持续链接

Description

    Resource tair_init(string $master_addr, string $slave_addr, string $group_name) 

Parameters

    master_addr tair config master server
    slave_addr tair config master server
    group_name group name 

Return Values

    Resouce<tair session> 

2、tair_get

Description

    mixed tair_get(string $resource, string $key) 

Parameters

    resource tair session resource
    key key 

Return Values

    mixed value 

获取不到结果, 会返回false

3、tair_put

Description

 mixed tair_put(string $resource, string $key, mixed $value, [ int $expire, [int $version]]) 

Parameters

    resource tair session resource
    key key
    value value
    expire expire
    version version 

Return Values

    bollean 

4、tair_incr

Description 使指定的key自增,并返回结果

 int tair_incr(string $resource, string $key,[ int $step, [ int $init_value, [int $expire]]]) 

Parameters

    resource tair session resource
    key key
    step step
    init_value init_value
    expire expire 

Return Values

    int result 

5、tair_decr

Description 使指定的key自减,并返回结果

int tair_incr(string $resource, string $key,[ int $step, [ int $init_value, [int $expire]]]) 

Parameters

    resource tair session resource
    key key
    step step
    init_value init_value
    expire expire 

Return Values

    int result 

6、tair_last_msg

Description 返回上个tair执行的结果信息

 array tair_last_msg() 

Parameters

Return Values

    array result 

代码示例

 <?php
 
 $rs = tair_init('10.0.0.210',null,'group_1');
 
 $value = tair_get($rs,'key_not_exists');
 
 if($value*false){
      print_r(tair_last_msg());
 }
 
 ?>

 Array
 (
     [0] => -3998
     [1] => TAIR_RETURN_DATA_NOT_EXIST
 )
