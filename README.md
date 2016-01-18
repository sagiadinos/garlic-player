# garlic-player - SMIL Player for Digital Signage

The goal is to create an as most as neccessary SMIL 3.0 compatible and as most as possible platform independent multimedia player for Digital Signage proposes.

## OS targets:

Primary: Linux, Windows
Secondary: Android, OSX and iOS

Compiled whith Qt 5.5. and QtAV 1.8.0 for video and audio media under Kubuntu 14.04

At the moment there is an early alpha version which is able to player local SMIL-Files via command line under Linux.
garlic-player -f path_to_smil_file

## actual supported SMIL-Features

###Playlist Elements
- seq
- par
- excl (is heavily @work)

###Media-Elements
- video
- audio
- images
- text (websites)

###Attributes for media
- region
- fit          fill, scale, meetbest, none

###Attributes for media and playlists:
- xml:id       Fallback for SMIL 2.0 id
- dur          indefinite, media, Clock values (full+partial clock),  normal (h min s and ms)
- begin        Clock values (like dur) and wallcock with periods and repeats. Date is supported only in full ISO 8601 format not short date like 201634W at the moment.
- end          same as begin
- repeatCount  only integer values (later for floats)

###Layered multizone layouts:
- root-layout
- layout

###Multizone attributes
- regionName
- top
- left
- width
- height
- z-index
- backgroundColor (compatible with CSS)

Contact: Nikolaos Sagiadinos ns@smil-control.com