<?php

/**
 * Base class that represents a row from the 'Users' table.
 *
 *
 *
 * @package    skyprom_phocoa.om
 */
abstract class BaseUsers extends BaseObject  implements Persistent {


	/**
	 * The Peer class.
	 * Instance provides a convenient way of calling static methods on a class
	 * that calling code may not be able to identify.
	 * @var        UsersPeer
	 */
	protected static $peer;

	/**
	 * The value for the handle field.
	 * @var        string
	 */
	protected $handle;

	/**
	 * The value for the permissions field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $permissions;

	/**
	 * The value for the passhash field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $passhash;

	/**
	 * The value for the oauth field.
	 * @var        string
	 */
	protected $oauth;

	/**
	 * The value for the openid field.
	 * @var        string
	 */
	protected $openid;

	/**
	 * The value for the email field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $email;

	/**
	 * The value for the realname field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $realname;

	/**
	 * The value for the uid field.
	 * @var        int
	 */
	protected $uid;

	/**
	 * The value for the url field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $url;

	/**
	 * The value for the avatarmediauid field.
	 * @var        int
	 */
	protected $avatarmediauid;

	/**
	 * The value for the languageorder field.
	 * Note: this column has a database default value of: 'de,fr,it,ru,en'
	 * @var        string
	 */
	protected $languageorder;

	/**
	 * The value for the country field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $country;

	/**
	 * The value for the region field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $region;

	/**
	 * The value for the karma field.
	 * Note: this column has a database default value of: 100
	 * @var        int
	 */
	protected $karma;

	/**
	 * Flag to prevent endless save loop, if this object is referenced
	 * by another object which falls in this transaction.
	 * @var        boolean
	 */
	protected $alreadyInSave = false;

	/**
	 * Flag to prevent endless validation loop, if this object is referenced
	 * by another object which falls in this transaction.
	 * @var        boolean
	 */
	protected $alreadyInValidation = false;

	/**
	 * Initializes internal state of BaseUsers object.
	 * @see        applyDefaults()
	 */
	public function __construct()
	{
		parent::__construct();
		$this->applyDefaultValues();
	}

	/**
	 * Applies default values to this object.
	 * This method should be called from the object's constructor (or
	 * equivalent initialization method).
	 * @see        __construct()
	 */
	public function applyDefaultValues()
	{
		$this->permissions = '';
		$this->passhash = '';
		$this->email = '';
		$this->realname = '';
		$this->url = '';
		$this->languageorder = 'de,fr,it,rm,en';
		$this->country = '';
		$this->region = '';
		$this->karma = 100;
	}

	/**
	 * Get the [handle] column value.
	 *
	 * @return     string
	 */
	public function getHandle()
	{
		return $this->handle;
	}

	/**
	 * Get the [permissions] column value.
	 *
	 * @return     string
	 */
	public function getPermissions()
	{
		return $this->permissions;
	}

	/**
	 * Get the [passhash] column value.
	 *
	 * @return     string
	 */
	public function getPasshash()
	{
		return $this->passhash;
	}

	/**
	 * Get the [oauth] column value.
	 *
	 * @return     string
	 */
	public function getOauth()
	{
		return $this->oauth;
	}

	/**
	 * Get the [openid] column value.
	 *
	 * @return     string
	 */
	public function getOpenid()
	{
		return $this->openid;
	}

	/**
	 * Get the [email] column value.
	 *
	 * @return     string
	 */
	public function getEmail()
	{
		return $this->email;
	}

	/**
	 * Get the [realname] column value.
	 *
	 * @return     string
	 */
	public function getRealname()
	{
		return $this->realname;
	}

	/**
	 * Get the [uid] column value.
	 *
	 * @return     int
	 */
	public function getUid()
	{
		return $this->uid;
	}

	/**
	 * Get the [url] column value.
	 *
	 * @return     string
	 */
	public function getUrl()
	{
		return $this->url;
	}

	/**
	 * Get the [avatarmediauid] column value.
	 *
	 * @return     int
	 */
	public function getAvatarmediauid()
	{
		return $this->avatarmediauid;
	}

	/**
	 * Get the [languageorder] column value.
	 *
	 * @return     string
	 */
	public function getLanguageorder()
	{
		return $this->languageorder;
	}

	/**
	 * Get the [country] column value.
	 *
	 * @return     string
	 */
	public function getCountry()
	{
		return $this->country;
	}

	/**
	 * Get the [region] column value.
	 *
	 * @return     string
	 */
	public function getRegion()
	{
		return $this->region;
	}

	/**
	 * Get the [karma] column value.
	 *
	 * @return     int
	 */
	public function getKarma()
	{
		return $this->karma;
	}

	/**
	 * Set the value of [handle] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setHandle($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->handle !== $v) {
			$this->handle = $v;
			$this->modifiedColumns[] = UsersPeer::HANDLE;
		}

		return $this;
	} // setHandle()

	/**
	 * Set the value of [permissions] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setPermissions($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->permissions !== $v || $v === '') {
			$this->permissions = $v;
			$this->modifiedColumns[] = UsersPeer::PERMISSIONS;
		}

		return $this;
	} // setPermissions()

	/**
	 * Set the value of [passhash] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setPasshash($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->passhash !== $v || $v === '') {
			$this->passhash = $v;
			$this->modifiedColumns[] = UsersPeer::PASSHASH;
		}

		return $this;
	} // setPasshash()

	/**
	 * Set the value of [oauth] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setOauth($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->oauth !== $v) {
			$this->oauth = $v;
			$this->modifiedColumns[] = UsersPeer::OAUTH;
		}

		return $this;
	} // setOauth()

	/**
	 * Set the value of [openid] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setOpenid($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->openid !== $v) {
			$this->openid = $v;
			$this->modifiedColumns[] = UsersPeer::OPENID;
		}

		return $this;
	} // setOpenid()

	/**
	 * Set the value of [email] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setEmail($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->email !== $v || $v === '') {
			$this->email = $v;
			$this->modifiedColumns[] = UsersPeer::EMAIL;
		}

		return $this;
	} // setEmail()

	/**
	 * Set the value of [realname] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setRealname($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->realname !== $v || $v === '') {
			$this->realname = $v;
			$this->modifiedColumns[] = UsersPeer::REALNAME;
		}

		return $this;
	} // setRealname()

	/**
	 * Set the value of [uid] column.
	 *
	 * @param      int $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setUid($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->uid !== $v) {
			$this->uid = $v;
			$this->modifiedColumns[] = UsersPeer::UID;
		}

		return $this;
	} // setUid()

	/**
	 * Set the value of [url] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setUrl($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->url !== $v || $v === '') {
			$this->url = $v;
			$this->modifiedColumns[] = UsersPeer::URL;
		}

		return $this;
	} // setUrl()

	/**
	 * Set the value of [avatarmediauid] column.
	 *
	 * @param      int $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setAvatarmediauid($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->avatarmediauid !== $v) {
			$this->avatarmediauid = $v;
			$this->modifiedColumns[] = UsersPeer::AVATARMEDIAUID;
		}

		return $this;
	} // setAvatarmediauid()

	/**
	 * Set the value of [languageorder] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setLanguageorder($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->languageorder !== $v || $v === 'de,fr,it,ru,en') {
			$this->languageorder = $v;
			$this->modifiedColumns[] = UsersPeer::LANGUAGEORDER;
		}

		return $this;
	} // setLanguageorder()

	/**
	 * Set the value of [country] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setCountry($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->country !== $v || $v === '') {
			$this->country = $v;
			$this->modifiedColumns[] = UsersPeer::COUNTRY;
		}

		return $this;
	} // setCountry()

	/**
	 * Set the value of [region] column.
	 *
	 * @param      string $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setRegion($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->region !== $v || $v === '') {
			$this->region = $v;
			$this->modifiedColumns[] = UsersPeer::REGION;
		}

		return $this;
	} // setRegion()

	/**
	 * Set the value of [karma] column.
	 *
	 * @param      int $v new value
	 * @return     Users The current object (for fluent API support)
	 */
	public function setKarma($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->karma !== $v || $v === 100) {
			$this->karma = $v;
			$this->modifiedColumns[] = UsersPeer::KARMA;
		}

		return $this;
	} // setKarma()

	/**
	 * Indicates whether the columns in this object are only set to default values.
	 *
	 * This method can be used in conjunction with isModified() to indicate whether an object is both
	 * modified _and_ has some values set which are non-default.
	 *
	 * @return     boolean Whether the columns in this object are only been set with default values.
	 */
	public function hasOnlyDefaultValues()
	{
			// First, ensure that we don't have any columns that have been modified which aren't default columns.
			if (array_diff($this->modifiedColumns, array(UsersPeer::PERMISSIONS,UsersPeer::PASSHASH,UsersPeer::EMAIL,UsersPeer::REALNAME,UsersPeer::URL,UsersPeer::LANGUAGEORDER,UsersPeer::COUNTRY,UsersPeer::REGION,UsersPeer::KARMA))) {
				return false;
			}

			if ($this->permissions !== '') {
				return false;
			}

			if ($this->passhash !== '') {
				return false;
			}

			if ($this->email !== '') {
				return false;
			}

			if ($this->realname !== '') {
				return false;
			}

			if ($this->url !== '') {
				return false;
			}

			if ($this->languageorder !== 'de,fr,it,ru,en') {
				return false;
			}

			if ($this->country !== '') {
				return false;
			}

			if ($this->region !== '') {
				return false;
			}

			if ($this->karma !== 100) {
				return false;
			}

		// otherwise, everything was equal, so return TRUE
		return true;
	} // hasOnlyDefaultValues()

	/**
	 * Hydrates (populates) the object variables with values from the database resultset.
	 *
	 * An offset (0-based "start column") is specified so that objects can be hydrated
	 * with a subset of the columns in the resultset rows.  This is needed, for example,
	 * for results of JOIN queries where the resultset row includes columns from two or
	 * more tables.
	 *
	 * @param      array $row The row returned by PDOStatement->fetch(PDO::FETCH_NUM)
	 * @param      int $startcol 0-based offset column which indicates which restultset column to start with.
	 * @param      boolean $rehydrate Whether this object is being re-hydrated from the database.
	 * @return     int next starting column
	 * @throws     PropelException  - Any caught Exception will be rewrapped as a PropelException.
	 */
	public function hydrate($row, $startcol = 0, $rehydrate = false)
	{
		try {

			$this->handle = ($row[$startcol + 0] !== null) ? (string) $row[$startcol + 0] : null;
			$this->permissions = ($row[$startcol + 1] !== null) ? (string) $row[$startcol + 1] : null;
			$this->passhash = ($row[$startcol + 2] !== null) ? (string) $row[$startcol + 2] : null;
			$this->oauth = ($row[$startcol + 3] !== null) ? (string) $row[$startcol + 3] : null;
			$this->openid = ($row[$startcol + 4] !== null) ? (string) $row[$startcol + 4] : null;
			$this->email = ($row[$startcol + 5] !== null) ? (string) $row[$startcol + 5] : null;
			$this->realname = ($row[$startcol + 6] !== null) ? (string) $row[$startcol + 6] : null;
			$this->uid = ($row[$startcol + 7] !== null) ? (int) $row[$startcol + 7] : null;
			$this->url = ($row[$startcol + 8] !== null) ? (string) $row[$startcol + 8] : null;
			$this->avatarmediauid = ($row[$startcol + 9] !== null) ? (int) $row[$startcol + 9] : null;
			$this->languageorder = ($row[$startcol + 10] !== null) ? (string) $row[$startcol + 10] : null;
			$this->country = ($row[$startcol + 11] !== null) ? (string) $row[$startcol + 11] : null;
			$this->region = ($row[$startcol + 12] !== null) ? (string) $row[$startcol + 12] : null;
			$this->karma = ($row[$startcol + 13] !== null) ? (int) $row[$startcol + 13] : null;
			$this->resetModified();

			$this->setNew(false);

			if ($rehydrate) {
				$this->ensureConsistency();
			}

			// FIXME - using NUM_COLUMNS may be clearer.
			return $startcol + 14; // 14 = UsersPeer::NUM_COLUMNS - UsersPeer::NUM_LAZY_LOAD_COLUMNS).

		} catch (Exception $e) {
			throw new PropelException("Error populating Users object", $e);
		}
	}

	/**
	 * Checks and repairs the internal consistency of the object.
	 *
	 * This method is executed after an already-instantiated object is re-hydrated
	 * from the database.  It exists to check any foreign keys to make sure that
	 * the objects related to the current object are correct based on foreign key.
	 *
	 * You can override this method in the stub class, but you should always invoke
	 * the base method from the overridden method (i.e. parent::ensureConsistency()),
	 * in case your model changes.
	 *
	 * @throws     PropelException
	 */
	public function ensureConsistency()
	{

	} // ensureConsistency

	/**
	 * Reloads this object from datastore based on primary key and (optionally) resets all associated objects.
	 *
	 * This will only work if the object has been saved and has a valid primary key set.
	 *
	 * @param      boolean $deep (optional) Whether to also de-associated any related objects.
	 * @param      PropelPDO $con (optional) The PropelPDO connection to use.
	 * @return     void
	 * @throws     PropelException - if this object is deleted, unsaved or doesn't have pk match in db
	 */
	public function reload($deep = false, PropelPDO $con = null)
	{
		if ($this->isDeleted()) {
			throw new PropelException("Cannot reload a deleted object.");
		}

		if ($this->isNew()) {
			throw new PropelException("Cannot reload an unsaved object.");
		}

		if ($con === null) {
			$con = Propel::getConnection(UsersPeer::DATABASE_NAME, Propel::CONNECTION_READ);
		}

		// We don't need to alter the object instance pool; we're just modifying this instance
		// already in the pool.

		$stmt = UsersPeer::doSelectStmt($this->buildPkeyCriteria(), $con);
		$row = $stmt->fetch(PDO::FETCH_NUM);
		$stmt->closeCursor();
		if (!$row) {
			throw new PropelException('Cannot find matching row in the database to reload object values.');
		}
		$this->hydrate($row, 0, true); // rehydrate

		if ($deep) {  // also de-associate any related objects?

		} // if (deep)
	}

	/**
	 * Removes this object from datastore and sets delete attribute.
	 *
	 * @param      PropelPDO $con
	 * @return     void
	 * @throws     PropelException
	 * @see        BaseObject::setDeleted()
	 * @see        BaseObject::isDeleted()
	 */
	public function delete(PropelPDO $con = null)
	{
		if ($this->isDeleted()) {
			throw new PropelException("This object has already been deleted.");
		}

		if ($con === null) {
			$con = Propel::getConnection(UsersPeer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			UsersPeer::doDelete($this, $con);
			$this->setDeleted(true);
			$con->commit();
		} catch (PropelException $e) {
			$con->rollBack();
			throw $e;
		}
	}

	/**
	 * Persists this object to the database.
	 *
	 * If the object is new, it inserts it; otherwise an update is performed.
	 * All modified related objects will also be persisted in the doSave()
	 * method.  This method wraps all precipitate database operations in a
	 * single transaction.
	 *
	 * @param      PropelPDO $con
	 * @return     int The number of rows affected by this insert/update and any referring fk objects' save() operations.
	 * @throws     PropelException
	 * @see        doSave()
	 */
	public function save(PropelPDO $con = null)
	{
		if ($this->isDeleted()) {
			throw new PropelException("You cannot save an object that has been deleted.");
		}

		if ($con === null) {
			$con = Propel::getConnection(UsersPeer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			$affectedRows = $this->doSave($con);
			$con->commit();
			UsersPeer::addInstanceToPool($this);
			return $affectedRows;
		} catch (PropelException $e) {
			$con->rollBack();
			throw $e;
		}
	}

	/**
	 * Performs the work of inserting or updating the row in the database.
	 *
	 * If the object is new, it inserts it; otherwise an update is performed.
	 * All related objects are also updated in this method.
	 *
	 * @param      PropelPDO $con
	 * @return     int The number of rows affected by this insert/update and any referring fk objects' save() operations.
	 * @throws     PropelException
	 * @see        save()
	 */
	protected function doSave(PropelPDO $con)
	{
		$affectedRows = 0; // initialize var to track total num of affected rows
		if (!$this->alreadyInSave) {
			$this->alreadyInSave = true;


			// If this object has been modified, then save it to the database.
			if ($this->isModified()) {
				if ($this->isNew()) {
					$pk = UsersPeer::doInsert($this, $con);
					$affectedRows += 1; // we are assuming that there is only 1 row per doInsert() which
										 // should always be true here (even though technically
										 // BasePeer::doInsert() can insert multiple rows).

					$this->setNew(false);
				} else {
					$affectedRows += UsersPeer::doUpdate($this, $con);
				}

				$this->resetModified(); // [HL] After being saved an object is no longer 'modified'
			}

			$this->alreadyInSave = false;

		}
		return $affectedRows;
	} // doSave()

	/**
	 * Array of ValidationFailed objects.
	 * @var        array ValidationFailed[]
	 */
	protected $validationFailures = array();

	/**
	 * Gets any ValidationFailed objects that resulted from last call to validate().
	 *
	 *
	 * @return     array ValidationFailed[]
	 * @see        validate()
	 */
	public function getValidationFailures()
	{
		return $this->validationFailures;
	}

	/**
	 * Validates the objects modified field values and all objects related to this table.
	 *
	 * If $columns is either a column name or an array of column names
	 * only those columns are validated.
	 *
	 * @param      mixed $columns Column name or an array of column names.
	 * @return     boolean Whether all columns pass validation.
	 * @see        doValidate()
	 * @see        getValidationFailures()
	 */
	public function validate($columns = null)
	{
		$res = $this->doValidate($columns);
		if ($res === true) {
			$this->validationFailures = array();
			return true;
		} else {
			$this->validationFailures = $res;
			return false;
		}
	}

	/**
	 * This function performs the validation work for complex object models.
	 *
	 * In addition to checking the current object, all related objects will
	 * also be validated.  If all pass then <code>true</code> is returned; otherwise
	 * an aggreagated array of ValidationFailed objects will be returned.
	 *
	 * @param      array $columns Array of column names to validate.
	 * @return     mixed <code>true</code> if all validations pass; array of <code>ValidationFailed</code> objets otherwise.
	 */
	protected function doValidate($columns = null)
	{
		if (!$this->alreadyInValidation) {
			$this->alreadyInValidation = true;
			$retval = null;

			$failureMap = array();


			if (($retval = UsersPeer::doValidate($this, $columns)) !== true) {
				$failureMap = array_merge($failureMap, $retval);
			}



			$this->alreadyInValidation = false;
		}

		return (!empty($failureMap) ? $failureMap : true);
	}

	/**
	 * Build a Criteria object containing the values of all modified columns in this object.
	 *
	 * @return     Criteria The Criteria object containing all modified values.
	 */
	public function buildCriteria()
	{
		$criteria = new Criteria(UsersPeer::DATABASE_NAME);

		if ($this->isColumnModified(UsersPeer::HANDLE)) $criteria->add(UsersPeer::HANDLE, $this->handle);
		if ($this->isColumnModified(UsersPeer::PERMISSIONS)) $criteria->add(UsersPeer::PERMISSIONS, $this->permissions);
		if ($this->isColumnModified(UsersPeer::PASSHASH)) $criteria->add(UsersPeer::PASSHASH, $this->passhash);
		if ($this->isColumnModified(UsersPeer::OAUTH)) $criteria->add(UsersPeer::OAUTH, $this->oauth);
		if ($this->isColumnModified(UsersPeer::OPENID)) $criteria->add(UsersPeer::OPENID, $this->openid);
		if ($this->isColumnModified(UsersPeer::EMAIL)) $criteria->add(UsersPeer::EMAIL, $this->email);
		if ($this->isColumnModified(UsersPeer::REALNAME)) $criteria->add(UsersPeer::REALNAME, $this->realname);
		if ($this->isColumnModified(UsersPeer::UID)) $criteria->add(UsersPeer::UID, $this->uid);
		if ($this->isColumnModified(UsersPeer::URL)) $criteria->add(UsersPeer::URL, $this->url);
		if ($this->isColumnModified(UsersPeer::AVATARMEDIAUID)) $criteria->add(UsersPeer::AVATARMEDIAUID, $this->avatarmediauid);
		if ($this->isColumnModified(UsersPeer::LANGUAGEORDER)) $criteria->add(UsersPeer::LANGUAGEORDER, $this->languageorder);
		if ($this->isColumnModified(UsersPeer::COUNTRY)) $criteria->add(UsersPeer::COUNTRY, $this->country);
		if ($this->isColumnModified(UsersPeer::REGION)) $criteria->add(UsersPeer::REGION, $this->region);
		if ($this->isColumnModified(UsersPeer::KARMA)) $criteria->add(UsersPeer::KARMA, $this->karma);

		return $criteria;
	}

	/**
	 * Builds a Criteria object containing the primary key for this object.
	 *
	 * Unlike buildCriteria() this method includes the primary key values regardless
	 * of whether or not they have been modified.
	 *
	 * @return     Criteria The Criteria object containing value(s) for primary key(s).
	 */
	public function buildPkeyCriteria()
	{
		$criteria = new Criteria(UsersPeer::DATABASE_NAME);

		$criteria->add(UsersPeer::HANDLE, $this->handle);

		return $criteria;
	}

	/**
	 * Returns the primary key for this object (row).
	 * @return     string
	 */
	public function getPrimaryKey()
	{
		return $this->getHandle();
	}

	/**
	 * Generic method to set the primary key (handle column).
	 *
	 * @param      string $key Primary key.
	 * @return     void
	 */
	public function setPrimaryKey($key)
	{
		$this->setHandle($key);
	}

	/**
	 * Sets contents of passed object to values from current object.
	 *
	 * If desired, this method can also make copies of all associated (fkey referrers)
	 * objects.
	 *
	 * @param      object $copyObj An object of Users (or compatible) type.
	 * @param      boolean $deepCopy Whether to also copy all rows that refer (by fkey) to the current row.
	 * @throws     PropelException
	 */
	public function copyInto($copyObj, $deepCopy = false)
	{

		$copyObj->setHandle($this->handle);

		$copyObj->setPermissions($this->permissions);

		$copyObj->setPasshash($this->passhash);

		$copyObj->setOauth($this->oauth);

		$copyObj->setOpenid($this->openid);

		$copyObj->setEmail($this->email);

		$copyObj->setRealname($this->realname);

		$copyObj->setUid($this->uid);

		$copyObj->setUrl($this->url);

		$copyObj->setAvatarmediauid($this->avatarmediauid);

		$copyObj->setLanguageorder($this->languageorder);

		$copyObj->setCountry($this->country);

		$copyObj->setRegion($this->region);

		$copyObj->setKarma($this->karma);


		$copyObj->setNew(true);

	}

	/**
	 * Makes a copy of this object that will be inserted as a new row in table when saved.
	 * It creates a new object filling in the simple attributes, but skipping any primary
	 * keys that are defined for the table.
	 *
	 * If desired, this method can also make copies of all associated (fkey referrers)
	 * objects.
	 *
	 * @param      boolean $deepCopy Whether to also copy all rows that refer (by fkey) to the current row.
	 * @return     Users Clone of current object.
	 * @throws     PropelException
	 */
	public function copy($deepCopy = false)
	{
		// we use get_class(), because this might be a subclass
		$clazz = get_class($this);
		$copyObj = new $clazz();
		$this->copyInto($copyObj, $deepCopy);
		return $copyObj;
	}

	/**
	 * Returns a peer instance associated with this om.
	 *
	 * Since Peer classes are not to have any instance attributes, this method returns the
	 * same instance for all member of this class. The method could therefore
	 * be static, but this would prevent one from overriding the behavior.
	 *
	 * @return     UsersPeer
	 */
	public function getPeer()
	{
		if (self::$peer === null) {
			self::$peer = new UsersPeer();
		}
		return self::$peer;
	}

	/**
	 * Resets all collections of referencing foreign keys.
	 *
	 * This method is a user-space workaround for PHP's inability to garbage collect objects
	 * with circular references.  This is currently necessary when using Propel in certain
	 * daemon or large-volumne/high-memory operations.
	 *
	 * @param      boolean $deep Whether to also clear the references on all associated objects.
	 */
	public function clearAllReferences($deep = false)
	{
		if ($deep) {
		} // if ($deep)

	}

} // BaseUsers
