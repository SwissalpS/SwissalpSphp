<?php

/**
 * Base class that represents a row from the 'Notes' table.
 *
 *
 *
 * @package    basicPDOclasses.om
 */
abstract class BaseNotes extends BaseObject  implements Persistent {


	/**
	 * The Peer class.
	 * Instance provides a convenient way of calling static methods on a class
	 * that calling code may not be able to identify.
	 * @var        NotesPeer
	 */
	protected static $peer;

	/**
	 * The value for the uid field.
	 * @var        int
	 */
	protected $uid;

	/**
	 * The value for the name field.
	 * Note: this column has a database default value of: '_name_'
	 * @var        string
	 */
	protected $name;

	/**
	 * The value for the email field.
	 * @var        string
	 */
	protected $email;

	/**
	 * The value for the url0 field.
	 * @var        string
	 */
	protected $url0;

	/**
	 * The value for the url1 field.
	 * @var        string
	 */
	protected $url1;

	/**
	 * The value for the children field.
	 * @var        string
	 */
	protected $children;

	/**
	 * The value for the note field.
	 * @var        string
	 */
	protected $note;

	/**
	 * The value for the mediauid field.
	 * @var        int
	 */
	protected $mediauid;

	/**
	 * The value for the lang field.
	 * Note: this column has a database default value of: 'de'
	 * @var        string
	 */
	protected $lang;

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
	 * The value for the bridgeuid field.
	 * Note: this column has a database default value of: 0
	 * @var        int
	 */
	protected $bridgeuid;

	/**
	 * The value for the date field.
	 * Note: this column has a database default value of: 0
	 * @var        int
	 */
	protected $date;

	/**
	 * The value for the publish field.
	 * @var        int
	 */
	protected $publish;

	/**
	 * The value for the karma field.
	 * Note: this column has a database default value of: 100
	 * @var        int
	 */
	protected $karma;

	/**
	 * The value for the handle field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $handle;

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
	 * Initializes internal state of BaseNotes object.
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
		$this->name = '_name_';
		$this->lang = 'de';
		$this->country = '';
		$this->region = '';
		$this->bridgeuid = 0;
		$this->date = 0;
		$this->karma = 100;
		$this->handle = '';
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
	 * Get the [name] column value.
	 *
	 * @return     string
	 */
	public function getName()
	{
		return $this->name;
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
	 * Get the [url0] column value.
	 *
	 * @return     string
	 */
	public function getUrl0()
	{
		return $this->url0;
	}

	/**
	 * Get the [url1] column value.
	 *
	 * @return     string
	 */
	public function getUrl1()
	{
		return $this->url1;
	}

	/**
	 * Get the [children] column value.
	 *
	 * @return     string
	 */
	public function getChildren()
	{
		return $this->children;
	}

	/**
	 * Get the [note] column value.
	 *
	 * @return     string
	 */
	public function getNote()
	{
		return $this->note;
	}

	/**
	 * Get the [mediauid] column value.
	 *
	 * @return     int
	 */
	public function getMediauid()
	{
		return $this->mediauid;
	}

	/**
	 * Get the [lang] column value.
	 *
	 * @return     string
	 */
	public function getLang()
	{
		return $this->lang;
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
	 * Get the [bridgeuid] column value.
	 *
	 * @return     int
	 */
	public function getBridgeuid()
	{
		return $this->bridgeuid;
	}

	/**
	 * Get the [date] column value.
	 *
	 * @return     int
	 */
	public function getDate()
	{
		return $this->date;
	}

	/**
	 * Get the [publish] column value.
	 *
	 * @return     int
	 */
	public function getPublish()
	{
		return $this->publish;
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
	 * Get the [handle] column value.
	 *
	 * @return     string
	 */
	public function getHandle()
	{
		return $this->handle;
	}

	/**
	 * Set the value of [uid] column.
	 *
	 * @param      int $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setUid($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->uid !== $v) {
			$this->uid = $v;
			$this->modifiedColumns[] = NotesPeer::UID;
		}

		return $this;
	} // setUid()

	/**
	 * Set the value of [name] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setName($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->name !== $v || $v === '_name_') {
			$this->name = $v;
			$this->modifiedColumns[] = NotesPeer::NAME;
		}

		return $this;
	} // setName()

	/**
	 * Set the value of [email] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setEmail($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->email !== $v) {
			$this->email = $v;
			$this->modifiedColumns[] = NotesPeer::EMAIL;
		}

		return $this;
	} // setEmail()

	/**
	 * Set the value of [url0] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setUrl0($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->url0 !== $v) {
			$this->url0 = $v;
			$this->modifiedColumns[] = NotesPeer::URL0;
		}

		return $this;
	} // setUrl0()

	/**
	 * Set the value of [url1] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setUrl1($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->url1 !== $v) {
			$this->url1 = $v;
			$this->modifiedColumns[] = NotesPeer::URL1;
		}

		return $this;
	} // setUrl1()

	/**
	 * Set the value of [children] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setChildren($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->children !== $v) {
			$this->children = $v;
			$this->modifiedColumns[] = NotesPeer::CHILDREN;
		}

		return $this;
	} // setChildren()

	/**
	 * Set the value of [note] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setNote($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->note !== $v) {
			$this->note = $v;
			$this->modifiedColumns[] = NotesPeer::NOTE;
		}

		return $this;
	} // setNote()

	/**
	 * Set the value of [mediauid] column.
	 *
	 * @param      int $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setMediauid($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->mediauid !== $v) {
			$this->mediauid = $v;
			$this->modifiedColumns[] = NotesPeer::MEDIAUID;
		}

		return $this;
	} // setMediauid()

	/**
	 * Set the value of [lang] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setLang($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->lang !== $v || $v === 'de') {
			$this->lang = $v;
			$this->modifiedColumns[] = NotesPeer::LANG;
		}

		return $this;
	} // setLang()

	/**
	 * Set the value of [country] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setCountry($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->country !== $v || $v === '') {
			$this->country = $v;
			$this->modifiedColumns[] = NotesPeer::COUNTRY;
		}

		return $this;
	} // setCountry()

	/**
	 * Set the value of [region] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setRegion($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->region !== $v || $v === '') {
			$this->region = $v;
			$this->modifiedColumns[] = NotesPeer::REGION;
		}

		return $this;
	} // setRegion()

	/**
	 * Set the value of [bridgeuid] column.
	 *
	 * @param      int $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setBridgeuid($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->bridgeuid !== $v || $v === 0) {
			$this->bridgeuid = $v;
			$this->modifiedColumns[] = NotesPeer::BRIDGEUID;
		}

		return $this;
	} // setBridgeuid()

	/**
	 * Set the value of [date] column.
	 *
	 * @param      int $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setDate($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->date !== $v || $v === 0) {
			$this->date = $v;
			$this->modifiedColumns[] = NotesPeer::DATE;
		}

		return $this;
	} // setDate()

	/**
	 * Set the value of [publish] column.
	 *
	 * @param      int $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setPublish($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->publish !== $v) {
			$this->publish = $v;
			$this->modifiedColumns[] = NotesPeer::PUBLISH;
		}

		return $this;
	} // setPublish()

	/**
	 * Set the value of [karma] column.
	 *
	 * @param      int $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setKarma($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->karma !== $v || $v === 100) {
			$this->karma = $v;
			$this->modifiedColumns[] = NotesPeer::KARMA;
		}

		return $this;
	} // setKarma()

	/**
	 * Set the value of [handle] column.
	 *
	 * @param      string $v new value
	 * @return     Notes The current object (for fluent API support)
	 */
	public function setHandle($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->handle !== $v || $v === '') {
			$this->handle = $v;
			$this->modifiedColumns[] = NotesPeer::HANDLE;
		}

		return $this;
	} // setHandle()

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
			if (array_diff($this->modifiedColumns, array(NotesPeer::NAME,NotesPeer::LANG,NotesPeer::COUNTRY,NotesPeer::REGION,NotesPeer::BRIDGEUID,NotesPeer::DATE,NotesPeer::KARMA,NotesPeer::HANDLE))) {
				return false;
			}

			if ($this->name !== '_name_') {
				return false;
			}

			if ($this->lang !== 'de') {
				return false;
			}

			if ($this->country !== '') {
				return false;
			}

			if ($this->region !== '') {
				return false;
			}

			if ($this->bridgeuid !== 0) {
				return false;
			}

			if ($this->date !== 0) {
				return false;
			}

			if ($this->karma !== 100) {
				return false;
			}

			if ($this->handle !== '') {
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

			$this->uid = ($row[$startcol + 0] !== null) ? (int) $row[$startcol + 0] : null;
			$this->name = ($row[$startcol + 1] !== null) ? (string) $row[$startcol + 1] : null;
			$this->email = ($row[$startcol + 2] !== null) ? (string) $row[$startcol + 2] : null;
			$this->url0 = ($row[$startcol + 3] !== null) ? (string) $row[$startcol + 3] : null;
			$this->url1 = ($row[$startcol + 4] !== null) ? (string) $row[$startcol + 4] : null;
			$this->children = ($row[$startcol + 5] !== null) ? (string) $row[$startcol + 5] : null;
			$this->note = ($row[$startcol + 6] !== null) ? (string) $row[$startcol + 6] : null;
			$this->mediauid = ($row[$startcol + 7] !== null) ? (int) $row[$startcol + 7] : null;
			$this->lang = ($row[$startcol + 8] !== null) ? (string) $row[$startcol + 8] : null;
			$this->country = ($row[$startcol + 9] !== null) ? (string) $row[$startcol + 9] : null;
			$this->region = ($row[$startcol + 10] !== null) ? (string) $row[$startcol + 10] : null;
			$this->bridgeuid = ($row[$startcol + 11] !== null) ? (int) $row[$startcol + 11] : null;
			$this->date = ($row[$startcol + 12] !== null) ? (int) $row[$startcol + 12] : null;
			$this->publish = ($row[$startcol + 13] !== null) ? (int) $row[$startcol + 13] : null;
			$this->karma = ($row[$startcol + 14] !== null) ? (int) $row[$startcol + 14] : null;
			$this->handle = ($row[$startcol + 15] !== null) ? (string) $row[$startcol + 15] : null;
			$this->resetModified();

			$this->setNew(false);

			if ($rehydrate) {
				$this->ensureConsistency();
			}

			// FIXME - using NUM_COLUMNS may be clearer.
			return $startcol + 16; // 16 = NotesPeer::NUM_COLUMNS - NotesPeer::NUM_LAZY_LOAD_COLUMNS).

		} catch (Exception $e) {
			throw new PropelException("Error populating Notes object", $e);
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
			$con = Propel::getConnection(NotesPeer::DATABASE_NAME, Propel::CONNECTION_READ);
		}

		// We don't need to alter the object instance pool; we're just modifying this instance
		// already in the pool.

		$stmt = NotesPeer::doSelectStmt($this->buildPkeyCriteria(), $con);
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
			$con = Propel::getConnection(NotesPeer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			NotesPeer::doDelete($this, $con);
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
			$con = Propel::getConnection(NotesPeer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			$affectedRows = $this->doSave($con);
			$con->commit();
			NotesPeer::addInstanceToPool($this);
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

			if ($this->isNew() ) {
				$this->modifiedColumns[] = NotesPeer::UID;
			}

			// If this object has been modified, then save it to the database.
			if ($this->isModified()) {
				if ($this->isNew()) {
					$pk = NotesPeer::doInsert($this, $con);
					$affectedRows += 1; // we are assuming that there is only 1 row per doInsert() which
										 // should always be true here (even though technically
										 // BasePeer::doInsert() can insert multiple rows).

					$this->setUid($pk);  //[IMV] update autoincrement primary key

					$this->setNew(false);
				} else {
					$affectedRows += NotesPeer::doUpdate($this, $con);
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


			if (($retval = NotesPeer::doValidate($this, $columns)) !== true) {
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
		$criteria = new Criteria(NotesPeer::DATABASE_NAME);

		if ($this->isColumnModified(NotesPeer::UID)) $criteria->add(NotesPeer::UID, $this->uid);
		if ($this->isColumnModified(NotesPeer::NAME)) $criteria->add(NotesPeer::NAME, $this->name);
		if ($this->isColumnModified(NotesPeer::EMAIL)) $criteria->add(NotesPeer::EMAIL, $this->email);
		if ($this->isColumnModified(NotesPeer::URL0)) $criteria->add(NotesPeer::URL0, $this->url0);
		if ($this->isColumnModified(NotesPeer::URL1)) $criteria->add(NotesPeer::URL1, $this->url1);
		if ($this->isColumnModified(NotesPeer::CHILDREN)) $criteria->add(NotesPeer::CHILDREN, $this->children);
		if ($this->isColumnModified(NotesPeer::NOTE)) $criteria->add(NotesPeer::NOTE, $this->note);
		if ($this->isColumnModified(NotesPeer::MEDIAUID)) $criteria->add(NotesPeer::MEDIAUID, $this->mediauid);
		if ($this->isColumnModified(NotesPeer::LANG)) $criteria->add(NotesPeer::LANG, $this->lang);
		if ($this->isColumnModified(NotesPeer::COUNTRY)) $criteria->add(NotesPeer::COUNTRY, $this->country);
		if ($this->isColumnModified(NotesPeer::REGION)) $criteria->add(NotesPeer::REGION, $this->region);
		if ($this->isColumnModified(NotesPeer::BRIDGEUID)) $criteria->add(NotesPeer::BRIDGEUID, $this->bridgeuid);
		if ($this->isColumnModified(NotesPeer::DATE)) $criteria->add(NotesPeer::DATE, $this->date);
		if ($this->isColumnModified(NotesPeer::PUBLISH)) $criteria->add(NotesPeer::PUBLISH, $this->publish);
		if ($this->isColumnModified(NotesPeer::KARMA)) $criteria->add(NotesPeer::KARMA, $this->karma);
		if ($this->isColumnModified(NotesPeer::HANDLE)) $criteria->add(NotesPeer::HANDLE, $this->handle);

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
		$criteria = new Criteria(NotesPeer::DATABASE_NAME);

		$criteria->add(NotesPeer::UID, $this->uid);

		return $criteria;
	}

	/**
	 * Returns the primary key for this object (row).
	 * @return     int
	 */
	public function getPrimaryKey()
	{
		return $this->getUid();
	}

	/**
	 * Generic method to set the primary key (uid column).
	 *
	 * @param      int $key Primary key.
	 * @return     void
	 */
	public function setPrimaryKey($key)
	{
		$this->setUid($key);
	}

	/**
	 * Sets contents of passed object to values from current object.
	 *
	 * If desired, this method can also make copies of all associated (fkey referrers)
	 * objects.
	 *
	 * @param      object $copyObj An object of Notes (or compatible) type.
	 * @param      boolean $deepCopy Whether to also copy all rows that refer (by fkey) to the current row.
	 * @throws     PropelException
	 */
	public function copyInto($copyObj, $deepCopy = false)
	{

		$copyObj->setName($this->name);

		$copyObj->setEmail($this->email);

		$copyObj->setUrl0($this->url0);

		$copyObj->setUrl1($this->url1);

		$copyObj->setChildren($this->children);

		$copyObj->setNote($this->note);

		$copyObj->setMediauid($this->mediauid);

		$copyObj->setLang($this->lang);

		$copyObj->setCountry($this->country);

		$copyObj->setRegion($this->region);

		$copyObj->setBridgeuid($this->bridgeuid);

		$copyObj->setDate($this->date);

		$copyObj->setPublish($this->publish);

		$copyObj->setKarma($this->karma);

		$copyObj->setHandle($this->handle);


		$copyObj->setNew(true);

		$copyObj->setUid(NULL); // this is a auto-increment column, so set to default value

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
	 * @return     Notes Clone of current object.
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
	 * @return     NotesPeer
	 */
	public function getPeer()
	{
		if (self::$peer === null) {
			self::$peer = new NotesPeer();
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

} // BaseNotes
