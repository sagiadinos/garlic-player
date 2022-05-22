# garlic-player - Open Source SMIL Player for Digital Signage

The garlic-player is a SMIL 3.0 compatible [Digital Signage Player](https://smil-control.com/magazine/what-is-a-digital-signage-player/).

The goal for the development is to create a most as neccessary SMIL 3.0 compatible and as most as possible platform independent free open source multimedia player to meet the needs of the digital signage industry. Currently garlic-player has reached a high level of compatibility and stability. So it is used in many projects and some android hardware solutions.

[Digital Signage](https://smil-control.com/) is a relatively young business and it has a lack of standards. As customer you can easily trapped into a vendor-lock. A standard like SMIL gives you independence back.

The garlic-player can be used offline and online.

Based on [SMIL standard](https://www.w3.org/TR/SMIL3/) it works together with every compatible [Digital Signage Software](https://smil-control.com/software/).

You are able to play local and remote (via http) SMIL files with the features described below.

[Download compiled binaries](https://garlic-player.com/garlic-player/)

## Libraries used
 - [Qt5](https://www.qt.io) >= 5.10 (5.12.x LTS recommended)
 - [QtAV](http://www.qtav.org) >= 1.13.0 as alternative to QtMultimedia
 - [Quazip](http://quazip.sourceforge.net) 0.7.3 (included in ./src/ext)
 - [zlib](https://zlib.net) 1.2.11 (included in ./src/ext)

## Current Operating System Supported
 - Linux
 - Windows
 - Android
 - macOS
 
## Successful tested with:
 - Linux 64Bit: Kde Neon, Kubuntu, Debian, Arch64
 - Raspberry Pi: 3 and 4 for 32 and 64Bit
 - Tinker Board: (Linaro OS 1.8)
 - Android Armv7 (4.4.1, 5.0.1, 5.1.1, 6.0.2, 7.1, 9 and 10)
 - Windows 64Bit: 7, 8 and 10
 - MacOS 64Bit (Intel): High Sierra, Mojave, Catalina and Big Sur 
 - MacOS M1: Big Sur (via Rosetta 2)
 - iOS 14.4.2 (experimental)

For Android and iOS player-c2qml is needed, cause only QML Multimedia is supported on this operating systems.

*garlic.sh [options] SMIL_INDEX*

## Command line options
 - -h, --help          Displays this help.
 - -v, --version       Displays version information.
 - -c, --config		   path to a config.xml
 - -m, --windows-mode  fullscreen, bigscreen (when multi monitor) or windowed mode
 - -z, --windows-size  size of windows (when windows mode) e.g. 980x540 means 980px width and 540px height
 - -s, --screen-select Starts in selected screen number

Arguments:
SMIL_INDEX        Path to SMIL index

Binaries can be downloaded at [garlic-player.com](https://garlic-player.com). You will find stable releases and nightly build there.

## Features (master trunk)
 - Support for multiple screens (not on Android)
 - multiple and overlapping zones e.g. splitscreens
 - logging
 - resume downloads
 - 301 redirect support
 - lot's of video formats 
 - supported SVG 1.2. Tiny Profile, Jpg, Png and Gif
 - media cache
 - SMIL3 subset, HTML5, HTML5-Widgets and (depending on device) WebGL support
 - WebWidgets with also support parameters
 - Webserver on port 8080 for Restful Api

## Currently supported SMIL-Features

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
- **brush**
- **html/text**
- **HTML Widgets**

### Other Elements
- **prefetch**
- **meta** for refresh index via http-equiv="Refresh"

#### Attributes for par and excl
- **endsync** first|last|id-value

#### Attributes for media
- **region**
- **fit** fill|meet|meetbest

#### Attributes for mbrush
- **color** color in CSS Stsyle

### Attributes for media and container (Timings and Synchronization)
- **xml:id** with Fallback for SMIL 2.0 id
- **title** stub
- **restart** (always|never|whenNotActive|default)
- **class** stub
- **xml:lang** stub
- **repeatCount** (currently only positive integer values)

#### Begin and End Attributes
- **Clock values* like dur
- **Wallclock** with weekday (-/+) periods and repeats. Date is supported in full ISO 8601 format not short date like 201634W.
- **Events** beginEvent, endEvent, activateEvent with additional clock value like id.endEvent+2s
- **Syncbase values** with additional clock value like id.begin+2s
- **accesskey** with additional clock value like accesskey('w')+2s
- **indefinite**
- **value lists** seperated by ";"

#### Timings with Dur
- **indefinite**
- **media**
- **clock values** (full+partial clock),  normal (h min s and ms)

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
- **backgroundColor** values are compatible with CSS style
- **backgroundImage** png and jpg images (not in android player)
- **backgroundRepeat**  values: repeat | repeatX | repeatY | noRepeat (not in android player)

<!-- -->  HTML-like comments in SMIL-File are supported and content inside will ignored

## additional features supported not in w3c standard
 - **shuffle play** in seq container via metadata
 - **automatic refresh** via metatags in SMIL header
 - **Logs/reports** system and inventory-reports, events- and playlogs via subscription meta and delivery by webdav like described in [a-smil.org Reporting](http://www.a-smil.org/index.php/Reporting)
 - **expr-attribute** in media tags for conditional play as described in [a-smil.org](http://www.a-smil.org/index.php/Conditional_play) by [IAdea](http://www.iadea.com/)
 - notify network trigger for begin and end attributes
 - placeholer in head with status and data tags
 - http downloads and caching
 - caching of remote SMIL index file
 - caching of remote media files
 - refresh via meta tag in cyclic timer periods
 - http 301 redirects for index and media
 - change config.xml during plaback
 - remote update

## HotKeys
 - toogle fullscreen with Ctrl-F (not in Android)
 - toogle bigscreen with Ctrl-B (not in Android)
 - quit app with Ctrl-Q
 - open configuration dialog with Ctrl-S

## future ToDo's
- REST API
- trigger for strings like ean-codes


Contact: [Nikolaos Sagiadinos] (mailto:ns@smil-control.com)
