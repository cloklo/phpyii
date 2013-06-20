PHP_ARG_ENABLE(yii, whether to enable Yii Framework support,
[  --enable-yii           Enable Yii Framework support])

AC_ARG_ENABLE(yii-gii,
[  --enable-yii-gii     Enable Gii tool of Yii Framework default=no],
[PHP_YII_GII=$enableval],
[PHP_YII_GII="no"])  

AC_ARG_ENABLE(yii-debug,
[  --enable-yii-debug     Enable Yii Framework debug mode default=no],
[PHP_YII_DEBUG=$enableval],
[PHP_YII_DEBUG="no"])  

if test "$PHP_YII" != "no"; then

  if test "$PHP_YII_GII" = "yes"; then
    AC_DEFINE(PHP_YII_GII,1,[define to 1 if you want to enable Gii tool of Yii Framework])
  else
    AC_DEFINE(PHP_YII_GII,0,[define to 1 if you want to enable Gii tool of Yii Framework])
  fi

  if test "$PHP_YII_DEBUG" = "yes"; then
    AC_DEFINE(PHP_YII_DEBUG,1,[define to 1 if you want to change the POST/GET by php script])
  else
    AC_DEFINE(PHP_YII_DEBUG,0,[define to 1 if you want to change the POST/GET by php script])
  fi

  AC_MSG_CHECKING([PHP version])

  tmp_version=$PHP_VERSION
  if test -z "$tmp_version"; then
    if test -z "$PHP_CONFIG"; then
      AC_MSG_ERROR([php-config not found])
    fi
    php_version=`$PHP_CONFIG --version 2>/dev/null|head -n 1|sed -e 's#\([0-9]\.[0-9]*\.[0-9]*\)\(.*\)#\1#'`
  else
    php_version=`echo "$tmp_version"|sed -e 's#\([0-9]\.[0-9]*\.[0-9]*\)\(.*\)#\1#'`
  fi

  if test -z "$php_version"; then
    AC_MSG_ERROR([failed to detect PHP version, please report])
  fi

  ac_IFS=$IFS
  IFS="."
  set $php_version
  IFS=$ac_IFS
  yii_php_version=`expr [$]1 \* 1000000 + [$]2 \* 1000 + [$]3`

  if test "$yii_php_version" -le "5002000"; then
    AC_MSG_ERROR([You need at least PHP 5.2.0 to be able to use this version of Yii Framework. PHP $php_version found])
  else
    AC_MSG_RESULT([$php_version, ok])
  fi

  ext_files="yii.c yiibase.c"

  if test "$PHP_YII_GII" = "yes"; then
    ext_files=$ext_files" gii/gii.c"
  fi

  PHP_NEW_EXTENSION(yii, $ext_files, $ext_shared)
fi
