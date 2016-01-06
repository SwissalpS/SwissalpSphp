<?php
/* * * *
 * * SwissalpS/Strings/Encoding/Utf8Loader.inc
 * * wrapper class to help load the functions
 * *    'utf8ToUnicode(&$str)'
 * *    'unicodeToUtf8(&$arr)'
 * *
 * * @version 20160106_224146 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\Strings\Encoding;

class Utf8Loader {

    final public static function load() {

        if (function_exists('utf8ToUnicode')) {

            return true;

        } // if already loaded

        require_once(dirname(dirname(dirname(__FILE__))) . DIRECTORY_SEPARATOR . '_external' . DIRECTORY_SEPARATOR . 'php_utf8' . DIRECTORY_SEPARATOR . 'utf8.inc');

        return true;

    } // load

} // Utf8Loader

/* * * *\ SwissalpS/Strings/Encoding/Utf8Loader.inc (CC) Luke JZ aka SwissalpS /* * * */
