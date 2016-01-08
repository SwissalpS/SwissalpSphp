-- phpMyAdmin SQL Dump
-- version 3.3.8.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jan 08, 2011 at 03:41 AM
-- Server version: 5.1.54
-- PHP Version: 5.3.3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

-- --------------------------------------------------------

--
-- Table structure for table `Bla000`
--

CREATE TABLE IF NOT EXISTS `Bla000` (
  `uid` varchar(64) COLLATE utf8_unicode_ci NOT NULL,
  `comment` text COLLATE utf8_unicode_ci NOT NULL,
  `en` text COLLATE utf8_unicode_ci,
  `de` text COLLATE utf8_unicode_ci,
  `fr` text COLLATE utf8_unicode_ci,
  `it` text COLLATE utf8_unicode_ci,
  `rm` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Bla000`
--

INSERT INTO `Bla000` (`uid`, `comment`, `en`, `de`, `fr`, `it`, `rm`) VALUES
('Bla000Comment', 'Tabellen Titel: Kommentar', 'comment (This is for admin information only, will not be displayed to public)', 'Kommentar (Dieses Feld dient als ''Eselsbr&#252;cke''/''Gedankenst&#252;tze'' f&#252;r Admins und wird nicht der &#214;ffentlichkeit gezeigt)', 'Commentaire (seulement pour Admin)', 'Commento (solo Admin)', 'commentari (Admin Info only)'),
('Bla000LangEN', 'Tabellen Titel: English', 'English', 'Englisch', 'Anglais', 'Inglese', 'Englais'),
('Bla000LangDE', 'Tabellen Titel: Deutsch', 'German', 'Deutsch', 'Allemand', 'Tedesco', 'Tudestg'),
('Bla000LangFR', 'Tabellen Titel: Franz&#246;sisch', 'French', 'Franz&#246;sisch', 'Fran&#231;ais', 'Francese', 'Franzos'),
('Bla000LangIT', 'Tabellen Titel: Italienisch', 'Italian', 'Italienisch', 'Italien', 'Italiano', 'Talian'),
('Bla000LangRM', 'Tabellen Titel: Rumantsch', 'Rumantsch', 'Rumantsch', 'Rumantsch', 'Rumantsch', 'Rumantsch'),
('SysSavedSuccessfully', 'Antwort bei erfolgreichem Sichern eines Eintrags. Der Eintragstitel wird vor dran gef&#252;gt und ein Punkt angeh&#228;ngt', 'saved successfully', 'erfolgreich gespeichert', 'enregistr&#233; avec succ&#232;s', 'salvato con successo', 'accumular avair success'),
('PermsNoPermsFound', 'Admin-}Rechte-}Keine gefunden... ein Punkt angeh&#228;ngt', 'No Permissions found', 'Keine Rechte gefunden', '', '', ''),
('SharedEdit', 'bearbeiten, meist im Admin Bereich', 'Edit', 'Bearbeiten', '&#201;diter', 'Modifica', 'Lavurar'),
('SharedDelete', 'l&#246;schen, meist im Admin Bereich', 'Delete', 'L&#246;schen', 'Effacer', 'Cancellare', 'Stidar'),
('SharedSave', 'speichern, meist im Admin Bereich', 'Save', 'Speichern', 'Sauver', 'Salvare', 'Magasinar'),
('SharedSearch', 'suchen Knopf-Titel', 'Search', 'Suchen', 'Recherche', 'Cerca', 'Tschertgar'),
('SharedClear', 'Leeren Knopf-Titel', 'Clear', 'Leeren', 'Vide', 'Vuoto', 'Svidar'),
('PermsSuperUser', 'admin Bereich checkboxen', 'Super User', 'Super Benutzer', 'Super Utilisateur', 'Super Utente', 'Super Ben&#252;tzer'),
('PermsMayWrite', 'admin Bereich checkboxen', 'May Write', 'Darf Schreiben', 'Peut &#233;crire', 'Pu&#242; scrivere', 'Pudair scriver'),
('PermsMayAdministrate', 'admin Bereich checkboxen', 'May Administrate', 'Darf Verwalten', 'Peut administrer', 'In grado di amministrare', 'Pudair administrar'),
('PermsMayRead', 'admin Bereich checkboxen', 'May Read', 'Darf Lesen', 'Autoris&#233; &#224; lire', 'Permesso di leggere', 'Pudair leger'),
('SharedCreate', '', 'Create', 'Erstellen', 'Dresser', 'Redigere', 'Eriger'),
('AdminPermsDomain', 'Admin->Rechte', 'Domain', 'Dom&#228;ne', 'Domaine', 'Dominio', 'Domena'),
('AdminPermsHash', 'Admin-}Rechte', 'Hash', 'Hash', 'Hachage', 'Hash', 'Hash'),
('SharedShowing', 'Seitenz&#228;hler/-Navigator', 'Showing', 'Zeige', 'Pr&#233;senter', 'Proiezione', 'Mussa'),
('PermsPlur', 'mehrzahl von Recht', 'Permissions', 'Rechte', 'Droits', 'Diritti', 'Dretgs'),
('SharedOf', 'zeige seiten x - y VON z', 'of', 'von', 'de', 'di', 'da'),
('PermsSing', 'einzahl von Recht', 'Permission', 'Recht', 'Droit', 'Diritto', 'Dretg'),
('PermDeleteSuccess', 'Bei Erfolgreichem l&#246;schen von Rechten', 'Permission successfully deleted.', 'Recht erfolgreich gel&#246;t.', '', '', ''),
('BlaPlur', '&#252;bersetzbare texte', 'Localizable Strings', 'Lokalisierbare Texte', 'Textes localisable', 'Testi localizzabili', 'Texts translatabel'),
('BlogPlur', 'blog eintr&#228;ge', 'Blog Entries', 'Blog Eintr&#228;ge', 'Entr&#233;es de blog', 'Accessi al blog', 'Blog inscripziuns'),
('PPermsPlur', 'Rechte Sammlungen', 'Permission Presets', 'Rechte Sammlungen', 'Collections des droits', 'Diritti Collezioni', 'Dretg collecziun'),
('NotePlur', 'mehrzahl von Benutzerbeitrag als titel gedacht', 'User Submissions', 'Benutzerbeitr&#228;ge', 'Soumissions de l''utilisateur', 'Utente Argomenti', 'Contribuziuns da utilisaders'),
('UsersPlur', 'mehrzahl von Benutzer als titel gedacht', 'Users', 'Benutzer', 'Utilisateurs', 'Utenti', 'Utilisaders'),
('MenuLang', 'Menutitel Sprachen', 'Languages', 'Sprachen', 'Langues', 'Lingue', 'Linguas'),
('MenuAdmin', 'Menutitel Admin', 'Admin', 'Administration', 'Administration', 'Amministrazione', 'Administraziun'),
('SharedLogout', 'logout', 'Logout', 'Abmelden', 'D&#233;connexion', 'Logout', 'Partir'),
('SharedLogin', 'login', 'Login', 'Anmelden', 'S''inscrire', 'Registro', 'Acceder'),
('SharedAddNew', 'admin neuen eintrag wird gefolgt vom Titel des Eintrags(art)', 'Add a new', 'Erstelle neuer(-s)', '', '', ''),
('SharedEnterPartial', 'gefolgt vom "Spaltentitel"', 'Enter partial', 'Suche nach', '', '', ''),
('AdminPPermsName', 'test entry from iPhone', 'name', 'Name', 'nom', '', ''),
('UsersSing', '', 'User', 'Benutzer/-in', 'Utilisateur', 'Utente', 'Utilisader/-dra'),
('SharedDetail', '', 'detail', 'Detail', 'D&#233;tail', 'Dettaglio', 'Detagl'),
('AdminUsersHandle', 'anmeldename', 'Handle', 'Alias', 'Alias', 'Alias', 'Alias'),
('LoginLoginMsg', '', 'You must log in to access the requested page.<br />(guest guest)', 'Du musst dich anmelden, um die angeforderte Seite zuzugreifen.<br />', 'Vous devez vous connecter pour acc&#233;der &#224; la page demand&#233;e.<br />(guest guest)', '&#200; necessario accedere per accedere alla pagina richiesta.<br />(guest guest)', 'annunziar<br />(guest guest)'),
('AdminUsersListNoUsersFound', '', 'No Users found', 'Keine Benutzer gefunden', '', '', ''),
('SharedJumpTo', '', 'Jump to', 'Wechseln zu', 'Aller &#224;', 'Vai a', 'Mida a'),
('LoginInvalid', '', 'Username or password is not valid.', 'Benutzername oder Kennwort ist ung&#252;ltig.', 'Nom d''utilisateur ou mot de passe n''est pas valide.', 'Nome utente o password non &#232; valida.', 'Nunvalid num d''utilisader u pled-clav.'),
('LoginNotImplemented', '', 'not yet implemented', 'noch nicht implementiert', 'pas encore mis en &#156;uvre', 'non ancora attuata', ''),
('SharedPassword', '', 'Password', 'Passwort', 'Mot de passe', 'Parola d''ordine', 'Pled-clav'),
('SharedPrevious', '', 'Previous', 'Vorherige', 'Pr&#233;dent', 'Precedente', 'Precedenta'),
('SharedNext', '', 'Next', 'N&#228;chsten', 'Prochain', 'Prossimo', 'Proxim'),
('SharedNew', '', 'New', 'Neu', 'Nouveau', 'Nuovo', 'Da nov'),
('BlaSing', '', 'Localizable String', 'Lokalisierbarer Text', 'Texte localisable', 'Testo localizzabile', 'Text translatabel'),
('AdminUsersLangOrder', 'welche Sprache soll verwendet werden falls die gew&#252;nschte nicht zur Verf&#252;gung steht', 'Language preference order', 'Sprach pr&#228;ferenz Reihenfolge', 'ordre de pr&#233;f&#233;rence la langue', 'Lingua di preferenza per', ''),
('SharedName', '', 'Name', 'Name', 'Nom', 'Nome', 'Num'),
('SharedMakeLocalizable', '', 'make localizeable', 'lokalisierbar machen', 'fair localisable', '', ''),
('SharedCancel', '', 'Cancel', 'Abbrechen', 'Annuler', 'Cancellare', 'Chala'),
('MetaDescription', '', 'SwissalpS PHOCOA application', 'SwissalpS PHOCOA Program', 'SwissalpS PHOCOA application', 'SwissalpS PHOCOA', 'SwissalpS PHOCOA'),
('MetaKeywords', '<p>&#160;Die meta tag Schl&#252;sselw&#246;rter durch Komma getrennt</p>', 'SwissalpS, PHOCOA, Webapp, YUI', 'SwissalpS, PHOCOA, Webapp, YUI', 'SwissalpS, PHOCOA, Webapp, YUI', 'SwissalpS, PHOCOA, Webapp, YUI', 'SwissalpS, PHOCOA, Webapp, YUI'),
('Bla000ScratchHTML', '', 'HTML Scratchpad, copy source to above fields. (won''t be saved down here)', 'Zum HTML kreieren, ''source'' kopieren und oben einf&#252;gen. (wird hier unten nicht gespeichert)', 'Pour faire de HTML pour les champs en haut.', '', ''),
('Bla000Deleted', '', 'Successfully deleted localizable string', 'Lokalisierbarer Text wurde erfolgreich gel&#246;scht', 'Texte localisables &#233;t&#233; supprim&#233; avec succ&#232;s', 'Localizable testo &#232; stato eliminato con successo', ''),
('MainMediaGallery', '', 'Media Gallery', 'Medien Galerie', 'Galerie des m&#233;dias', 'Media Gallery', 'Galaria da medias'),
('SysDateFormat', 'PHP Zeitformattierungszeichenkette um Datum der gew&#252;en Sprache anzupassen. Als Referenz: http://ch2.php.net/manual/en/function.date.php', 'm/d/Y (\\U\\S \\f\\o\\r\\m\\a\\t)', 'd.m.Y', 'd-m-Y', 'd/m/Y', 'd.m.Y'),
('ErrorDate', '', 'Please use ''dd.mm.(yy)yy'' format for date.', 'Bitte das Datum im Format: ''dd.mm.(yy)yy'' eingeben.', '''dd.mm.(yy)yy''', '''dd.mm.(yy)yy''', '''dd.mm.(yy)yy'''),
('SharedEditLocalized', '', 'Edit localized', 'lokalisiert bearbeiten', '', '', ''),
('GlobalDisclaimer', '', '', '&#169; example.com<br /><br />\r\n\r\nAlle Rechte vorbehalten. info@example.com<br /><br />\r\n\r\nTexte, Bilder, Grafiken sowie Layout dieser Webpr&#228;senz unterliegen weltweitem Urheberrecht. Unerlaubte Verwendung, Reproduktion oder Weitergabe einzelner Inhalte oder kompletter Seiten k&#246;nnen sowohl straf- als auch zivilrechtlich verfolgt werden. Links (auch so genannte &#147;Deep Links&#148;) auf diese Seiten sind ausdr&#252;cklich erw&#252;nscht, jedoch nur in einem eigenen Fenster, nicht im Frameset/iframe eines anderen Angebots. Direkte Links auf einzelne Elemente (zum Beispiel Bilder) sind jedoch nicht gestattet. Wenn sie Inhalte f&#252;r anderen als zum privaten, pers&#246;nlichen Gebrauch verwenden m&#246;chten, fragen Sie uns bitte.<br /><br />\r\n\r\n<b>Hinweise zum Datenschutz</b>\r\nBeim Aufruf dieser Webseiten werden vom Server automatisch Protokolle geschrieben. Diese lassen jedoch keine R&#252;ckschl&#252;sse auf Ihre Identit&#228;t zu und dienen lediglich zur Analyse der Nutzungsh&#228;ufigkeit dieser Seiten. Wenn Sie uns Ihre pers&#246;nlichen Daten (zum Beispiel &#252;ber ein Formular oder durch eine E-Mail) mitteilen, verwenden wir diese nur f&#252;r direkte Kommunikation mit Ihnen. Sie erhalten ausser den Antworten auf Ihre konkreten Anfragen keine weiteren E-Mails von uns, wenn Sie diese nicht ausdr&#252;cklich anfordern. Wir geben Ihre Daten keinesfalls an Dritte weiter. Stand August 2010', '', '', ''),
('NotesNoneFound', '', 'No notes found', 'Keine Benutzerbeitr&#228;ge gefunden', '', '', ''),
('NoteAddNew', '', 'Add a Note', 'Einen Benutzerbeitrag erstellen', '', '', ''),
('SharedLanguage', '', 'Language', 'Sprache', 'Langue', 'Lingua', 'Lingua'),
('NoteSing', '', 'User Submission', 'Benutzerbeitrag', 'Contribution de l''utilisateur', 'Utente contributo', 'Contribuziun da utilisader'),
('SharedURL', '', 'Homepage', 'Startseite', 'Page d''accueil', 'Homepage', 'Homepage'),
('SharedEmail', '', 'Email', 'E-Mail', 'Email', 'E-mail', 'E-mail'),
('NoteNoteMT', '', 'Please write a comment.', 'Bitte etwas schreiben.', 'S''il vous pla&#238;t &#233;crivez quelque chose.', 'Prega di scrivere qualcosa.', 'Prega di scrivere qualcosa.'),
('NoteInvalidEmail', '', 'Please enter a ''valid'' email address or none at all. If you are sure that the one you provided is correctly typed, please try with another one or email us about your problem.', 'Bitte geben Sie eine "g&#252;ltige" E-Mail-Adresse oder gar keine. Wenn Sie sicher sind, dass Sie richtig getippt haben, versuchen Sie es bitte mit einer anderen oder mailen Sie uns &#252;ber Ihr Problem.', 'S''il vous pla&#238;t entrer une adresse d''email ''valide'' ou pas du tout. Si vous &#234;tes s&#251;r que celui que vous avez fourni est correctement saisi, s''il vous pla&#238;t essayer avec un autre ou nous envoyer un courriel au sujet de votre probl&#232;me.', 'Si prega di inserire un indirizzo email ''valido'' o del tutto assenti. Se sei sicuro che quello che hai fornito &#232; digitato correttamente, provare con un altro o via e-mail del tuo problema.', ''),
('NoteCopyrightUL', '', 'By uploading a media file, you give us the permission to display it on our websites. Please only upload or link to data to which you either own the copyright or whose copyright explicitly allows you to allow us to use it.', 'Durch das Hochladen einer Mediendatei, geben Sie uns die Erlaubnis, es auf unseren Webseiten anzuzeigen. Bitte nur Dateien und Links hochladen/angeben, zu denen Sie entweder das Urheberrecht besitzen oder deren Urheberrechte Ihnen ausdr&#252;cklich erlauben, uns zu erlauben, diese zu benutzen.', 'En t&#233;l&#233;chargeant un fichier multim&#233;dia, vous nous donnez la permission de l''afficher sur nos sites Web. S''il vous pla&#238;t ne t&#233;l&#233;charger ou lien vers les donn&#233;es &#224; laquelle vous soit propre le droit d''auteur ou dont les droits d''auteur autorise explicitement que vous pour nous permettre de l''utiliser.', 'Con il caricamento di un file multimediale, ci dai il permesso di visualizzare sul nostro sito web. Si prega di caricare solo o link a dati a cui si sia possiede il copyright oi cui diritti d''autore consente esplicitamente di permetterci di usarlo.', ''),
('SharedUploadMedia', '', 'Upload an image, audio, or video file', 'Lade eine Bild-, Audio- oder Videodatei hoch', 'T&#233;l&#233;charger une image, un fichier audio ou vid&#233;o', 'Caricare una foto, file audio o video', ''),
('NoteMandatory', '', 'This field is mandatory', 'Dieses Feld ist obligatorisch', 'Ce champ est obligatoire', 'Questo campo &#232; obbligatorio', ''),
('NoteInvalidUrl0', '', 'Please provide a valid url or none at all.', 'Bitte geben Sie eine g&#252;ltige URL oder &#252;berhaupt keine.', 'S''il vous pla&#238;t fournir une URL valide ou pas du tout.', 'Si prega di fornire un URL valido o del tutto assenti.', ''),
('NotesListBy', '', 'by', 'von', 'par', 'per', 'per'),
('AdminNotesBeCarefull', '', 'Please be careful with the fields below. They are mostly parsed by script. Change at your own risk!', 'Die folgenden Felder werden meist dynamish gesetzt. Bitte mit Vorsicht ver&#228;ndern.', '', '', ''),
('LikeThisBadgeDirectLink', '', 'direct link to this post', 'direkter Link zu diesem Beitrag', '', '', ''),
('LikeThisBadgeDelicious', '', 'add to del.icio.us', 'del.icio.us', 'del.icio.us', 'del.icio.us', 'del.icio.us'),
('LikeThisBadgeReddit', '', 'reddit', '', '', '', ''),
('LikeThisBadgeFurl', '', 'furl', '', '', '', ''),
('LikeThisBadgeDigg', '', 'digg', '', '', '', ''),
('SharedList', '', 'list', 'Liste', 'liste', '', ''),
('NotesList', '', 'User Submissions', 'Besucherbeitr&#228;ge', 'Soumissions de l''utilisateur', 'Utente Argomenti', 'Contribuziuns da utilisaders'),
('LikeThisBadgeMail', '', 'send this link via email to somebody', 'diesen Link mittels email versenden', 'email', 'email', 'email'),
('LikeThisBadgeTwitter', '', 'twitter this link', 'diesen Link twittern', 'twitter', 'twitter', 'twitter'),
('LikeThisBadgeBlinklist', '', 'blinklist', 'blinklist', 'blinklist', 'blinklist', 'blinklist'),
('LikeThisBadgeTechnorati', '', 'technorati', '', '', '', ''),
('NotesReadRest', '', 'read all', 'rest lesen', 'lire rest', 'leggi il resto', ''),
('NoteThankYou', '', 'Thank you for your post', 'Vielen Dank f&#252;r Ihren Beitrag', 'Merci pour votre message', 'Grazie per il tuo post', ''),
('HelpNoteNewName', '', 'Your name, will be shown in public.', 'Ihr Name, wird &#246;ffentlich gezeigt.', 'Votre nom, sera montr&#233; en public.', 'Il tuo nome, verr&#224; mostrato in pubblico.', ''),
('NoteThankYouExplanation', '', 'After screening, your post will appear.<br />This may take a few minutes.', 'Nach dem Screening, wird Ihr Beitrag erscheinen.<br />Dies kann einige Minuten dauern.', 'Apr&#232;s s&#233;lection, votre message s''affichera.<br />Cela peut prendre quelques minutes.', 'Dopo la proiezione, il tuo post verr&#224; visualizzato.<br />Questa operazione potrebbe richiedere alcuni minuti.', ''),
('NoteThanksLinkText', '', 'This is the direct link to your post.', 'Dies ist der direkte Link zu Ihrem Beitrag.', 'C''est le lien direct &#224; votre message.', 'Questo &#232; il link diretto al tuo post.', ''),
('HelpNoteNothing', '', 'No help available.', 'Keine Hilfe verf&#252;gbar.', 'Aucune aide disponible.', 'Nessun aiuto disponibile.', ''),
('HelpNoteNewEmail', ' (Enables visitors to send you messages without actually sharing your address)', 'Your email address. It will not be shown in public. SkyPromenade keeps it to itself.', 'Ihre E-Mail-Adresse. Sie wird nicht &#246;ffentlich gezeigt.', 'Votre adresse e-mail. Ne sera pas montr&#233; publiquement.', 'Il tuo indirizzo e-mail. Non ti verr&#224; mostrato pubblicamente.', ''),
('HelpNoteNewUrl0', '', 'URL of a website you run or promote. SkyPromenade will delete the full post if the URL points to content not thought suitable to the idea of this website. (e.g. adult sites)', 'URL einer Website, die Sie betreiben oder f&#246;rdern. SkyPromenade l&#246;scht den ganzen Beitrag, wenn die URL auf unsittliche oder zwecks-entfremdende Inhalte zeigt.', '', '', ''),
('HelpNoteNewLanguage', '', 'The language your note is written in.', 'Die Sprache, in welcher Ihre Notiz geschrieben ist.', 'La langue, votre note est &#233;crite en.', 'La lingua, la nota &#232; scritta.', ''),
('HelpNoteNewCountry', '', 'Your country of origin.', 'Ihr Land.', 'Votre pays.', 'Il vostro paese.', ''),
('HelpNoteNewRegion', '', 'Your region. State, town, mountain... whatever you like to call the place you come from.', 'Ihrer Region. Stadt, Gegend, Quartier, Berg ... wie auch immer Sie gerne Ihre Heimat nennen.', 'Votre r&#233;gion. Ville, montagne ... ce que vous voulez appeler l''endroit d''o&#249; vous venez.', 'La tua regione. Stato, citt&#224;, montagna ... comunque lo si voglia chiamare il luogo di provenienza.', ''),
('HelpNoteNewNote', '', 'Your message to the world. Tell us about your experience on/around a suspension bridge.', 'Ihre Botschaft an die Welt. Erz&#228;hlen Sie uns von Ihren Erfahrungen auf / um eine H&#228;ngebr&#252;cke.', 'Votre message au monde. Parlez-nous de votre exp&#233;rience sur / autour d''un pont suspendu.', 'Il tuo messaggio al mondo. Racconta la tua esperienza su / intorno a un ponte sospeso.', ''),
('HelpNoteNewUpload', '', 'You may upload a media file to enhance your message. This may be an image, short movie or even an audio recording. Please do not upload anything to which you do not own the rights.', 'Sie d&#252;eine Mediendatei hochladen um Ihre Mitteilung zu unterstreichen. Dies darf ein Bild, kurzes Video oder eine Tonaufnahme sein. Hauptsache die Rechte dies zu tun sind in Ihrem besitz.', 'Vous pouvez t&#233;charger un fichier multim&#233;a pour am&#233;orer votre message. Ce peut &#234;e une image, court-m&#233;age ou m&#234; un enregistrement audio. S''il vous pla&#238;t&#233;charger seulement si vous poss&#233;z les droits.', 'Si pu&#242;ricare un file multimediale per migliorare il tuo messaggio. Questo pu&#242;sere un''immagine, un filmato breve o anche una registrazione audio. Per favore non caricare nulla con cui tu non possiedi i diritti.', ''),
('NoteToBridge', '', 'to bridge:', 'zu Br&#252;cke:', '&#224; pont:', 'per ponte:', ''),
('SharedFirst', '', 'First', 'Erste', 'Premi&#232;re', 'Prima', ''),
('ImpressumIdea', '', 'Idea: Producer Name', 'Idee: Name des Produzenten', 'Id&#233;e: ...', 'Idea: ...', 'Idea: ...'),
('ImpressumOptics', '', 'Optical consulting:<a href="http://example.com/" target="_blank" >optics: Mr G</a>', 'Optische Beratung:<a href="http://example.com/" target="_blank" >optics: Mr G</a>', 'Consultation optique:<a href="http://example.com/" target="_blank" >optics: Mr G</a>', 'Ottica di consulenza:<a href="http://example.com/" target="_blank" >optics: Mr G</a>', ''),
('ImpressumFunction', '', 'Function: Luke Zimmermann aka SwissalpS', 'Funktion: Luke Zimmermann aka SwissalpS', 'Fonction: Luke Zimmermann aka SwissalpS', 'Funzione: Luke Zimmermann aka SwissalpS', ''),
('SharedContinue', '', 'Continue', 'Weiter', 'Continuer', 'Continua', ''),
('HelpNoteAllInOne', '', '<b>Name</b>: Your name, will be shown in public.<br />\r\n<b>Email</b>: Your email address. It will not be shown in public. SkyPromenade keeps it to itself.<br />\r\n<b>Homepage</b>: URL of a website you run or promote. SkyPromenade will delete the full post if the URL points to content not thought suitable to the idea of this website. (e.g. adult sites)<br />\r\n<b>Language</b>: The language your note is written in.<br />\r\n<b>Country</b>: Your country of origin.<br />\r\n<b>Region</b>: Your region. State, town, mountain... whatever you like to call the place you come from.<br />\r\n<b>User Submission</b>: Your message to the world. Tell us about your experience on/around a suspension bridge.<br />\r\n<b></b>: You may upload a media file to enhance your message. This may be an image, short movie or even an audio recording. Please do not upload anything to which you do not own the rights.', '<b>Name</b>: Ihr Name, wird &#246;ffentlich gezeigt.<br />\r\n<b>E-Mail</b>: Ihre E-Mail-Adresse. Sie wird nicht &#246;ffentlich gezeigt.<br />\r\n<b>Startseite</b>: URL einer Website, die Sie betreiben oder f&#246;rdern. SkyPromenade l&#246;scht den ganzen Beitrag, wenn die URL auf unsittliche oder zwecks-entfremdende Inhalte zeigt.<br />\r\n<b>Sprache</b>: Die Sprache, in welcher Ihre Notiz geschrieben ist.<br />\r\n<b>Land</b>: Ihr Land.<br />\r\n<b>Region</b>: Ihrer Region. Stadt, Gegend, Quartier, Berg ... wie auch immer Sie gerne Ihre Heimat nennen.<br />\r\n<b>Benutzerbeitrag</b>: Ihre Botschaft an die Welt. Erz&#228;hlen Sie uns von Ihren Erfahrungen auf / um eine H&#228;ngebr&#252;cke.<br />\r\n<b></b>: Sie d&#252;rfen eine Mediendatei hochladen um Ihre Mitteilung zu unterstreichen. Dies darf ein Bild, kurzes Video oder eine Tonaufnahme sein. Hauptsache die Rechte dies zu tun sind in Ihrem besitz.', '<b>Nom</b>: Votre nom, sera montr&#233; en public.<br />\r\n<b>Email</b>: Votre adresse e-mail. Ne sera pas montr&#233; publiquement.<br />\r\n<b>Page d''accueil</b>: url<br />\r\n<b>Langue</b>: La langue, votre note est &#233;crite en.<br />\r\n<b>&#201;tat</b>: Votre pays.<br />\r\n<b>R&#233;gion</b>: Votre r&#233;gion. Ville, montagne ... ce que vous voulez appeler l''endroit d''o&#249; vous venez.<br />\r\n<b>Contribution de l''utilisateur</b>: Votre message au monde. Parlez-nous de votre exp&#233;rience sur / autour d''un pont suspendu.<br />\r\n<b></b>: Vous pouvez t&#233;charger un fichier multim&#233;a pour am&#233;orer votre message. Ce peut &#234;e une image, court-m&#233;age ou m&#234; un enregistrement audio. S''il vous pla&#238;t&#233;charger seulement si vous poss&#233;z les droits.', '<b>Nome</b>: Il tuo nome, verr&#224; mostrato in pubblico.<br />\r\n<b>E-mail</b>: Il tuo indirizzo e-mail. Non ti verr&#224; mostrato pubblicamente.<br />\r\n<b>Homepage</b>: url<br />\r\n<b>Lingua</b>: La lingua, la nota &#232; scritta.<br />\r\n<b>Paese</b>: Il vostro paese.<br />\r\n<b>Regione</b>: La tua regione. Stato, citt&#224;, montagna ... comunque lo si voglia chiamare il luogo di provenienza.<br />\r\n<b>Utente contributo</b>: Il tuo messaggio al mondo. Racconta la tua esperienza su / intorno a un ponte sospeso.<br />\r\n<b></b>: Si pu&#242;ricare un file multimediale per migliorare il tuo messaggio. Questo pu&#242;sere un''immagine, un filmato breve o anche una registrazione audio. Per favore non caricare nulla con cui tu non possiedi i diritti.', ''),
('SharedSaveing', '', 'work in progress... please wait', 'Arbeite... Bitte warten', 'travaux en cours ... patientez s''il vous pla&#238;t', 'lavori in corso ... si prega di attendere', ''),
('NotesDeleteSure', '', 'Are you sure you want to delete this note including all attachments and children?', '', '', '', ''),
('SharedLast', '', 'last', 'letzte', 'derni&#232;re', 'ultimo', ''),
('AdminUsersPassRepeat', '', 'Password repeated', 'Passwort best&#228;tigt', 'Encore mot de passe', 'Ancora parola d''ordine', 'Pled-clav'),
('AdminPanelBlaTurnOff', '', 'Stop editing localizable texts', 'Fertig mit bearbeiten von lokalisierbaren Texten', '', '', ''),
('AdminPanelBlaTurnOn', '', 'Start editing localizable texts', 'Zeige lokalisierbare Texte damit ich sie bearbeiten kann', '', '', ''),
('AdminPanelTitle', '', 'Admin Quick Panel', '<b>Admin Schnell Steuerung</b>', '', '', ''),
('AdminPanelBlaTurnOnTitle', '', 'Clicking this will enable you to edit localizable portions of the webpage.', 'Hiermit werden die lokalisierbaren Texte sichtbar gemacht. Per Klick wird die &#220;bersetzungsmaske in einem neuen Fenster ge&#246;ffnet.', '', '', ''),
('AdminPanelBlaTurnOffTitle', '', 'Clear hotspots for localizable portions of the webpage', 'L&#228;sst die lokalisierbaren Teile der webseite wieder unempfindlich f&#252;r den Mauszeiger werden.', '', '', '');
