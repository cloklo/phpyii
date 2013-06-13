PHP_ARG_ENABLE(yii, whether to enable yii support,
[  --enable-yii           Enable yii support])

AC_ARG_ENABLE(yii-debug,
[  --enable-yii-debug     Enable yii debug mode default=no],
[PHP_YII_DEBUG=$enableval],
[PHP_YII_DEBUG="no"])  

if test "$PHP_YII" != "no"; then

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
    AC_MSG_ERROR([You need at least PHP 5.2.0 to be able to use this version of Yii. PHP $php_version found])
  else
    AC_MSG_RESULT([$php_version, ok])
  fi
  PHP_NEW_EXTENSION(yii, yii.c, $ext_shared)
fi
