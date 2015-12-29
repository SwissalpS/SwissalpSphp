<?php
/* * * *
 * * SssS_bootstrap.inc
 * * add autoload for SwissalpSphp classes
 * *
 * * @version 20151226_170552 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('DIR_SEP')) {
    define('DIR_SEP', DIRECTORY_SEPARATOR);
} // if dir_sep not defined yet
if (!defined('NL')) {
    define('NL', chr(10));
} // if no NL yet
if (!defined('SwissalpS_FRAMEWORK_DIR')) {
    define('SwissalpS_FRAMEWORK_DIR', dirname(__FILE__));
} // if not defined where SwissalpSphp is
// phocoa framework dir
if (!defined('FRAMEWORK_DIR')) {
    if (defined('PATH_DIR_SRC_PHOCOA')) {
        define('FRAMEWORK_DIR', PATH_DIR_SRC_PHOCOA);
    } else {
        define('FRAMEWORK_DIR', dirname(dirname(__FILE__)) . DIR_SEP . 'phocoa' . DIR_SEP . 'phocoa');
    } // if got other define with same meaning
} // if no phocoa defined
if (!defined('PROPEL_FRAMEWORK_DIR')) {
    define('PROPEL_FRAMEWORK_DIR', dirname(dirname(__FILE__)) . DIR_SEP . 'Propel2' . DIR_SEP . 'src');
} // if no propel lib (for pdo)
if (!defined('HORDE_FRAMEWORK_DIR')) {
    define('HORDE_FRAMEWORK_DIR', SwissalpS_FRAMEWORK_DIR . DIR_SEP . '_synergize' . DIR_SEP . 'PHOCOA' . DIR_SEP . 'Horde');
} // if no horde lib (for yaml)
if (!defined('PEAR_LOG_FRAMEWORK_DIR')) {
    define('PEAR_LOG_FRAMEWORK_DIR', SwissalpS_FRAMEWORK_DIR . DIR_SEP . '_synergize' . DIR_SEP . 'PHOCOA');
} // if no  lib (for log)
if (!defined('SMARTY_DIR')) {
    define('SMARTY_DIR', SwissalpS_FRAMEWORK_DIR . DIR_SEP . '_synergize' . DIR_SEP . 'PHOCOA' . DIR_SEP . 'Smarty');
} // if no smarty lib
if (!defined('APP_ROOT')) {
    define('APP_ROOT', $_SERVER['PWD']);
} // if no app-root defined, this is used and checked for subfolder 'classes' (and namespaced subfolders) and ../vendor
$aTmp = array('bOK' => false);
require(SwissalpS_FRAMEWORK_DIR . DIR_SEP . '_synergize' . DIR_SEP . 'PHOCOA' . DIR_SEP . 'util' . DIR_SEP . 'WFIncluding.inc');
$aTmp['bOK'] = spl_autoload_register(array('WFIncluding', 'autoload'));
if (true !== $aTmp['bOK']) {
    throw new Exception("Error registering WFIncluding::autoload()");
} // if failed to register autoload

/* * * *\ SssS_bootstrap (CC) Luke JZ aka SwissalpS /* * * */
?>
