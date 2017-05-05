# garlic-player - Open Source SMIL Player for Digital Signage

The goal is to create an as most as neccessary SMIL 3.0 and compatible and as most as possible platform independent free open source multimedia player to meet Digital Signage needs.
Garlic should be used online with web-based content managment systems like [SmilControl](http://smil-control.com) or other.
Offline usage via command-line is also possible.

## OS targets:

Linux, Windows, Raspberry Pi 3 maybe later OSX, Android

Developing with Qt 5.8.0, QtAV 1.11.0 (also not reconnomed QtMultimedia Support) and Quazip 0.7.3 for wgt (zipped html)

Garlic actually is able to play local and remote (via http) SMIL Files with actual supported features.
Compilations tested with Kde Neon, Debian 8 Jessie, Rasperian Jessie (rpi2) and Windows 7 i64 (VS2015 Community Version)

*garlic.sh [options] SMIL_INDEX*

Options:
-h, --help          Displays this help.
-v, --version       Displays version information.
-m, --windows-mode  fullscreen, bigscreen (when multi monitor) or windowed mode
-z, --windows-size  size of windows (when windows mode) e.g. 980x540 means 980px width and 540px height
-s, --screen-select Starts in selected screen

Arguments:
SMIL_INDEX        Path to SMIL index

Only sources! Binaries will deployed soon.

## Features
 - Support for multiple screens
 - Logging in file
 - Redirects
 - lot's of video formats 
 - Media Cache
 - SMIL3, HTML5 and HTML5-Widgets support

## actual supported SMIL-Features

### Time container elements
- **seq**
- **par**
- **excl**
- **priorityClass**

#### Attributes for excl/priorityClass time container
- **peers** stop|never|pause|defer
- **higher** stop|pause
- **lower** never|defer

### Media-Elements
- **video**
- **audio**
- **images**
- **text** websites only
- **wgt** 

### other Elements
- **prefetch**
- **meta** for refresh index via http-equiv="Refresh"

#### Attributes for media
- **region**
- **fit** fill|scale|meetbest

### Attributes for media and time container:
- **xml:id** with Fallback for SMIL 2.0 id
- **title** stub
- **class** stub
- **xml:lang** stub
- **dur** with values indefinite, media, Clock values (full+partial clock),  normal (h min s and ms)
- **begin** with Clock values (like dur) and wallcock with periods and repeats. Date is supported only in full ISO 8601 format not short date like 201634W.
- **end** has same features as begin
- **repeatCount** only integer values (float support maybe implemented later)

### Layered multizone layouts elements:
- **root-layout**
- **layout**

### Multizone attributes
- **regionName**
- **top** values in px or %
- **left** values in px or %
- **width** values in px or %
- **height** values in px or %
- **z-index** integer values
- **backgroundColor** values are compatible with CSS

<!-- -->  HTML-like comments in SMIL-File are supported and content inside will ignored

## additional features supported not in w3c standard
- **shuffle play** in seq container via metadata

## actual supported remote features via downloader
- http downloads and caching
- caching of remote SMIL index file
- caching of remote media files
- refresh via meta tag in cyclic timer periods
- http 301 redirects for index and media

## HotKeys
- toogle fullscreen with Ctrl-F
- toogle bigscreen with Ctrl-B
- quit app with Ctrl-Q
- open configuration dialog with Ctrl-C

## Actual Working
- deployment Linux and Windows for first binary release

## future ToDo's
- change config.xml during plaback
- **expr-attribute** in media tags for conditional play as described in [a-smil.org](http://www.a-smil.org/index.php/Conditional_play) by [IAdea](http://www.iadea.com/)
- **logs and reports** as described in [a-smil.org reporting](http://www.a-smil.org/index.php/Reporting)
- **administration functions** as described in [a-smil.org Maintance tasks](http://www.a-smil.org/index.php/Maintenance_tasks)

Contact: [Nikolaos Sagiadinos] (mailto:ns@smil-control.com)
