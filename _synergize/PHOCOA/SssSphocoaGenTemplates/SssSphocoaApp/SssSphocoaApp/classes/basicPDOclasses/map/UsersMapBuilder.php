<?php


/**
 * This class adds structure of 'Users' table to 'lukecom' DatabaseMap object.
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
class UsersMapBuilder implements MapBuilder {

	/**
	 * The (dot-path) name of this class
	 */
	const CLASS_NAME = 'skyprom_phocoa.map.UsersMapBuilder';

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
		$this->dbMap = Propel::getDatabaseMap(UsersPeer::DATABASE_NAME);

		$tMap = $this->dbMap->addTable(UsersPeer::TABLE_NAME);
		$tMap->setPhpName('Users');
		$tMap->setClassname('Users');

		$tMap->setUseIdGenerator(false);

		$tMap->addPrimaryKey('HANDLE', 'Handle', 'VARCHAR', true, 64);

		$tMap->addColumn('PERMISSIONS', 'Permissions', 'VARCHAR', true, 256);

		$tMap->addColumn('PASSHASH', 'Passhash', 'VARCHAR', true, 255);

		$tMap->addColumn('OAUTH', 'Oauth', 'VARCHAR', false, 256);

		$tMap->addColumn('OPENID', 'Openid', 'VARCHAR', false, 256);

		$tMap->addColumn('EMAIL', 'Email', 'VARCHAR', true, 128);

		$tMap->addColumn('REALNAME', 'Realname', 'VARCHAR', true, 64);

		$tMap->addColumn('UID', 'Uid', 'INTEGER', true, 11);

		$tMap->addColumn('URL', 'Url', 'VARCHAR', false, 256);

		$tMap->addColumn('AVATARMEDIAUID', 'Avatarmediauid', 'INTEGER', true, 11);

		$tMap->addColumn('LANGUAGEORDER', 'Languageorder', 'VARCHAR', true, 16);

		$tMap->addColumn('COUNTRY', 'Country', 'VARCHAR', false, 32);

		$tMap->addColumn('REGION', 'Region', 'VARCHAR', false, 32);

		$tMap->addColumn('KARMA', 'Karma', 'INTEGER', true, 11);

	} // doBuild()

} // UsersMapBuilder
