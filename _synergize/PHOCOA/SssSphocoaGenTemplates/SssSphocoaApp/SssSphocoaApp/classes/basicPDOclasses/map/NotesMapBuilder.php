<?php


/**
 * This class adds structure of 'Notes' table to 'lukecom' DatabaseMap object.
 *
 *
 *
 * These statically-built map classes are used by Propel to do runtime db structure discovery.
 * For example, the createSelectSql() method checks the type of a given column used in an
 * ORDER BY clause to know whether it needs to apply SQL to make the ORDER BY case-insensitive
 * (i.e. if it's a text column type).
 *
 * @package    skyprom_phocoa.map
 */
class NotesMapBuilder implements MapBuilder {

	/**
	 * The (dot-path) name of this class
	 */
	const CLASS_NAME = 'skyprom_phocoa.map.NotesMapBuilder';

	/**
	 * The database map.
	 */
	private $dbMap;

	/**
	 * Tells us if this DatabaseMapBuilder is built so that we
	 * don't have to re-build it every time.
	 *
	 * @return     boolean true if this DatabaseMapBuilder is built, false otherwise.
	 */
	public function isBuilt()
	{
		return ($this->dbMap !== null);
	}

	/**
	 * Gets the databasemap this map builder built.
	 *
	 * @return     the databasemap
	 */
	public function getDatabaseMap()
	{
		return $this->dbMap;
	}

	/**
	 * The doBuild() method builds the DatabaseMap
	 *
	 * @return     void
	 * @throws     PropelException
	 */
	public function doBuild()
	{
		$this->dbMap = Propel::getDatabaseMap(NotesPeer::DATABASE_NAME);

		$tMap = $this->dbMap->addTable(NotesPeer::TABLE_NAME);
		$tMap->setPhpName('Notes');
		$tMap->setClassname('Notes');

		$tMap->setUseIdGenerator(true);

		$tMap->addPrimaryKey('UID', 'Uid', 'INTEGER', true, 11);

		$tMap->addColumn('NAME', 'Name', 'VARCHAR', true, 64);

		$tMap->addColumn('EMAIL', 'Email', 'VARCHAR', false, 128);

		$tMap->addColumn('URL0', 'Url0', 'VARCHAR', false, 128);

		$tMap->addColumn('URL1', 'Url1', 'VARCHAR', false, 128);

		$tMap->addColumn('CHILDREN', 'Children', 'VARCHAR', false, 256);

		$tMap->addColumn('NOTE', 'Note', 'LONGVARCHAR', true, null);

		$tMap->addColumn('MEDIAUID', 'Mediauid', 'INTEGER', false, 11);

		$tMap->addColumn('LANG', 'Lang', 'VARCHAR', true, 2);

		$tMap->addColumn('COUNTRY', 'Country', 'VARCHAR', true, 32);

		$tMap->addColumn('REGION', 'Region', 'VARCHAR', true, 32);

		$tMap->addColumn('BRIDGEUID', 'Bridgeuid', 'INTEGER', true, 11);

		$tMap->addColumn('DATE', 'Date', 'INTEGER', true, 11);

		$tMap->addColumn('PUBLISH', 'Publish', 'INTEGER', false, 1);

		$tMap->addColumn('KARMA', 'Karma', 'INTEGER', true, 11);

		$tMap->addColumn('HANDLE', 'Handle', 'VARCHAR', false, 64);

	} // doBuild()

} // NotesMapBuilder
