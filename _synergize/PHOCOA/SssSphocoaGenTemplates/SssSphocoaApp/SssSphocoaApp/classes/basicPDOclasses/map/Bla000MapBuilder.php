<?php


/**
 * This class adds structure of 'Bla000' table to 'lukecom' DatabaseMap object.
 *
 *
 *
 * These statically-built map classes are used by Propel to do runtime db structure discovery.
 * For example, the createSelectSql() method checks the type of a given column used in an
 * ORDER BY clause to know whether it needs to apply SQL to make the ORDER BY case-insensitive
 * (i.e. if it's a text column type).
 *
 * @package    basicPDOclasses.map
 */
class Bla000MapBuilder implements MapBuilder {

	/**
	 * The (dot-path) name of this class
	 */
	const CLASS_NAME = 'basicPDOclasses.map.Bla000MapBuilder';

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
		$this->dbMap = Propel::getDatabaseMap(Bla000Peer::DATABASE_NAME);

		$tMap = $this->dbMap->addTable(Bla000Peer::TABLE_NAME);
		$tMap->setPhpName('Bla000');
		$tMap->setClassname('Bla000');

		$tMap->setUseIdGenerator(false);

		$tMap->addPrimaryKey('UID', 'Uid', 'VARCHAR', true, 64);

		$tMap->addColumn('COMMENT', 'Comment', 'LONGVARCHAR', true, null);

		$tMap->addColumn('EN', 'En', 'LONGVARCHAR', false, null);

		$tMap->addColumn('DE', 'De', 'LONGVARCHAR', false, null);

		$tMap->addColumn('FR', 'Fr', 'LONGVARCHAR', false, null);

		$tMap->addColumn('IT', 'It', 'LONGVARCHAR', false, null);

		$tMap->addColumn('RM', 'Rm', 'LONGVARCHAR', false, null);

	} // doBuild()

} // Bla000MapBuilder
