<?php


/**
 * This class adds structure of 'mediaMap' table to 'lukecom' DatabaseMap object.
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
class MediamapMapBuilder implements MapBuilder {

	/**
	 * The (dot-path) name of this class
	 */
	const CLASS_NAME = 'skyprom_phocoa.map.MediamapMapBuilder';

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
		$this->dbMap = Propel::getDatabaseMap(MediamapPeer::DATABASE_NAME);

		$tMap = $this->dbMap->addTable(MediamapPeer::TABLE_NAME);
		$tMap->setPhpName('Mediamap');
		$tMap->setClassname('Mediamap');

		$tMap->setUseIdGenerator(true);

		$tMap->addPrimaryKey('MEDIAUID', 'Mediauid', 'INTEGER', true, 11);

		$tMap->addColumn('URL', 'Url', 'VARCHAR', true, 256);

		$tMap->addColumn('TYPE', 'Type', 'VARCHAR', true, 32);

		$tMap->addColumn('WIDTH', 'Width', 'INTEGER', true, 7);

		$tMap->addColumn('HEIGHT', 'Height', 'INTEGER', true, 7);

		$tMap->addColumn('CSSCLASS', 'Cssclass', 'VARCHAR', true, 128);

		$tMap->addColumn('MIME', 'Mime', 'VARCHAR', true, 128);

		$tMap->addColumn('KARMA', 'Karma', 'INTEGER', true, 11);

		$tMap->addColumn('BIGWIDTH', 'Bigwidth', 'INTEGER', true, 7);

		$tMap->addColumn('BIGHEIGHT', 'Bigheight', 'INTEGER', true, 7);

		$tMap->addColumn('BIGURL', 'Bigurl', 'VARCHAR', true, 256);

		$tMap->addColumn('THUMBWIDTH', 'Thumbwidth', 'INTEGER', true, 7);

		$tMap->addColumn('THUMBHEIGHT', 'Thumbheight', 'INTEGER', true, 7);

		$tMap->addColumn('THUMBURL', 'Thumburl', 'VARCHAR', true, 256);

	} // doBuild()

} // MediamapMapBuilder
