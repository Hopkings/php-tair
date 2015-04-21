# tair\_init #
初始化tair链接, 并返回资源描述符.  **注: 该函数永远是持续链接**

**Description**
> Resource tair\_init(string $master\_addr, string $slave\_addr, string $group\_name)

**Parameters**
  * **master\_addr**  tair config master server
  * **slave\_addr**   tair config master server
  * **group\_name**   group name

**Return Values**
  * Resouce<tair session>

# tair\_get #

**Description**
> mixed tair\_get(string $resource, string $key)

**Parameters**
  * **resource**  tair session resource
  * **key**   key

**Return Values**
  * mixed value

获取不到结果, 会返回false

# tair\_put #

**Description**
```
 mixed tair_put(string $resource, string $key, mixed $value, [ int $expire, [int $version]]) 
```

**Parameters**
  * **resource**  tair session resource
  * **key**   key
  * **value**   value
  * **expire**   expire
  * **version**   version

**Return Values**
  * bollean

# tair\_incr #

**Description**
使指定的key自增,并返回结果
```
 int tair_incr(string $resource, string $key,[ int $step, [ int $init_value, [int $expire]]]) 
```
**Parameters**
  * **resource**  tair session resource
  * **key**   key
  * **step**   step
  * **init\_value**   init\_value
  * **expire**   expire

**Return Values**
  * int result

# tair\_decr #

**Description**
使指定的key自减,并返回结果
```
int tair_incr(string $resource, string $key,[ int $step, [ int $init_value, [int $expire]]]) 
```

**Parameters**
  * **resource**  tair session resource
  * **key**   key
  * **step**   step
  * **init\_value**   init\_value
  * **expire**   expire

**Return Values**
  * int result

# tair\_last\_msg #

**Description**
返回上个tair执行的结果信息
```
 array tair_last_msg() 
```

**Parameters**

**Return Values**
  * array result

# 代码示例 #
```
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
```