<?php

/**
 * Base class that represents a row from the 'Bla000' table.
 *
 *
 *
 * @package    skyprom_phocoa.om
 */
abstract class BaseBla000 extends BaseObject  implements Persistent {


	/**
	 * The Peer class.
	 * Instance provides a convenient way of calling static methods on a class
	 * that calling code may not be able to identify.
	 * @var        Bla000Peer
	 */
	protected static $peer;

	/**
	 * The value for the uid field.
	 * @var        string
	 */
	protected $uid;

	/**
	 * The value for the comment field.
	 * @var        string
	 */
	protected $comment;

	/**
	 * The value for the en field.
	 * @var        string
	 */
	protected $en;

	/**
	 * The value for the de field.
	 * @var        string
	 */
	protected $de;

	/**
	 * The value for the fr field.
	 * @var        string
	 */
	protected $fr;

	/**
	 * The value for the it field.
	 * @var        string
	 */
	protected $it;

	/**
	 * The value for the rm field.
	 * @var        string
	 */
	protected $rm;

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
	 * Initializes internal state of BaseBla000 object.
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
	}

	/**
	 * Get the [uid] column value.
	 *
	 * @return     string
	 */
	public function getUid()
	{
		return $this->uid;
	}

	/**
	 * Get the [comment] column value.
	 *
	 * @return     string
	 */
	public function getComment()
	{
		return $this->comment;
	}

	/**
	 * Get the [en] column value.
	 *
	 * @return     string
	 */
	public function getEn()
	{
		return $this->en;
	}

	/**
	 * Get the [de] column value.
	 *
	 * @return     string
	 */
	public function getDe()
	{
		return $this->de;
	}

	/**
	 * Get the [fr] column value.
	 *
	 * @return     string
	 */
	public function getFr()
	{
		return $this->fr;
	}

	/**
	 * Get the [it] column value.
	 *
	 * @return     string
	 */
	public function getIt()
	{
		return $this->it;
	}

	/**
	 * Get the [rm] column value.
	 *
	 * @return     string
	 */
	public function getRm()
	{
		return $this->rm;
	}

	/**
	 * Set the value of [uid] column.
	 *
	 * @param      string $v new value
	 * @return     Bla000 The current object (for fluent API support)
	 */
	public function setUid($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->uid !== $v) {
			$this->uid = $v;
			$this->modifiedColumns[] = Bla000Peer::UID;
		}

		return $this;
	} // setUid()

	/**
	 * Set the value of [comment] column.
	 *
	 * @param      string $v new value
	 * @return     Bla000 The current object (for fluent API support)
	 */
	public function setComment($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->comment !== $v) {
			$this->comment = $v;
			$this->modifiedColumns[] = Bla000Peer::COMMENT;
		}

		return $this;
	} // setComment()

	/**
	 * Set the value of [en] column.
	 *
	 * @param      string $v new value
	 * @return     Bla000 The current object (for fluent API support)
	 */
	public function setEn($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->en !== $v) {
			$this->en = $v;
			$this->modifiedColumns[] = Bla000Peer::EN;
		}

		return $this;
	} // setEn()

	/**
	 * Set the value of [de] column.
	 *
	 * @param      string $v new value
	 * @return     Bla000 The current object (for fluent API support)
	 */
	public function setDe($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->de !== $v) {
			$this->de = $v;
			$this->modifiedColumns[] = Bla000Peer::DE;
		}

		return $this;
	} // setDe()

	/**
	 * Set the value of [fr] column.
	 *
	 * @param      string $v new value
	 * @return     Bla000 The current object (for fluent API support)
	 */
	public function setFr($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->fr !== $v) {
			$this->fr = $v;
			$this->modifiedColumns[] = Bla000Peer::FR;
		}

		return $this;
	} // setFr()

	/**
	 * Set the value of [it] column.
	 *
	 * @param      string $v new value
	 * @return     Bla000 The current object (for fluent API support)
	 */
	public function setIt($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->it !== $v) {
			$this->it = $v;
			$this->modifiedColumns[] = Bla000Peer::IT;
		}

		return $this;
	} // setIt()

	/**
	 * Set the value of [rm] column.
	 *
	 * @param      string $v new value
	 * @return     Bla000 The current object (for fluent API support)
	 */
	public function setRm($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->rm !== $v) {
			$this->rm = $v;
			$this->modifiedColumns[] = Bla000Peer::RM;
		}

		return $this;
	} // setRm()

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
			if (array_diff($this->modifiedColumns, array())) {
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

			$this->uid = ($row[$startcol + 0] !== null) ? (string) $row[$startcol + 0] : null;
			$this->comment = ($row[$startcol + 1] !== null) ? (string) $row[$startcol + 1] : null;
			$this->en = ($row[$startcol + 2] !== null) ? (string) $row[$startcol + 2] : null;
			$this->de = ($row[$startcol + 3] !== null) ? (string) $row[$startcol + 3] : null;
			$this->fr = ($row[$startcol + 4] !== null) ? (string) $row[$startcol + 4] : null;
			$this->it = ($row[$startcol + 5] !== null) ? (string) $row[$startcol + 5] : null;
			$this->rm = ($row[$startcol + 6] !== null) ? (string) $row[$startcol + 6] : null;
			$this->resetModified();

			$this->setNew(false);

			if ($rehydrate) {
				$this->ensureConsistency();
			}

			// FIXME - using NUM_COLUMNS may be clearer.
			return $startcol + 7; // 7 = Bla000Peer::NUM_COLUMNS - Bla000Peer::NUM_LAZY_LOAD_COLUMNS).

		} catch (Exception $e) {
			throw new PropelException("Error populating Bla000 object", $e);
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
			$con = Propel::getConnection(Bla000Peer::DATABASE_NAME, Propel::CONNECTION_READ);
		}

		// We don't need to alter the object instance pool; we're just modifying this instance
		// already in the pool.

		$stmt = Bla000Peer::doSelectStmt($this->buildPkeyCriteria(), $con);
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
			$con = Propel::getConnection(Bla000Peer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			Bla000Peer::doDelete($this, $con);
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
			$con = Propel::getConnection(Bla000Peer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			$affectedRows = $this->doSave($con);
			$con->commit();
			Bla000Peer::addInstanceToPool($this);
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
					$pk = Bla000Peer::doInsert($this, $con);
					$affectedRows += 1; // we are assuming that there is only 1 row per doInsert() which
										 // should always be true here (even though technically
										 // BasePeer::doInsert() can insert multiple rows).

					$this->setNew(false);
				} else {
					$affectedRows += Bla000Peer::doUpdate($this, $con);
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


			if (($retval = Bla000Peer::doValidate($this, $columns)) !== true) {
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
		$criteria = new Criteria(Bla000Peer::DATABASE_NAME);

		if ($this->isColumnModified(Bla000Peer::UID)) $criteria->add(Bla000Peer::UID, $this->uid);
		if ($this->isColumnModified(Bla000Peer::COMMENT)) $criteria->add(Bla000Peer::COMMENT, $this->comment);
		if ($this->isColumnModified(Bla000Peer::EN)) $criteria->add(Bla000Peer::EN, $this->en);
		if ($this->isColumnModified(Bla000Peer::DE)) $criteria->add(Bla000Peer::DE, $this->de);
		if ($this->isColumnModified(Bla000Peer::FR)) $criteria->add(Bla000Peer::FR, $this->fr);
		if ($this->isColumnModified(Bla000Peer::IT)) $criteria->add(Bla000Peer::IT, $this->it);
		if ($this->isColumnModified(Bla000Peer::RM)) $criteria->add(Bla000Peer::RM, $this->rm);

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
		$criteria = new Criteria(Bla000Peer::DATABASE_NAME);

		$criteria->add(Bla000Peer::UID, $this->uid);

		return $criteria;
	}

	/**
	 * Returns the primary key for this object (row).
	 * @return     string
	 */
	public function getPrimaryKey()
	{
		return $this->getUid();
	}

	/**
	 * Generic method to set the primary key (uid column).
	 *
	 * @param      string $key Primary key.
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
	 * @param      object $copyObj An object of Bla000 (or compatible) type.
	 * @param      boolean $deepCopy Whether to also copy all rows that refer (by fkey) to the current row.
	 * @throws     PropelException
	 */
	public function copyInto($copyObj, $deepCopy = false)
	{

		$copyObj->setUid($this->uid);

		$copyObj->setComment($this->comment);

		$copyObj->setEn($this->en);

		$copyObj->setDe($this->de);

		$copyObj->setFr($this->fr);

		$copyObj->setIt($this->it);

		$copyObj->setRm($this->rm);


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
	 * @return     Bla000 Clone of current object.
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
	 * @return     Bla000Peer
	 */
	public function getPeer()
	{
		if (self::$peer === null) {
			self::$peer = new Bla000Peer();
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

} // BaseBla000
