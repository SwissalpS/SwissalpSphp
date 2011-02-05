<?php

/**
 * Base class that represents a row from the 'mediaMap' table.
 *
 *
 *
 * @package    skyprom_phocoa.om
 */
abstract class BaseMediamap extends BaseObject  implements Persistent {


	/**
	 * The Peer class.
	 * Instance provides a convenient way of calling static methods on a class
	 * that calling code may not be able to identify.
	 * @var        MediamapPeer
	 */
	protected static $peer;

	/**
	 * The value for the mediauid field.
	 * @var        int
	 */
	protected $mediauid;

	/**
	 * The value for the url field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $url;

	/**
	 * The value for the type field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $type;

	/**
	 * The value for the width field.
	 * Note: this column has a database default value of: 240
	 * @var        int
	 */
	protected $width;

	/**
	 * The value for the height field.
	 * Note: this column has a database default value of: 80
	 * @var        int
	 */
	protected $height;

	/**
	 * The value for the cssclass field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $cssclass;

	/**
	 * The value for the mime field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $mime;

	/**
	 * The value for the karma field.
	 * Note: this column has a database default value of: 100
	 * @var        int
	 */
	protected $karma;

	/**
	 * The value for the bigwidth field.
	 * Note: this column has a database default value of: 800
	 * @var        int
	 */
	protected $bigwidth;

	/**
	 * The value for the bigheight field.
	 * Note: this column has a database default value of: 600
	 * @var        int
	 */
	protected $bigheight;

	/**
	 * The value for the bigurl field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $bigurl;

	/**
	 * The value for the thumbwidth field.
	 * @var        int
	 */
	protected $thumbwidth;

	/**
	 * The value for the thumbheight field.
	 * @var        int
	 */
	protected $thumbheight;

	/**
	 * The value for the thumburl field.
	 * Note: this column has a database default value of: ''
	 * @var        string
	 */
	protected $thumburl;

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
	 * Initializes internal state of BaseMediamap object.
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
		$this->url = '';
		$this->type = '';
		$this->width = 240;
		$this->height = 80;
		$this->cssclass = '';
		$this->mime = '';
		$this->karma = 100;
		$this->bigwidth = 800;
		$this->bigheight = 600;
		$this->bigurl = '';
		$this->thumburl = '';
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
	 * Get the [url] column value.
	 *
	 * @return     string
	 */
	public function getUrl()
	{
		return $this->url;
	}

	/**
	 * Get the [type] column value.
	 *
	 * @return     string
	 */
	public function getType()
	{
		return $this->type;
	}

	/**
	 * Get the [width] column value.
	 *
	 * @return     int
	 */
	public function getWidth()
	{
		return $this->width;
	}

	/**
	 * Get the [height] column value.
	 *
	 * @return     int
	 */
	public function getHeight()
	{
		return $this->height;
	}

	/**
	 * Get the [cssclass] column value.
	 *
	 * @return     string
	 */
	public function getCssclass()
	{
		return $this->cssclass;
	}

	/**
	 * Get the [mime] column value.
	 *
	 * @return     string
	 */
	public function getMime()
	{
		return $this->mime;
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
	 * Get the [bigwidth] column value.
	 *
	 * @return     int
	 */
	public function getBigwidth()
	{
		return $this->bigwidth;
	}

	/**
	 * Get the [bigheight] column value.
	 *
	 * @return     int
	 */
	public function getBigheight()
	{
		return $this->bigheight;
	}

	/**
	 * Get the [bigurl] column value.
	 *
	 * @return     string
	 */
	public function getBigurl()
	{
		return $this->bigurl;
	}

	/**
	 * Get the [thumbwidth] column value.
	 *
	 * @return     int
	 */
	public function getThumbwidth()
	{
		return $this->thumbwidth;
	}

	/**
	 * Get the [thumbheight] column value.
	 *
	 * @return     int
	 */
	public function getThumbheight()
	{
		return $this->thumbheight;
	}

	/**
	 * Get the [thumburl] column value.
	 *
	 * @return     string
	 */
	public function getThumburl()
	{
		return $this->thumburl;
	}

	/**
	 * Set the value of [mediauid] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setMediauid($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->mediauid !== $v) {
			$this->mediauid = $v;
			$this->modifiedColumns[] = MediamapPeer::MEDIAUID;
		}

		return $this;
	} // setMediauid()

	/**
	 * Set the value of [url] column.
	 *
	 * @param      string $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setUrl($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->url !== $v || $v === '') {
			$this->url = $v;
			$this->modifiedColumns[] = MediamapPeer::URL;
		}

		return $this;
	} // setUrl()

	/**
	 * Set the value of [type] column.
	 *
	 * @param      string $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setType($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->type !== $v || $v === '') {
			$this->type = $v;
			$this->modifiedColumns[] = MediamapPeer::TYPE;
		}

		return $this;
	} // setType()

	/**
	 * Set the value of [width] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setWidth($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->width !== $v || $v === 240) {
			$this->width = $v;
			$this->modifiedColumns[] = MediamapPeer::WIDTH;
		}

		return $this;
	} // setWidth()

	/**
	 * Set the value of [height] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setHeight($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->height !== $v || $v === 80) {
			$this->height = $v;
			$this->modifiedColumns[] = MediamapPeer::HEIGHT;
		}

		return $this;
	} // setHeight()

	/**
	 * Set the value of [cssclass] column.
	 *
	 * @param      string $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setCssclass($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->cssclass !== $v || $v === '') {
			$this->cssclass = $v;
			$this->modifiedColumns[] = MediamapPeer::CSSCLASS;
		}

		return $this;
	} // setCssclass()

	/**
	 * Set the value of [mime] column.
	 *
	 * @param      string $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setMime($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->mime !== $v || $v === '') {
			$this->mime = $v;
			$this->modifiedColumns[] = MediamapPeer::MIME;
		}

		return $this;
	} // setMime()

	/**
	 * Set the value of [karma] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setKarma($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->karma !== $v || $v === 100) {
			$this->karma = $v;
			$this->modifiedColumns[] = MediamapPeer::KARMA;
		}

		return $this;
	} // setKarma()

	/**
	 * Set the value of [bigwidth] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setBigwidth($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->bigwidth !== $v || $v === 800) {
			$this->bigwidth = $v;
			$this->modifiedColumns[] = MediamapPeer::BIGWIDTH;
		}

		return $this;
	} // setBigwidth()

	/**
	 * Set the value of [bigheight] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setBigheight($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->bigheight !== $v || $v === 600) {
			$this->bigheight = $v;
			$this->modifiedColumns[] = MediamapPeer::BIGHEIGHT;
		}

		return $this;
	} // setBigheight()

	/**
	 * Set the value of [bigurl] column.
	 *
	 * @param      string $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setBigurl($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->bigurl !== $v || $v === '') {
			$this->bigurl = $v;
			$this->modifiedColumns[] = MediamapPeer::BIGURL;
		}

		return $this;
	} // setBigurl()

	/**
	 * Set the value of [thumbwidth] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setThumbwidth($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->thumbwidth !== $v) {
			$this->thumbwidth = $v;
			$this->modifiedColumns[] = MediamapPeer::THUMBWIDTH;
		}

		return $this;
	} // setThumbwidth()

	/**
	 * Set the value of [thumbheight] column.
	 *
	 * @param      int $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setThumbheight($v)
	{
		if ($v !== null) {
			$v = (int) $v;
		}

		if ($this->thumbheight !== $v) {
			$this->thumbheight = $v;
			$this->modifiedColumns[] = MediamapPeer::THUMBHEIGHT;
		}

		return $this;
	} // setThumbheight()

	/**
	 * Set the value of [thumburl] column.
	 *
	 * @param      string $v new value
	 * @return     Mediamap The current object (for fluent API support)
	 */
	public function setThumburl($v)
	{
		if ($v !== null) {
			$v = (string) $v;
		}

		if ($this->thumburl !== $v || $v === '') {
			$this->thumburl = $v;
			$this->modifiedColumns[] = MediamapPeer::THUMBURL;
		}

		return $this;
	} // setThumburl()

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
			if (array_diff($this->modifiedColumns, array(MediamapPeer::URL,MediamapPeer::TYPE,MediamapPeer::WIDTH,MediamapPeer::HEIGHT,MediamapPeer::CSSCLASS,MediamapPeer::MIME,MediamapPeer::KARMA,MediamapPeer::BIGWIDTH,MediamapPeer::BIGHEIGHT,MediamapPeer::BIGURL,MediamapPeer::THUMBURL))) {
				return false;
			}

			if ($this->url !== '') {
				return false;
			}

			if ($this->type !== '') {
				return false;
			}

			if ($this->width !== 240) {
				return false;
			}

			if ($this->height !== 80) {
				return false;
			}

			if ($this->cssclass !== '') {
				return false;
			}

			if ($this->mime !== '') {
				return false;
			}

			if ($this->karma !== 100) {
				return false;
			}

			if ($this->bigwidth !== 800) {
				return false;
			}

			if ($this->bigheight !== 600) {
				return false;
			}

			if ($this->bigurl !== '') {
				return false;
			}

			if ($this->thumburl !== '') {
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

			$this->mediauid = ($row[$startcol + 0] !== null) ? (int) $row[$startcol + 0] : null;
			$this->url = ($row[$startcol + 1] !== null) ? (string) $row[$startcol + 1] : null;
			$this->type = ($row[$startcol + 2] !== null) ? (string) $row[$startcol + 2] : null;
			$this->width = ($row[$startcol + 3] !== null) ? (int) $row[$startcol + 3] : null;
			$this->height = ($row[$startcol + 4] !== null) ? (int) $row[$startcol + 4] : null;
			$this->cssclass = ($row[$startcol + 5] !== null) ? (string) $row[$startcol + 5] : null;
			$this->mime = ($row[$startcol + 6] !== null) ? (string) $row[$startcol + 6] : null;
			$this->karma = ($row[$startcol + 7] !== null) ? (int) $row[$startcol + 7] : null;
			$this->bigwidth = ($row[$startcol + 8] !== null) ? (int) $row[$startcol + 8] : null;
			$this->bigheight = ($row[$startcol + 9] !== null) ? (int) $row[$startcol + 9] : null;
			$this->bigurl = ($row[$startcol + 10] !== null) ? (string) $row[$startcol + 10] : null;
			$this->thumbwidth = ($row[$startcol + 11] !== null) ? (int) $row[$startcol + 11] : null;
			$this->thumbheight = ($row[$startcol + 12] !== null) ? (int) $row[$startcol + 12] : null;
			$this->thumburl = ($row[$startcol + 13] !== null) ? (string) $row[$startcol + 13] : null;
			$this->resetModified();

			$this->setNew(false);

			if ($rehydrate) {
				$this->ensureConsistency();
			}

			// FIXME - using NUM_COLUMNS may be clearer.
			return $startcol + 14; // 14 = MediamapPeer::NUM_COLUMNS - MediamapPeer::NUM_LAZY_LOAD_COLUMNS).

		} catch (Exception $e) {
			throw new PropelException("Error populating Mediamap object", $e);
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
			$con = Propel::getConnection(MediamapPeer::DATABASE_NAME, Propel::CONNECTION_READ);
		}

		// We don't need to alter the object instance pool; we're just modifying this instance
		// already in the pool.

		$stmt = MediamapPeer::doSelectStmt($this->buildPkeyCriteria(), $con);
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
			$con = Propel::getConnection(MediamapPeer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			MediamapPeer::doDelete($this, $con);
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
			$con = Propel::getConnection(MediamapPeer::DATABASE_NAME, Propel::CONNECTION_WRITE);
		}

		$con->beginTransaction();
		try {
			$affectedRows = $this->doSave($con);
			$con->commit();
			MediamapPeer::addInstanceToPool($this);
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
				$this->modifiedColumns[] = MediamapPeer::MEDIAUID;
			}

			// If this object has been modified, then save it to the database.
			if ($this->isModified()) {
				if ($this->isNew()) {
					$pk = MediamapPeer::doInsert($this, $con);
					$affectedRows += 1; // we are assuming that there is only 1 row per doInsert() which
										 // should always be true here (even though technically
										 // BasePeer::doInsert() can insert multiple rows).

					$this->setMediauid($pk);  //[IMV] update autoincrement primary key

					$this->setNew(false);
				} else {
					$affectedRows += MediamapPeer::doUpdate($this, $con);
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


			if (($retval = MediamapPeer::doValidate($this, $columns)) !== true) {
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
		$criteria = new Criteria(MediamapPeer::DATABASE_NAME);

		if ($this->isColumnModified(MediamapPeer::MEDIAUID)) $criteria->add(MediamapPeer::MEDIAUID, $this->mediauid);
		if ($this->isColumnModified(MediamapPeer::URL)) $criteria->add(MediamapPeer::URL, $this->url);
		if ($this->isColumnModified(MediamapPeer::TYPE)) $criteria->add(MediamapPeer::TYPE, $this->type);
		if ($this->isColumnModified(MediamapPeer::WIDTH)) $criteria->add(MediamapPeer::WIDTH, $this->width);
		if ($this->isColumnModified(MediamapPeer::HEIGHT)) $criteria->add(MediamapPeer::HEIGHT, $this->height);
		if ($this->isColumnModified(MediamapPeer::CSSCLASS)) $criteria->add(MediamapPeer::CSSCLASS, $this->cssclass);
		if ($this->isColumnModified(MediamapPeer::MIME)) $criteria->add(MediamapPeer::MIME, $this->mime);
		if ($this->isColumnModified(MediamapPeer::KARMA)) $criteria->add(MediamapPeer::KARMA, $this->karma);
		if ($this->isColumnModified(MediamapPeer::BIGWIDTH)) $criteria->add(MediamapPeer::BIGWIDTH, $this->bigwidth);
		if ($this->isColumnModified(MediamapPeer::BIGHEIGHT)) $criteria->add(MediamapPeer::BIGHEIGHT, $this->bigheight);
		if ($this->isColumnModified(MediamapPeer::BIGURL)) $criteria->add(MediamapPeer::BIGURL, $this->bigurl);
		if ($this->isColumnModified(MediamapPeer::THUMBWIDTH)) $criteria->add(MediamapPeer::THUMBWIDTH, $this->thumbwidth);
		if ($this->isColumnModified(MediamapPeer::THUMBHEIGHT)) $criteria->add(MediamapPeer::THUMBHEIGHT, $this->thumbheight);
		if ($this->isColumnModified(MediamapPeer::THUMBURL)) $criteria->add(MediamapPeer::THUMBURL, $this->thumburl);

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
		$criteria = new Criteria(MediamapPeer::DATABASE_NAME);

		$criteria->add(MediamapPeer::MEDIAUID, $this->mediauid);

		return $criteria;
	}

	/**
	 * Returns the primary key for this object (row).
	 * @return     int
	 */
	public function getPrimaryKey()
	{
		return $this->getMediauid();
	}

	/**
	 * Generic method to set the primary key (mediauid column).
	 *
	 * @param      int $key Primary key.
	 * @return     void
	 */
	public function setPrimaryKey($key)
	{
		$this->setMediauid($key);
	}

	/**
	 * Sets contents of passed object to values from current object.
	 *
	 * If desired, this method can also make copies of all associated (fkey referrers)
	 * objects.
	 *
	 * @param      object $copyObj An object of Mediamap (or compatible) type.
	 * @param      boolean $deepCopy Whether to also copy all rows that refer (by fkey) to the current row.
	 * @throws     PropelException
	 */
	public function copyInto($copyObj, $deepCopy = false)
	{

		$copyObj->setUrl($this->url);

		$copyObj->setType($this->type);

		$copyObj->setWidth($this->width);

		$copyObj->setHeight($this->height);

		$copyObj->setCssclass($this->cssclass);

		$copyObj->setMime($this->mime);

		$copyObj->setKarma($this->karma);

		$copyObj->setBigwidth($this->bigwidth);

		$copyObj->setBigheight($this->bigheight);

		$copyObj->setBigurl($this->bigurl);

		$copyObj->setThumbwidth($this->thumbwidth);

		$copyObj->setThumbheight($this->thumbheight);

		$copyObj->setThumburl($this->thumburl);


		$copyObj->setNew(true);

		$copyObj->setMediauid(NULL); // this is a auto-increment column, so set to default value

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
	 * @return     Mediamap Clone of current object.
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
	 * @return     MediamapPeer
	 */
	public function getPeer()
	{
		if (self::$peer === null) {
			self::$peer = new MediamapPeer();
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

} // BaseMediamap
