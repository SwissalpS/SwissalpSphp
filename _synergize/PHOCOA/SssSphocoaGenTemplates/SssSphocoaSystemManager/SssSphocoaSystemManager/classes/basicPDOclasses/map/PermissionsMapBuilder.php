<?php


/**
 * This class adds structure of 'permissions' table to 'lukecom' DatabaseMap object.
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
class PermissionsMapBuilder implements MapBuilder {

	/**
	 * The (dot-path) name of this class
	 */
	const CLASS_NAME = 'basicPDOclasses.map.PermissionsMapBuilder';

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
		$this->dbMap = Propel::getDatabaseMap(PermissionsPeer::DATABASE_NAME);

		$tMap = $this->dbMap->addTable(PermissionsPeer::TABLE_NAME);
		$tMap->setPhpName('Permissions');
		$tMap->setClassname('Permissions');

		$tMap->setUseIdGenerator(true);

		$tMap->addPrimaryKey('UID', 'Uid', 'INTEGER', true, 11);

		$tMap->addColumn('DOMAIN', 'Domain', 'VARCHAR', true, 16);

		$tMap->addColumn('HASH', 'Hash', 'INTEGER', true, 128);

	} // doBuild()

} // PermissionsMapBuilder
