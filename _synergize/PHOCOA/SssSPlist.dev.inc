<?php
/* * * *
 * * SssSPlist.dev.inc
 * *
 * *
 * *
 * * alpha: attempting to merge with WFObject or at least implement WFKeyValueCoding
 * *
 * * @version 20100508_213345 (CC) Luke JZ aka SwissalpS
 * * * */

class SssSXHTML extends WFObject {} // SssSXHTML

class SssSXtend extends SssSXHTML {} // SssSXtend

class SssSPlist extends SssS_Plist implements WFKeyValueCoding {

	function __construct() {
		parent::__construct();
	} // __construct

	function SssSPlist($m_plist = '', $s_charset = '',
			$b_outPlistKeyOnOwnLine = false,
			$b_identifyDataAndDatesInOutputArray = true,
			$bPathCheckCreate = false) {
		$this->SssS_Plist($m_plist, $s_charset,
			$b_outPlistKeyOnOwnLine,
			$b_identifyDataAndDatesInOutputArray,
			$bPathCheckCreate);
	} // SssSPlist


	/**
     * Get the value for the given key.
     *
     * First tries to use a getter method (get<key>) then attempts access on class member.
     *
     * @param string The key to retrive the value for.
     * @return mixed The value for the given key.
     */
    function valueForKey($key) {
        if ($key == NULL) throw( new WFUndefinedKeyException("NULL key Exception") );

        $performed = false;

        // try calling getter with naming convention "key"
        if (method_exists($this, $key))
        {
            $result = $this->$key();
            $performed = true;
        }

        // try calling getter with naming convention "getKey"
        if (!$performed)
        {
            $getterMethod = 'get' . ucfirst($key);
            if (method_exists($this, $getterMethod))
            {
                $result = $this->$getterMethod();
                $performed = true;
            }
        }

        // try accessing property directly
        if (!$performed)
        {
            $vars = get_object_vars($this);
            if (array_key_exists($key, $vars))
            {
                $result = $this->$key;
                $performed = true;
            }
        }

        if (!$performed)
        {
            $result = $this->valueForUndefinedKey($key);
        }

        return $result;
    } // valueForKey

    /**
     * Set the value for the given key.
     *
     * First tries to use a setter method (set<key>) then attempts access on class member.
     *
     * @param mixed The value to set.
     * @param string The key to set the value for.
     */
    function setValueForKey($value, $key) {
        $performed = false;

        // try calling setter
        $setMethod = "set" . ucfirst($key);
        if (method_exists($this, $setMethod))
        {
            $this->$setMethod($value);
            $performed = true;
        }

        if (!$performed)
        {
            // try accesing instance var directly
            $vars = get_object_vars($this);
            if (array_key_exists($key, $vars))
            {
                $this->$key = $value;
                $performed = true;
            }
        }

        if (!$performed)
        {
            throw( new WFUndefinedKeyException("Unknown key '$key' requested for object '" . get_class($this) . "'.") );
        }
    } // setValueForKey

    /**
     * Get the value for the given key path.
     *
     * valueForKeypath's default implementation is in {@link WFObject}.
     *
     * The default implementation does some very special things...
     * 1. Magic Arrays<br>
     *    Magic arrays are a way of returning an array of values calculated from an array of objects.
     *    For instance, let's say you have an array of Person objects, for instance in an addressBook application, and that each person has a unique ID.
     *    Now, you want an array containing the ID of every person, but you don't want to have to write a foreach loop to do it.
     *    You could use valueForKeypath's magic instead:
     *    <code>
     *    $arrayOfPersonIDs = $addressBook->valueForKeyPath("people.id")
     *    </code>
     *    And afterwards, arrayOfPersonIDs will have an array containing the ID for each person in the address book, in the same order that the Person objects appear in the array.
     * 2. Array Operators, based on: http://developer.apple.com/documentation/Cocoa/Conceptual/KeyValueCoding/Concepts/ArrayOperators.html
     *
     *    In a given keyPath, you can include an operator to perform a calculation on the keyPath to that point, provided that the result is an array: "transactions.@sum.amount"<br>
     *    Operators are preceeded by @:<br>
     *    count - Count of items specified by remainder of keypath.<br>
     *    first - Return the first item in the array, or NULL<BR>
     *    sum - Sum of items specified by remainder of keypath.<br>
     *    max - Maximum value of items specified by remainder of keypath.<br>
     *    min - Minimum value of items specified by remainder of keypath.<br>
     *    avg - Average of items specified by remainder of keypath.<br>
     *    unionOfArrays - Union of all objects in the arrays specified by remainder of keypath.<br>
     *    unionOfObjects - Union of all items specified by remainder of keypath. Identical to normal magic, ie: books.author == books.@unionOfObjects.author<br>
     *    distinctUnionOfArrays - same as @unionOfArrays but with duplicate objects removed. Duplicates determined by PHP === operator.<br>
     *    distinctUnionOfObjects - same as @unionOfObjects but with duplicate objects removed. Duplicates determined by PHP === operator.<br>
     *
     * 3. Static Method/Property access<br>
     *    If the first part of the keypath contains '::', then "static" mode will be enabled, which allows you to use KVC on an instance of an object to access static methods.
     *    Note that at present all static access must be done with {@link valueForStaticKeyPath()}. This may become more flexible in the future.
     *
     * @see valueForKey()
     * @param string The keyPath to retrive the value for.
     * @return mixed The value for the given keyPath.
     */
    // TODO: this one takes adoption as it overloads and overlaps with
    // SssS_Plist
    function valueForKeyPath($keyPath) {
        return self::valueForTargetAndKeyPath($keyPath, $this);
    } // valueForKeyPath

    /**
     * Creates an associative array with the set of passed keys and the corresponding values
     *
     * @param array An array of keys.
     * @return array An associative array of the passed in keys, now with values from valueForKey($theKey).
     * @throws object WFUndefinedKeyException
     */
    function valuesForKeys($keys) {
        $hash = array();
        foreach ($keys as $k) {
            $v = $this->valueForKey($k);
            $hash[$k] = $v;
        }
        return $hash;
    } // valuesForKeys

    /**
     * Set values for multiple keys.
     *
     * @param array An array of key => value
     * @throws object WFUndefinedKeyException
     */
    function setValuesForKeys($valuesForKeys) {
        foreach ($valuesForKeys as $k => $v) {
            $this->setValueForKey($v, $k);
        }
    } // setValuesForKeys

    /**
     * Creates an associative array with the set of passed keys and keyPaths and the corresponding values.
     *
     * @param array An array of key => keyPath. If a "key" is encountered without a value, uses the "key" as the "keyPath".
     * @return array An associative array of the passed in keys, now with values from valueForKeyPath($theKeyPath).
     * @throws object WFUndefinedKeyException
     */
    function valuesForKeyPaths($keysAndKeyPaths) {
        $hash = array();
        // fix integer keys into keys... this allows keysAndKeyPaths to return ('myProp', 'myProp2' => 'myKeyPath', 'myProp3')
        foreach ( array_keys($keysAndKeyPaths) as $k ) {
            if (gettype($k) == 'integer')
            {
                $keysAndKeyPaths[$keysAndKeyPaths[$k]] = $keysAndKeyPaths[$k];
                unset($keysAndKeyPaths[$k]);
            }
        }
        foreach ($keysAndKeyPaths as $k => $keyPath) {
            $v = $this->valueForKeyPath($keyPath);
            $hash[$k] = $v;
        }
        return $hash;
    } // valuesForKeyPaths

    /**
     * Set the value for the given keyPath.
     *
     * @see setValueForKey()
     * @param mixed The value to set.
     * @param string The keyPath to set the value for.
     */
	// to integrate WFKeyValueCoding this would need to look at keypath arrays and strings with '/' are for fArrayizeKeyPath
	// strings with '.' are for WFKeyValueCoding
	// just hope that neither uses the others separator in keys
	// and hope that if path is only one item long that plist apps prepend /
	// the integration requires a new strain to keep this one ... if base class is to be WFObject
    function setValueForKeyPath($mObject, $mKeyPath = '', $iStartID = -1) { //($value, $keyPath) {
    	if (is_array($mKeyPath))
    		return parent::setValueForKeyPath($mObject, $mKeyPath, $iStartID);

    	$bHasDash = (!(false === strstr($mKeyPath, '/')));
    	$bHasDot = (!(false === strstr($mKeyPath, '.')));
    	$sAskWho = '';
    	if ($bHasDash && (!$bHasDot)) {
    		$sAskWho = 'parent';
    		return parent::setValueForKeyPath($mObject, $mKeyPath, $iStartID);
    	} elseif ($bHasDot && (!$bHasDash))
    		$sAskWho = 'self';
    	else {
    		// could have either both seperators or none -> one word
    		if (empty($mKeyPath)) {
    			$sAskWho = (is_array($mObject))
    				? 'parent' : 'self';
    		} //

    		try {
    			$this->keyPathToTargetAndKey($keyPath);
    			// will throw if can't find
    			$sAskWho = 'self';
    		} catch (Exception $e) {
    			// kvc didn't find key ->
    			$sAskWho = 'parent';
    		} // try catch

    	} // who to ask, kvc or plist

    	if ('parent' == $sAskWho)
    		return parent::setValueForKeyPath($mObject, $mKeyPath, $iStartID);

        list($target, $targetKey) = $this->keyPathToTargetAndKey($keyPath);
        $target->setValueForKey($value, $targetKey);
    } // setValueForKeyPath

    /**
     * Validate the given value for the given keypath.
     *
     * The default implementation (in WFObject) finds the target object based on the keypath and then calls the validateValueForKey method.
     *
     * @see validateValueForKey()
     * @param mixed A reference to value to check. Passed by reference so that the implementation can normalize the data.
     * @param string keyPath the keyPath for the value.
     * @param boolean A reference to a boolean. This value will always be FALSE when the method is called. If the implementation edits the $value, set to TRUE.
     * @param array An array of WFError objects describing the error.
     * @return boolean TRUE indicates a valid value, FALSE indicates an error.
     */
    function validateValueForKeyPath(&$value, $keyPath, &$edited, &$errors) {
        list($target, $targetKey) = $this->keyPathToTargetAndKey($keyPath);

        if (!($target instanceof WFObject))
        {
            throw( new WFException("Target not an object at keypath: " . $keyPath . " for object " . get_class($this)) );
        }
        return $target->validateValueForKey($value, $targetKey, $edited, $errors);
    } // validateValueForKeyPath

    /**
     * Validate the given value for the given key.
     *
     * Clients can normalize the value, and also report and error if the value is not valid.
     *
     * If the value is valid without modificiation, return TRUE and do not alter $edited or $error.
     * If the value is valid after being modified, return TRUE, and $edited to true.
     * IF the value is not valid, do not alter $value or $edited, but fill out the $error object with desired information.
     *
     * The default implementation (in WFObject) looks for a method named validate<key> and calls it, otherwise it returns TRUE.
     *
     * Classes that wish to provide validators for keys should implement one validator per key, with the following prototype:
     *
     *      <code>
     *      // parameters are the same as for the validateValueForKey function, minus the key name.
     *      // function should return TRUE if valid, FALSE if not valid.
     *      validate<KeyName>(&$value, &$edited, &$errors);
     *
     *      // clients wishing to add errors do so with:
     *      $errors[] = new WFError('My error message'); // could also add an error code (string) parameter.
     *      </code>
     *
     * @param mixed A reference to value to check. Passed by reference so that the implementation can normalize the data.
     * @param string The key for the value to validate.
     * @param boolean A reference to a boolean. This value will always be FALSE when the method is called. If the implementation edits the $value, set to TRUE.
     * @param array An array of WFError objects describing the error. The array is empty by default; you can add new error entries.
     * @return boolean TRUE indicates a valid value, FALSE indicates an error.
     */
    function validateValueForKey(&$value, $key, &$edited, &$errors) {
        $valid = true;

        // try calling validator
        $validatorMethod = 'validate' . ucfirst($key);
        if (method_exists($this, $validatorMethod))
        {
            // track whether or not validator lies
            $errCount = count($errors);

            // run validator
            $valid = $this->$validatorMethod($value, $edited, $errors);

            // check for mismatch b/w $valid and errors generated
            $errCount = count($errors) - $errCount;
            if ($valid && $errCount) throw( new WFException("Validator for key '{$key}' returned TRUE but also returned errors.") );
            else if (!$valid && $errCount === 0) throw( new WFException("Validator for key '{$key}' returned FALSE but didn't provide any errors.") );
        }

        if (!$valid)
        {
            WFLog::log("Errors: " . print_r($errors, true), WFLog::TRACE_LOG);
        }

        return $valid;
    } // validateValueForKey

    /**
     * Validate the value, calling the setter if the value is valid.
     *
     * @see validateValueForKey()
     */
    function validatedSetValueForKey(&$value, $key, &$edited, &$errors) {
        if ($this->validateValueForKey($value, $key, $edited, $errors))
        {
            $this->setValueForKey($value, $key);
            return true;
        }
        return false;
    } // validatedSetValueForKey

    /**
     * Validate the value, calling the setter if the value is valid.
     *
     * @see validateValueForKeyPath()
     */
    function validatedSetValueForKeyPath(&$value, $keyPath, &$edited, &$errors) {
        if ($this->validateValueForKeyPath($value, $keyPath, $edited, $errors))
        {
            $this->setValueForKeyPath($value, $keyPath);
            return true;
        }
        return false;
    } // validatedSetValueForKeyPath

    /**
     * Run the object-level validation code.
     *
     * An object-level validator is used for interproperty validation, for instance validating 'postalCode' depends on 'country'.
     *
     * The default implementation will call all defined Key-Value Validators (any method matching "^validate*").
     *
     * Validations are done via {@link validatedSetValueForKey()}, meaning that changes made to values by the validators will be updated via setValueForKey.
     *
     * Subclasses needing to do interproperty validation should override the validateObject() method. If subclasses wish to block the default behavior of re-validating
     * all properties with validators, then the subclass should not call the super method. Subclasses wishing to preserve this behavior should call parent::validateObject($errors).
     *
     * NOTE: It's called validateObject right now instead of validate primarily because Propel already has a validate() method.
     *
     * NOTE: you can pass in an array or an {@link WFErrorArray} object. The latter provides convenience methods that make accessing individual errors easier.
     * See {@link WFErrorArray} for structure of errors array.
     *
     * @experimental
     * @param array An array, passed by reference, which will be populated with any errors encountered. Errors are grouped by key, ie $errors['key'] = array()
     * @return boolean TRUE if valid; FALSE if not.
     * @throws object WFExecption
     */
    function validateObject(&$errors) {
        if ($errors === null)
        {
            $errors = new WFErrorArray();
        }

        $allMethods = get_class_methods(get_class($this));
        foreach ($allMethods as $f) {
            if (strncasecmp('validate', $f, 8) === 0)
            {
                // now, make sure it's a KVV method by reflecting the args; should be 3 args.
                $methodInfo = new ReflectionMethod(get_class($this), $f);
                if ($methodInfo->getNumberOfParameters() !== 3) continue;
                $p = $methodInfo->getParameters();
                if (!($p[0]->isPassedByReference() and $p[1]->isPassedByReference() and $p[2]->isPassedByReference())) continue;

                // we found a real validator! now, validate the value.
                $key = strtolower(substr($f, 8, 1)) . substr($f, 9);
                $keyErrors = array();
                $val = $this->valueForKey($key);
                $ok = $this->validatedSetValueForKey($val, $key, $edited, $keyErrors);
                if (!$ok)
                {
                    $errors[$key] = $keyErrors;
                }
            }
        }

        return count($errors) == 0;
    } // validateObject

    /**
     * Called by valueForKey() if the key cannot be located through normal methods.
     *
     * The default implementation raises as WFUndefinedKeyException. Subclasses can override this function to return an alternate value for the undefined key.
     * @param string The key.
     * @return mixed The value of the key.
     * @throws object WFUndefinedKeyException
     */
    function valueForUndefinedKey($key) {
        throw( new WFUndefinedKeyException("Unknown key '$key' requested for object '" . get_class($this) . "'.") );
    } // valueForUndefinedKey

    /**
     * Called by valueForStaticKey() if the key cannot be located through normal methods.
     *
     * The default implementation raises as WFUndefinedKeyException. Subclasses can override this function to return an alternate value for the undefined key.
     *
     * NOTE: subclass overrides probably won't work so well until PHP 5.3.
     *
     * @param string The key.
     * @return mixed The value of the key.
     * @throws object WFUndefinedKeyException
     */
    static function valueForUndefinedStaticKey($key) {
        throw( new WFUndefinedKeyException("Unknown key '$key' requested for object '" . __CLASS__ . "'.") );
    } // valueForUndefinedStaticKey

    /**
     * Get the value of a static keypath. A static keypath is a key called against a CLASS rather than an instance.
     *
     * @param string The key (method/property name). Key must be in form "ClassName::MethodName".
     * @param string The class name to which the key belongs
     * @return mixed The value of the key.
     * @throws object WFUndefinedKeyException
     */
    static function valueForStaticKey($key) {
        return self::valueForStaticKeyPath($key);
    } // valueForStaticKey

    /**
     * This function is a wrapper of valueForKeyPath that allows you to use KVC to access static methods/properties without having an instance to a class.
     *
     * MyObject::valueForStaticKeyPath('MyObject::myStaticMethod')
     *
     * @see valueForKey()
     * @param string The keyPath to retrive the value for.
     * @return mixed The value for the given keyPath.
     * @throws object WFUndefinedKeyException
     */
    static function valueForStaticKeyPath($keyPath) {
        return self::valueForTargetAndKeyPath($keyPath);
    } // valueForStaticKeyPath

/* WFKeyValueCoding interface methods END here */

/* these are used by above and belong into WFKeyValueCoding interface
as they declare objects unfit if they don't implement these, sorry unfit WFObjects - so this bit is to pass as WFObject */

 protected static function _valueForStaticKey($key, $target)
    {
        if ($key == NULL) throw( new WFUndefinedKeyException("NULL key Exception") );

        $performed = false;

        // try calling getter with naming convention "key"
        if (method_exists($target, $key))
        {
            $result = call_user_func(array($target, $key));
            $performed = true;
        }

        // try calling getter with naming convention "getKey"
        if (!$performed)
        {
            $getterMethod = 'get' . ucfirst($key);
            if (method_exists($target, $getterMethod))
            {
                $result = call_user_func(array($target, $getterMethod));
                $performed = true;
            }
        }

        // try accessing property directly
        if (!$performed)
        {
            $vars = get_class_vars($target);
            if (array_key_exists($key, $vars))
            {
                throw( new WFException("No way to support this before PHP 5.3.") );
                //$result = $target::$$key;
                $performed = true;
            }
        }

        if (!$performed)
        {
            $result = self::valueForUndefinedStaticKey($key);
        }

        return $result;
    } // _valueForStaticKey


    /**
     * Helper function for implementing KVC.
     *
     * This is public so that other objects that don't subclass WFObject can leverage this codebase to implement KVC.
     *
     * @param string The keyPath.
     * @param object Generic The root object to run the keyPath search against.
     * @return mixed
     * @throws Exception, WFUndefinedKeyException
     */
    public static function valueForTargetAndKeyPath($keyPath, $rootObject = NULL) {
        if ($keyPath == NULL) throw( new Exception("NULL keyPath Exception") );
        $staticMode = ($rootObject === NULL);

        // initialize
        $result = NULL;

        $keyParts = explode('.', $keyPath);
        $keyPartCount = count($keyParts);
        $modelKeyPath = NULL;
        if ($keyPartCount > 0)
        {
            $modelKeyPath = join('.', array_slice($keyParts, 1));
        }


        // walk keypath
        $keys = explode('.', $keyPath);
        $keyPartsLeft = $keyCount = count($keys);
        $arrayMode = false;
        for ($keyI = 0; $keyI < $keyCount; $keyI++) {
            $key = $keys[$keyI];
            $keyPartsLeft--;

            // determine target; use this if on first key, use result otherwise
            if ($keyI == 0)
            {
                // having "::" in your first key or a rootObject of NULL triggers STATIC mode
                if ($staticMode && strpos($key, '::') !== false)
                {
                    $staticParts = explode('::', $key);
                    if (count($staticParts) !== 2)
                    {
                        throw( new WFException("First part of keypath for static KVC must be 'ClassName::StaticMethodName'; you passed: " . $key) );
                    }
                    $target = $staticParts[0];
                    $key = $staticParts[1];
                    if (!class_exists($target)) throw( new WFException("First part of a static keypath must be a valid class name, you passed: " . $target) );
                }
                else
                {
                    $target = $rootObject;
                }
            }
            else
            {
                $target = $result;
            }

            // get result of this part of path
            if ($staticMode && $keyI == 0)
            {
                if (!is_string($target)) throw( new WFException('Target is not class name at static keyPath: ' . join('.', array_slice($keys, 0, $keyI))) );
                $staticF = array($target, '_valueForStaticKey');
                if (!is_callable($staticF)) throw( new WFException('Target class (' . $target . ') does not implement WFObject protocol.') );
                $result = call_user_func($staticF, $key, $target);
            }
            else
            {
                if (!is_object($target)) throw( new WFUndefinedKeyException('Value at keyPath: "' . join('.', array_slice($keys, 0, $keyI)) . "\" is not an object when trying to get next key \"{$key}\".") );
                $result = $target->valueForKey($key);
            }

            if (is_array($result) or ($result instanceof ArrayObject))
            {
                $arrayMode = true;
            }

            // IF the result of the a key is an array, we do some magic.
            // We CREATE a new array with the results of calling EACH object in the array with the rest of the path.
            // We also support several operators: http://developer.apple.com/documentation/Cocoa/Conceptual/KeyValueCoding/Concepts/ArrayOperators.html
            if ($arrayMode and $keyPartsLeft)
            {
                $nextPart = $keys[$keyI + 1];
                // are we in operator mode as well?
                if (in_array($nextPart, array('@count', '@first', '@sum', '@max', '@min', '@avg', '@unionOfArrays', '@unionOfObjects', '@distinctUnionOfArrays', '@distinctUnionOfObjects')))
                {
                    $operator = $nextPart;
                    $rightKeyPath = join('.', array_slice($keyParts, $keyI + 2));
                }
                else
                {
                    $operator = NULL;
                    $rightKeyPath = join('.', array_slice($keyParts, $keyI + 1));
                }
                //print "magic on $keyPath at " . join('.', array_slice($keyParts, 0, $keyI + 1)) . " kp: $rightKeyPath\n";

                // if there is a rightKeyPath, need to calculate magic array from remaining keypath. Otherwise, just use current result (it's arrayMode) as magicArray.
                if ($rightKeyPath)
                {
                    $magicArray = array();
                    foreach ($result as $object) {
                        if (!is_object($object)) throw( new Exception("All array items must be OBJECTS THAT IMPLEMENT Key-Value Coding for KVC Magic Arrays to work.") );
                        if (!method_exists($object, 'valueForKey')) throw( new Exception("target is not Key-Value Coding compliant for valueForKey.") );
                        $magicArray[] = $object->valueForKeyPath($rightKeyPath);
                    }
                }
                else
                {
                    $magicArray = $result;
                }

                if ($operator)
                {
                    switch ($operator) {
                        case '@count':
                            $result = count($magicArray);
                            break;
                        case '@first':
                            if (count($magicArray) > 0)
                            {
                                $result = $magicArray[0];
                            }
                            else
                            {
                                $result = null;
                            }
                            break;
                        case '@sum':
                            $result = array_sum ( $magicArray );
                            break;
                        case '@max':
                            $result = max ( $magicArray );
                            break;
                        case '@min':
                            $result = min ( $magicArray );
                            break;
                        case '@avg':
                            $result = array_sum ( $magicArray ) / count($magicArray);
                            break;
                        case '@unionOfArrays':
                            $result = array();
                            foreach ($magicArray as $item) {
                                if (!is_array($item))
                                {
                                    throw( new Exception("unionOfArrays requires that all results be arrays... non-array encountered: $item") );
                                }
                                $result = array_merge($item, $result);
                            }
                            break;
                        // I think this is equivalent to what our magic arrays do anyway
                        // for instance: transactions.payee will give a list of all payee objects of each transaction
                        // it would seem: transactions.@unionOfObjects.payee would yield the same?
                        case '@unionOfObjects':
                            $result = $magicArray;
                            break;
                        case '@distinctUnionOfArrays':
                            $result = array();
                            foreach ($magicArray as $item) {
                                if (!is_array($item))
                                {
                                    throw( new Exception("distinctUnionOfArrays requires that all results be arrays... non-array encountered: $item") );
                                }
                                $result = array_merge($item, $result);
                            }
                            $result = array_unique($result);
                            break;
                        case '@distinctUnionOfObjects':
                            $result = array_unique($magicArray);
                            break;
                    }
                }
                else
                {
                    $result = $magicArray;
                }
                break;
            }
        }

        return $result;
    } // valueForTargetAndKeyPath


	 /**
     * Helper function to convert a keyPath into the targetKeyPath (the object to call xxxKey on) and the targetKey (the key to call on the target object).
     *
     * Usage:
     *
     * <code>
     * list($targetKeyPath, $targetKey) = keyPathToParts($keyPath);
     * </code>
     *
     * @return array targetKeyPath, targetKey.
     */
    protected function keyPathToTargetAndKey($keyPath) {
        if ($keyPath == NULL) throw( new Exception("NULL key Exception") );

        // walk keypath
        // If the keypath is a.b.c.d then the targetKeyPath is a.b.c and the targetKey is d
        $keyParts = explode('.', $keyPath);
        $keyPartCount = count($keyParts);
        if ($keyPartCount == 0) throw( new Exception("Illegal keyPath: '{$keyPath}'. KeyPath must have at least one part.") );

        if ($keyPartCount == 1)
        {
            $targetKey = $keyPath;
            $target = $this;
        }
        else
        {
            $targetKey = $keyParts[$keyPartCount - 1];
            $targetKeyPath = join('.', array_slice($keyParts, 0, $keyPartCount - 1));
            $target = $this->valueForKeyPath($targetKeyPath);
        }

        return array($target, $targetKey);
    } // keyPathToTargetAndKey


    /**
     *  Empty placeholder for exposedProperties setup.
     *
     *  Subclasses should call parent and merge results.
     *
     *  @return array
     */
    public static function exposedProperties() {
        return array();
    } // exposedProperties
} // SssSPlist

/* * * *\ SssSPlist.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
