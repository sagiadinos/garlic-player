# garlic-player - SMIL Player for Digital Signage

The goal is to create an as most as neccessary SMIL 3.0 compatible and as most as possible platform independent multimedia player to meet Digital Signage needs.
Garlic should be used online with web-based content managment systems like [SmilControl](http://smil-control.com) or other.
Offline usage via command-line will also possible.

## OS targets:

Primary: Linux, Windows
Secondary: Android, OSX and iOS

Developing with Qt 5.5 and QtAV 1.8.0 under Kubuntu 14.04

At the moment there is an early alpha version which is able to play local SMIL-Files with actual supported features via command-line.

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
Only sources are public. First binaries will deployed, when excl-element will supported and a downloader is implemented.

## actual supported SMIL-Features

###Time container elements
- **seq**
- **par**
- **excl** (is heavily @work)
- **priorityClass** partially supported

####Attributes for time container
peers stop|never

###Media-Elements
- **video**
- **audio**
- **images**
- **text** websites only

####Attributes for media
- **region**
- **fit** and the values fill, scale, meetbest, none

###Attributes for media and time container:
- **xml:id** with Fallback for SMIL 2.0 id
- **dur** with values indefinite, media, Clock values (full+partial clock),  normal (h min s and ms)
- **begin** with Clock values (like dur) and wallcock with periods and repeats. Date is supported only in full ISO 8601 format not short date like 201634W at the moment.
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

Contact: [Nikolaos Sagiadinos] (mailto:ns@smil-control.com)
