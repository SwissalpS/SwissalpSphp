<?php
/*	CLASS		:	Console
	DESCRIPTION	:	The Console class is used to handle PHP debugging. The purpose is to remove display of error messages to an external view.
	ADDED		:	4.0
 */
class Console 
{
	private $debug_mode = 1; // flag to indicate if the system is in debug mode. 1 = yes, 0 = no
	private $items = array(); // the list of items to be logged
	private $name = '';
	private $title = '';
	
	/* 	FUNCTION	: 	Constructor
		DESCRIPTION	:	This is the constructor for the class.
		PARAMS		:	title [string] = The title of this console
		RESULT		:	
	 */
	function __construct($title)
	{
		$this->title = $$title;
		$this->name = strtolower(str_ireplace(array(' ','-'), '_', $title));
		
	}
	
	/* 	FUNCTION	: 	setDebugMode
		DESCRIPTION	:	Sets the console to indicate if it is in debug mode or not
		PARAMS		:	debug_mode [int] = The value to indicate the debug mode
		RESULT		:	
	 */
	function setDebugMode($debug_mode)
	{
		$this->debug_mode = $debug_mode;
	}
	
	/* 	FUNCTION	: 	printConsole
		DESCRIPTION	:	Displays the console in a separate window 
		PARAMS		:
		RESULT		:	
	 */
	function printConsole()
	{
		global $site_core;
		if($this->debug_mode)
		{
			
			print '<script type="text/javascript">';
				print "\n".'console_'.$this->name.' = window.open("","'.$this->name.'","menubar=no,width=360,height=250,toolbar=no,resizable=yes");';
			
				print "\n".'console_'.$this->name.'.document.write("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");';
				
				print "\n".'console_'.$this->name.'.document.write("<html><head><title>'.$this->title.'</title>");';
				print "\n".'console_'.$this->name.'.document.write("<link rel=\"stylesheet\" href=\"'.$site_core.'/Console/console.css\" /></head>")';
			
				print "\n".'console_'.$this->name.'.document.write("<table>");';
				print "\n".'console_'.$this->name.'.document.write("<tr><th class=\"num\">#</th><th class=\"comment\">Comment</th></tr>");';
				$num = 1;
				foreach($this->items as $log_item)
				{
					print "\n".'console_'.$this->name.'.document.write("<tr><td class=\"num\">'.$num.'</td><td class=\"comment\">'.$log_item.'</td></tr>");';
					$num++;
				}
				
				print "\n".'console_'.$this->name.'.document.write("</table>");';
				print "\n".'console_'.$this->name.'.document.write("</body></html>");';
				print "\n".'console_'.$this->name.'.document.close();';
			print '</script>';
			
		}
		
	}
	
	/* 	FUNCTION	: 	rec
		DESCRIPTION	:	Takes a string and saves it to the log
		PARAMS		: 	log_item [string] = The item to be logged. 
		RESULT		:	
	 */
	function rec($log_item)
	{
		$this->items[] = $log_item;
	}
}	
?>