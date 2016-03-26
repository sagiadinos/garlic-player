# garlic-player - SMIL Player for Digital Signage

The goal is to create an as most as neccessary SMIL 3.0 and compatible and as most as possible platform independent multimedia player to meet Digital Signage needs.
Garlic should be used online with web-based content managment systems like [SmilControl](http://smil-control.com) or other.
Offline usage via command-line will also possible.

## OS targets:

Linux, Windows and maybe later OSX

Developing with Qt 5.5 and QtAV 1.8.0 under Kubuntu 14.04

At the moment there is an alpha which is able to play local and remote (http) SMIL-Files with actual supported features via command-line.

*garlic.sh [options] SMIL_INDEX*
```sh
Options:
-h, --help        Displays this help.
-v, --version     Displays version information.
-f, --fullscreen  Starts in fullscreen mode
-w, --window      Starts in windows mode

Arguments:
SMIL_INDEX        Path to SMIL index
```
Only sources! Binaries will deployed later.

## actual supported SMIL-Features

###Time container elements
- **seq**
- **par**
- **excl**
- **priorityClass**

####Attributes for excl/priorityClass time container
- **peers** stop|never|pause|defer
- **higher** stop|pause
- **lower** never|defer

###Media-Elements
- **video**
- **audio**
- **images**
- **text** websites only

###other Elements
- **prefetch**
- **meta** for refresh index via http-equiv="Refresh"

####Attributes for media
- **region**
- **fit** fill|scale|meetbest

###Attributes for media and time container:
- **xml:id** with Fallback for SMIL 2.0 id
- **title** stub
- **class** stub
- **xml:lang** stub
- **dur** with values indefinite, media, Clock values (full+partial clock),  normal (h min s and ms)
- **begin** with Clock values (like dur) and wallcock with periods and repeats. Date is supported only in full ISO 8601 format not short date like 201634W.
- **end** has same features as begin
- **repeatCount** only integer values (float support maybe implemented later)

###Layered multizone layouts elements:
- **root-layout**
- **layout**

###Multizone attributes
- **regionName**
- **top** values in px or %
- **left** values in px or %
- **width** values in px or %
- **height** values in px or %
- **z-index** integer values
- **backgroundColor** values are compatible with CSS

<!-- --> - HTML-like comments in SMIL-File are supported and content insite will ignored

## additional features supported not in w3c standard
- **shuffle play** in seq container via metadata

## actual supported remote features via downloader
- http downloads and caching
- caching of remote SMIL index file
- caching of remote media files
- refresh via meta tag in cyclic timer periods
- http 301 redirects for index and media

## ToDo's
- **expr-attribute** in media tags for conditional play as described in [a-smil.org](http://www.a-smil.org/index.php/Conditional_play) by [IAdea](http://www.iadea.com/)
- **logs and reports** as described in [a-smil.org reporting](http://www.a-smil.org/index.php/Reporting)
- **administration functions** as described in [a-smil.org Maintance tasks](http://www.a-smil.org/index.php/Maintenance_tasks)

Contact: [Nikolaos Sagiadinos] (mailto:ns@smil-control.com)
